from __future__ import annotations
from functools import wraps
from flask import request, jsonify, current_app
from flask_jwt_extended import get_jwt_identity
import hashlib
from .extensions import cache

MIN_USERNAME_LENGTH = 3
MAX_USERNAME_LENGTH = 80
MIN_PASSWORD_LENGTH = 6
DEFAULT_PAGE_LIMIT = 50
MAX_PAGE_LIMIT = 100

def validate_request(schema_class):
    def decorator(f):
        @wraps(f)
        def decorated_function(*args, **kwargs):
            data = request.get_json(silent=True)
            if data is None:
                return jsonify({
                    "success": False,
                    "message": "Invalid JSON",
                    "code": "INVALID_JSON"
                }), 400
            
            try:
                schema = schema_class()
                errors = schema.validate(data)
                
                if errors:
                    error_details = []
                    for field, messages in errors.items():
                        error_details.append(f"{field}: {', '.join(messages)}")
                    return jsonify({
                        "success": False,
                        "message": "Validation failed",
                        "code": "VALIDATION_ERROR",
                        "errors": error_details
                    }), 400
                
                return f(data, *args, **kwargs)
                
            except Exception as e:
                current_app.logger.error(f"Validation error: {str(e)}")
                return jsonify({
                    "success": False,
                    "message": str(e),
                    "code": "VALIDATION_ERROR"
                }), 400
                
        return decorated_function
    return decorator

def cache_key_prefix():
    try:
        user_id = get_jwt_identity()
        return f"user_{user_id}"
    except:
        return "anonymous"

def hash_password(password: str) -> str:
    return hashlib.sha256(password.encode()).hexdigest()

def get_pagination_params():
    limit = request.args.get('limit', type=int)
    offset = request.args.get('offset', 0, type=int)
    
    if limit is None:
        limit = DEFAULT_PAGE_LIMIT
    elif limit > MAX_PAGE_LIMIT:
        limit = MAX_PAGE_LIMIT
    elif limit < 1:
        limit = 1
    
    if offset < 0:
        offset = 0
    
    return limit, offset

def validate_password(password: str, min_length: int = MIN_PASSWORD_LENGTH) -> bool:
    if not password or len(password) < min_length:
        return False
    return True

def validate_username(username: str, min_len: int = MIN_USERNAME_LENGTH, max_len: int = MAX_USERNAME_LENGTH) -> bool:
    if not username or len(username) < min_len or len(username) > max_len:
        return False
    if not username.isalnum():
        return False
    return True

def invalidate_task_cache(user_id: int = None, task_id: int = None):
    if user_id:
        cache.delete(f"user_tasks_{user_id}")
    if task_id:
        cache.delete(f"task_{task_id}")
    cache.delete("all_tasks_stats")

class APIError(Exception):
    def __init__(self, message, status_code=400, code=None, errors=None):
        super().__init__(message)
        self.message = message
        self.status_code = status_code
        self.code = code
        self.errors = errors
    
    def to_dict(self):
        result = {
            "success": False,
            "message": self.message,
            "code": self.code or "API_ERROR"
        }
        if self.errors:
            result["errors"] = self.errors
        return result