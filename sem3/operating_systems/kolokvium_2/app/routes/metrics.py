from datetime import datetime, timezone
from flask import Blueprint, Response, jsonify
from prometheus_client import generate_latest, CONTENT_TYPE_LATEST
from ..models import Task, User

metrics_bp = Blueprint("metrics", __name__)

@metrics_bp.route("/metrics", methods=["GET"])
def get_metrics():
    return Response(
        generate_latest(),
        mimetype=CONTENT_TYPE_LATEST
    )

@metrics_bp.route("/stats", methods=["GET"])
def get_stats():
    stats = {
        "users": {
            "total": User.query.count(),
            "active_today": User.query.filter(
                User.updated_at >= datetime.now(timezone.utc).date()
            ).count()
        },
        "tasks": {
            "total": Task.query.count(),
            "by_status": {
                "todo": Task.query.filter_by(status="todo").count(),
                "in_progress": Task.query.filter_by(status="in_progress").count(),
                "done": Task.query.filter_by(status="done").count(),
            }
        }
    }
    
    return jsonify(stats), 200