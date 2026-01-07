from __future__ import annotations
from flask import Blueprint, request, jsonify, current_app
from flask_jwt_extended import jwt_required, get_jwt_identity
from sqlalchemy.exc import SQLAlchemyError
from datetime import datetime, timezone
from ..models import db, Task, TaskStatus
from ..extensions import cache
from ..utils import invalidate_task_cache

tasks_bp = Blueprint("tasks", __name__)

@tasks_bp.route("/tasks", methods=["POST"])
@jwt_required()
def create_task():
    """Create a new task"""
    try:
        current_user_id = get_jwt_identity()
        
        data = request.get_json()
        if not data:
            return jsonify({
                "success": False,
                "message": "Invalid JSON",
                "code": "INVALID_JSON"
            }), 400
        
        # Validate required fields
        title = data.get("title")
        status = data.get("status", "todo")
        
        if not title or not isinstance(title, str) or len(title.strip()) == 0:
            return jsonify({
                "success": False,
                "message": "Title is required and must be a non-empty string",
                "code": "INVALID_TITLE"
            }), 400
        
        # Validate status
        try:
            task_status = TaskStatus(status)
        except ValueError:
            valid_statuses = TaskStatus.get_all()
            return jsonify({
                "success": False,
                "message": f"Invalid status. Must be one of: {', '.join(valid_statuses)}",
                "code": "INVALID_STATUS",
                "valid_statuses": valid_statuses
            }), 400
        
        # Create task
        task = Task(
            title=title.strip(),
            description=data.get("description", ""),
            status=task_status,
            user_id=current_user_id
        )
        
        db.session.add(task)
        db.session.commit()
        
        # Invalidate cache
        invalidate_task_cache(user_id=current_user_id)
        
        return jsonify({
            "success": True,
            "message": "Task created successfully",
            "data": task.to_dict()
        }), 201
        
    except SQLAlchemyError as e:
        db.session.rollback()
        current_app.logger.error(f"Database error in create_task: {str(e)}")
        return jsonify({
            "success": False,
            "message": "Database error",
            "code": "DATABASE_ERROR"
        }), 500
    except Exception as e:
        db.session.rollback()
        current_app.logger.error(f"Unexpected error in create_task: {str(e)}")
        return jsonify({
            "success": False,
            "message": "Internal server error",
            "code": "INTERNAL_ERROR"
        }), 500

@tasks_bp.route("/tasks", methods=["GET"])
@jwt_required()
def list_tasks():
    """Get all tasks for current user"""
    try:
        current_user_id = get_jwt_identity()
        
        status = request.args.get("status")
        limit = request.args.get("limit", type=int, default=100)
        offset = request.args.get("offset", type=int, default=0)
        
        query = Task.query.filter_by(user_id=current_user_id)
        
        if status:
            try:
                task_status = TaskStatus(status)
                query = query.filter_by(status=task_status)
            except ValueError:
                valid_statuses = TaskStatus.get_all()
                return jsonify({
                    "success": False,
                    "message": f"Invalid status. Must be one of: {', '.join(valid_statuses)}",
                    "code": "INVALID_STATUS"
                }), 400
        
        # Apply pagination
        if limit > 100:
            limit = 100
        elif limit < 1:
            limit = 1
        
        query = query.order_by(Task.created_at.desc())
        total = query.count()
        tasks = query.limit(limit).offset(offset).all()
        
        # Calculate statistics
        stats = {
            "todo": Task.query.filter_by(user_id=current_user_id, status=TaskStatus.TODO).count(),
            "in_progress": Task.query.filter_by(user_id=current_user_id, status=TaskStatus.IN_PROGRESS).count(),
            "done": Task.query.filter_by(user_id=current_user_id, status=TaskStatus.DONE).count(),
            "total": total
        }
        
        return jsonify({
            "success": True,
            "data": {
                "tasks": [task.to_dict() for task in tasks],
                "total": total,
                "limit": limit,
                "offset": offset,
                "stats": stats
            }
        }), 200
        
    except Exception as e:
        current_app.logger.error(f"Error in list_tasks: {str(e)}")
        return jsonify({
            "success": False,
            "message": "Failed to fetch tasks",
            "code": "FETCH_ERROR"
        }), 500

@tasks_bp.route("/tasks/<int:task_id>", methods=["GET"])
@jwt_required()
def get_task(task_id):
    """Get a specific task by ID"""
    try:
        current_user_id = get_jwt_identity()
        
        task = Task.query.filter_by(id=task_id, user_id=current_user_id).first()
        
        if not task:
            return jsonify({
                "success": False,
                "message": "Task not found",
                "code": "TASK_NOT_FOUND"
            }), 404
        
        return jsonify({
            "success": True,
            "data": task.to_dict()
        }), 200
        
    except Exception as e:
        current_app.logger.error(f"Error in get_task: {str(e)}")
        return jsonify({
            "success": False,
            "message": "Failed to fetch task",
            "code": "FETCH_ERROR"
        }), 500

@tasks_bp.route("/tasks/<int:task_id>", methods=["PUT"])
@jwt_required()
def update_task(task_id):
    """Update a task (full update)"""
    try:
        current_user_id = get_jwt_identity()
        
        task = Task.query.filter_by(id=task_id, user_id=current_user_id).first()
        
        if not task:
            return jsonify({
                "success": False,
                "message": "Task not found",
                "code": "TASK_NOT_FOUND"
            }), 404
        
        data = request.get_json()
        if not data:
            return jsonify({
                "success": False,
                "message": "No data provided",
                "code": "NO_DATA"
            }), 400
        
        # Validate required fields for PUT
        if "title" not in data:
            return jsonify({
                "success": False,
                "message": "Title is required",
                "code": "MISSING_TITLE"
            }), 400
        
        if "status" not in data:
            return jsonify({
                "success": False,
                "message": "Status is required",
                "code": "MISSING_STATUS"
            }), 400
        
        # Validate title
        title = data.get("title")
        if not isinstance(title, str) or len(title.strip()) == 0:
            return jsonify({
                "success": False,
                "message": "Title must be a non-empty string",
                "code": "INVALID_TITLE"
            }), 400
        
        # Validate status
        try:
            task_status = TaskStatus(data["status"])
        except ValueError:
            valid_statuses = TaskStatus.get_all()
            return jsonify({
                "success": False,
                "message": f"Invalid status. Must be one of: {', '.join(valid_statuses)}",
                "code": "INVALID_STATUS"
            }), 400
        
        # Update task
        task.title = title.strip()
        task.description = data.get("description", "")
        task.status = task_status
        
        db.session.commit()
        invalidate_task_cache(user_id=current_user_id, task_id=task_id)
        
        return jsonify({
            "success": True,
            "message": "Task updated successfully",
            "data": task.to_dict()
        }), 200
        
    except SQLAlchemyError as e:
        db.session.rollback()
        current_app.logger.error(f"Database error in update_task: {str(e)}")
        return jsonify({
            "success": False,
            "message": "Database error",
            "code": "DATABASE_ERROR"
        }), 500
    except Exception as e:
        db.session.rollback()
        current_app.logger.error(f"Unexpected error in update_task: {str(e)}")
        return jsonify({
            "success": False,
            "message": "Internal server error",
            "code": "INTERNAL_ERROR"
        }), 500

@tasks_bp.route("/tasks/<int:task_id>", methods=["PATCH"])
@jwt_required()
def patch_task(task_id):
    """Partially update a task"""
    try:
        current_user_id = get_jwt_identity()
        
        task = Task.query.filter_by(id=task_id, user_id=current_user_id).first()
        
        if not task:
            return jsonify({
                "success": False,
                "message": "Task not found",
                "code": "TASK_NOT_FOUND"
            }), 404
        
        data = request.get_json()
        if not data:
            return jsonify({
                "success": False,
                "message": "No data provided",
                "code": "NO_DATA"
            }), 400
        
        updated = False
        
        # Update title if provided
        if "title" in data:
            title = data["title"]
            if not isinstance(title, str) or len(title.strip()) == 0:
                return jsonify({
                    "success": False,
                    "message": "Title must be a non-empty string",
                    "code": "INVALID_TITLE"
                }), 400
            task.title = title.strip()
            updated = True
        
        # Update description if provided
        if "description" in data:
            task.description = data["description"] or ""
            updated = True
        
        # Update status if provided
        if "status" in data:
            try:
                task_status = TaskStatus(data["status"])
                task.status = task_status
                updated = True
            except ValueError:
                valid_statuses = TaskStatus.get_all()
                return jsonify({
                    "success": False,
                    "message": f"Invalid status. Must be one of: {', '.join(valid_statuses)}",
                    "code": "INVALID_STATUS"
                }), 400
        
        if not updated:
            return jsonify({
                "success": False,
                "message": "No valid fields to update",
                "code": "NO_VALID_FIELDS"
            }), 400
        
        db.session.commit()
        invalidate_task_cache(user_id=current_user_id, task_id=task_id)
        
        return jsonify({
            "success": True,
            "message": "Task updated successfully",
            "data": task.to_dict()
        }), 200
        
    except SQLAlchemyError as e:
        db.session.rollback()
        current_app.logger.error(f"Database error in patch_task: {str(e)}")
        return jsonify({
            "success": False,
            "message": "Database error",
            "code": "DATABASE_ERROR"
        }), 500
    except Exception as e:
        db.session.rollback()
        current_app.logger.error(f"Unexpected error in patch_task: {str(e)}")
        return jsonify({
            "success": False,
            "message": "Internal server error",
            "code": "INTERNAL_ERROR"
        }), 500

@tasks_bp.route("/tasks/<int:task_id>", methods=["DELETE"])
@jwt_required()
def delete_task(task_id):
    """Delete a task"""
    try:
        current_user_id = get_jwt_identity()
        
        task = Task.query.filter_by(id=task_id, user_id=current_user_id).first()
        
        if not task:
            return jsonify({
                "success": False,
                "message": "Task not found",
                "code": "TASK_NOT_FOUND"
            }), 404
        
        db.session.delete(task)
        db.session.commit()
        
        invalidate_task_cache(user_id=current_user_id, task_id=task_id)
        
        return jsonify({
            "success": True,
            "message": "Task deleted successfully",
            "data": {
                "task_id": task_id
            }
        }), 200
        
    except SQLAlchemyError as e:
        db.session.rollback()
        current_app.logger.error(f"Database error in delete_task: {str(e)}")
        return jsonify({
            "success": False,
            "message": "Database error",
            "code": "DATABASE_ERROR"
        }), 500
    except Exception as e:
        db.session.rollback()
        current_app.logger.error(f"Unexpected error in delete_task: {str(e)}")
        return jsonify({
            "success": False,
            "message": "Internal server error",
            "code": "INTERNAL_ERROR"
        }), 500

@tasks_bp.route("/tasks/stats", methods=["GET"])
@jwt_required()
def get_task_stats():
    """Get task statistics for current user"""
    try:
        current_user_id = get_jwt_identity()
        
        stats = {
            "todo": Task.query.filter_by(user_id=current_user_id, status=TaskStatus.TODO).count(),
            "in_progress": Task.query.filter_by(user_id=current_user_id, status=TaskStatus.IN_PROGRESS).count(),
            "done": Task.query.filter_by(user_id=current_user_id, status=TaskStatus.DONE).count(),
            "total": Task.query.filter_by(user_id=current_user_id).count()
        }
        
        return jsonify({
            "success": True,
            "data": stats
        }), 200
        
    except Exception as e:
        current_app.logger.error(f"Error in get_task_stats: {str(e)}")
        return jsonify({
            "success": False,
            "message": "Failed to get task statistics",
            "code": "STATS_ERROR"
        }), 500