from __future__ import annotations
from flask import Blueprint, request, jsonify, current_app
from flask_jwt_extended import jwt_required, get_jwt_identity
from ..models import db, Task, TaskStatus, User
from ..extensions import cache, TASKS_CREATED_TOTAL, TASKS_UPDATED_TOTAL
from ..utils import cache_key_prefix, APIError, invalidate_task_cache
from ..validators import validate_task_status, validate_task_data

tasks_bp = Blueprint("tasks", __name__)

@tasks_bp.route("/tasks", methods=["POST"])
@jwt_required()
def create_task():
    current_user_id = get_jwt_identity()
    
    data = request.get_json()
    if not data:
        raise APIError("Invalid JSON", 400, "INVALID_JSON")
    
    if "title" not in data:
        raise APIError("Title is required", 400, "MISSING_TITLE")
    
    if "status" not in data:
        raise APIError("Status is required", 400, "MISSING_STATUS")
    
    if not isinstance(data["title"], str) or len(data["title"].strip()) == 0:
        raise APIError("Title must be a non-empty string", 400, "INVALID_TITLE")
    
    task_status = validate_task_status(data["status"])
    
    try:
        task = Task(
            title=data["title"].strip(),
            description=data.get("description", ""),
            status=task_status,
            user_id=current_user_id
        )
        
        db.session.add(task)
        db.session.commit()
        
        invalidate_task_cache(user_id=current_user_id)
        
        TASKS_CREATED_TOTAL.labels(user_id=current_user_id).inc()
        
        return jsonify(task.to_dict()), 201
        
    except Exception as e:
        db.session.rollback()
        current_app.logger.error(f"Create task error for user {current_user_id}: {str(e)}")
        raise APIError("Failed to create task", 500, "TASK_CREATE_ERROR")

@tasks_bp.route("/tasks", methods=["GET"])
@jwt_required()
@cache.cached(timeout=60, key_prefix=cache_key_prefix)
def list_tasks():
    current_user_id = get_jwt_identity()
    
    status = request.args.get("status")
    limit = request.args.get("limit", type=int)
    offset = request.args.get("offset", 0, type=int)
    
    query = Task.query.filter_by(user_id=current_user_id)
    
    if status:
        try:
            task_status = TaskStatus(status)
            query = query.filter_by(status=task_status)
        except ValueError:
            raise APIError(f"Invalid status. Must be one of: {', '.join(TaskStatus.get_all())}", 
                            400, "INVALID_STATUS")
    
    if limit:
        if limit > 100:
            limit = 100
        elif limit < 1:
            limit = 1
        query = query.limit(limit).offset(offset)
    
    query = query.order_by(Task.created_at.desc())
    
    tasks = query.all()
    return jsonify([task.to_dict() for task in tasks]), 200

@tasks_bp.route("/tasks/<int:task_id>", methods=["GET"])
@jwt_required()
def get_task(task_id):
    current_user_id = get_jwt_identity()
    
    task = Task.query.filter_by(id=task_id, user_id=current_user_id).first()
    
    if not task:
        raise APIError("Task not found", 404, "TASK_NOT_FOUND")
    
    return jsonify(task.to_dict()), 200

@tasks_bp.route("/tasks/<int:task_id>", methods=["PUT"])
@jwt_required()
def replace_task(task_id):
    current_user_id = get_jwt_identity()
    
    task = Task.query.filter_by(id=task_id, user_id=current_user_id).first()
    
    if not task:
        raise APIError("Task not found", 404, "TASK_NOT_FOUND")
    
    data = request.get_json()
    if not data:
        raise APIError("No data provided for update", 400, "NO_DATA")
    
    required_fields = ['title', 'status']
    for field in required_fields:
        if field not in data:
            raise APIError(f"Missing required field: {field}", 400, "MISSING_FIELD")
    
    if not isinstance(data['title'], str) or len(data['title'].strip()) == 0:
        raise APIError("Title must be a non-empty string", 400, "INVALID_TITLE")
    
    task_status = validate_task_status(data['status'])
    
    task.title = data['title'].strip()
    task.description = data.get('description', '') or ''
    task.status = task_status
    
    try:
        db.session.commit()
        
        invalidate_task_cache(user_id=current_user_id, task_id=task_id)
        
        TASKS_UPDATED_TOTAL.labels(
            user_id=current_user_id,
            status=data['status']
        ).inc()
        
        return jsonify(task.to_dict()), 200
        
    except Exception as e:
        db.session.rollback()
        current_app.logger.error(f"Replace task error for task {task_id}: {str(e)}")
        raise APIError("Failed to update task", 500, "TASK_UPDATE_ERROR")

@tasks_bp.route("/tasks/<int:task_id>", methods=["PATCH"])
@jwt_required()
def update_task(task_id):
    current_user_id = get_jwt_identity()
    
    task = Task.query.filter_by(id=task_id, user_id=current_user_id).first()
    
    if not task:
        raise APIError("Task not found", 404, "TASK_NOT_FOUND")
    
    data = request.get_json()
    if not data:
        raise APIError("No data provided for update", 400, "NO_DATA")
    
    updated = False
    
    if "title" in data and data["title"] is not None:
        if not isinstance(data["title"], str) or len(data["title"].strip()) == 0:
            raise APIError("Title must be a non-empty string", 400, "INVALID_TITLE")
        task.title = data["title"].strip()
        updated = True
    
    if "description" in data:
        task.description = data["description"] if data["description"] is not None else ""
        updated = True
    
    if "status" in data and data["status"] is not None:
        task_status = validate_task_status(data["status"])
        task.status = task_status
        updated = True
    
    if not updated:
        raise APIError("No valid fields to update", 400, "NO_VALID_FIELDS")
    
    try:
        db.session.commit()
        
        invalidate_task_cache(user_id=current_user_id, task_id=task_id)
        
        if "status" in data:
            TASKS_UPDATED_TOTAL.labels(
                user_id=current_user_id,
                status=data["status"]
            ).inc()
        
        return jsonify(task.to_dict()), 200
        
    except Exception as e:
        db.session.rollback()
        current_app.logger.error(f"Update task error for task {task_id}: {str(e)}")
        raise APIError("Failed to update task", 500, "TASK_UPDATE_ERROR")

@tasks_bp.route("/tasks/<int:task_id>", methods=["DELETE"])
@jwt_required()
def delete_task(task_id):
    current_user_id = get_jwt_identity()
    
    task = Task.query.filter_by(id=task_id, user_id=current_user_id).first()
    
    if not task:
        raise APIError("Task not found", 404, "TASK_NOT_FOUND")
    
    try:
        db.session.delete(task)
        db.session.commit()
        
        invalidate_task_cache(user_id=current_user_id, task_id=task_id)
        
        return "", 204
        
    except Exception as e:
        db.session.rollback()
        current_app.logger.error(f"Delete task error for task {task_id}: {str(e)}")
        raise APIError("Failed to delete task", 500, "TASK_DELETE_ERROR")

@tasks_bp.route("/tasks/stats", methods=["GET"])
@jwt_required()
@cache.cached(timeout=300, key_prefix=cache_key_prefix)
def get_stats():
    current_user_id = get_jwt_identity()
    
    stats = {
        "total": Task.query.filter_by(user_id=current_user_id).count(),
        "todo": Task.query.filter_by(user_id=current_user_id, status=TaskStatus.TODO).count(),
        "in_progress": Task.query.filter_by(user_id=current_user_id, status=TaskStatus.IN_PROGRESS).count(),
        "done": Task.query.filter_by(user_id=current_user_id, status=TaskStatus.DONE).count(),
    }
    
    return jsonify(stats), 200

@tasks_bp.route("/tasks/search", methods=["GET"])
@jwt_required()
def search_tasks():
    current_user_id = get_jwt_identity()
    query = request.args.get("query", "")
    
    if not query:
        raise APIError("Search query required", 400, "MISSING_QUERY")
    
    tasks = Task.query.filter_by(user_id=current_user_id).filter(
        (Task.title.ilike(f"%{query}%")) | 
        (Task.description.ilike(f"%{query}%"))
    ).order_by(Task.created_at.desc()).all()
    
    return jsonify([task.to_dict() for task in tasks]), 200