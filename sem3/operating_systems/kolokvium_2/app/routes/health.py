from datetime import datetime, timezone
from flask import Blueprint, jsonify, current_app
from sqlalchemy import text
from ..models import db
from ..extensions import cache

health_bp = Blueprint("health", __name__)

@health_bp.route("/live", methods=["GET"])
def liveness_check():
    return jsonify({
        "status": "alive",
        "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
    }), 200

@health_bp.route("/ready", methods=["GET"])
def readiness_check():
    checks = {
        "database": False,
        "cache": False,
        "overall": False
    }
    
    try:
        result = db.session.execute(text("SELECT 1")).scalar()
        if result == 1:  
            checks["database"] = True
    except Exception as e:
        current_app.logger.warning(f"Database check failed: {e}")
        checks["database"] = False
    
    cache_type = current_app.config.get("CACHE_TYPE", "NullCache")
    if cache_type == "NullCache":
        checks["cache"] = True
    else:
        try:
            cache.set("health_check", "ok", timeout=5)
            if cache.get("health_check") == "ok": 
                checks["cache"] = True
            else:
                checks["cache"] = False
        except Exception as e:
            current_app.logger.warning(f"Cache check failed: {e}")
            checks["cache"] = False
    
    checks["overall"] = checks["database"] and checks["cache"]
    
    status_code = 200 if checks["overall"] else 503
    
    return jsonify({
        "status": "ready" if checks["overall"] else "not ready",
        "checks": checks,
        "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z"),
        "database_url": current_app.config.get('SQLALCHEMY_DATABASE_URI', 'unknown'),
        "cache_type": cache_type
    }), status_code