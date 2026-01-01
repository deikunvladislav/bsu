from __future__ import annotations
from functools import wraps
from flask import request, jsonify, current_app
from flask_jwt_extended import get_jwt_identity
import hashlib

def validate_request(schema_class):
    def decorator(f):
        @wraps(f)
        def decorated_function(*args, **kwargs):
            data = request.get_json(silent=True)
            if data is None:
                raise APIError("Invalid JSON", 400, "INVALID_JSON")
            
            try:
                schema = schema_class()
                errors = schema.validate(data)
                
                if errors:
                    error_details = []
                    for field, messages in errors.items():
                        error_details.append(f"{field}: {', '.join(messages)}")
                    raise APIError(
                        "Validation failed", 
                        400, 
                        "VALIDATION_ERROR",
                        errors=error_details
                    )
                
                return f(data, *args, **kwargs)
                
            except Exception as e:
                current_app.logger.error(f"Validation error: {str(e)}")
                if isinstance(e, APIError):
                    raise e
                else:
                    raise APIError(str(e), 400, "VALIDATION_ERROR")
                
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
        limit = 50
    elif limit > 100:
        limit = 100
    elif limit < 1:
        limit = 1
    
    if offset < 0:
        offset = 0
    
    return limit, offset

class APIError(Exception):
    def __init__(self, message, status_code=400, code=None, errors=None):
        super().__init__(message)
        self.message = message
        self.status_code = status_code
        self.code = code
        self.errors = errors
    
    def to_dict(self):
        result = {
            "message": self.message,
            "code": self.code or "API_ERROR"
        }
        if self.errors:
            result["errors"] = self.errors
        return result