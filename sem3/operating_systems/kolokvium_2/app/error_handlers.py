from __future__ import annotations
from flask import jsonify, request
from sqlalchemy.exc import SQLAlchemyError
from datetime import datetime, timezone
from .utils import APIError
from flask_jwt_extended.exceptions import JWTExtendedException

def setup_error_handlers(app):
    
    @app.errorhandler(APIError)
    def handle_api_error(error):
        response = jsonify(error.to_dict())
        response.status_code = error.status_code
        return response
    
    @app.errorhandler(JWTExtendedException)
    def handle_jwt_error(error):
        return jsonify({
            "message": str(error),
            "code": "JWT_ERROR",
            "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
        }), 401
    
    @app.errorhandler(400)
    def handle_bad_request(error):
        return jsonify({
            "message": "Bad request",
            "code": "BAD_REQUEST",
            "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
        }), 400
    
    @app.errorhandler(404)
    def handle_not_found(error):
        return jsonify({
            "message": "Resource not found",
            "code": "NOT_FOUND",
            "path": request.path,
            "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
        }), 404
    
    @app.errorhandler(405)
    def handle_method_not_allowed(error):
        return jsonify({
            "message": "Method not allowed",
            "code": "METHOD_NOT_ALLOWED",
            "allowed_methods": error.valid_methods,
            "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
        }), 405
    
    @app.errorhandler(415)
    def handle_unsupported_media_type(error):
        return jsonify({
            "message": "Unsupported media type. Use application/json",
            "code": "UNSUPPORTED_MEDIA_TYPE",
            "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
        }), 415
    
    @app.errorhandler(500)
    def handle_internal_error(error):
        app.logger.error(f"Internal server error: {error}")
        
        return jsonify({
            "message": "Internal server error",
            "code": "INTERNAL_ERROR",
            "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
        }), 500
    
    @app.errorhandler(SQLAlchemyError)
    def handle_database_error(error):
        app.logger.error(f"Database error: {error}")
        
        return jsonify({
            "message": "Database error occurred",
            "code": "DATABASE_ERROR",
            "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
        }), 500
    
    @app.errorhandler(Exception)
    def handle_generic_error(error):
        app.logger.error(f"Unhandled error: {error}")
        
        if hasattr(error, 'code'):
            if error.code == 415:
                return jsonify({
                    "message": "Unsupported media type. Use application/json",
                    "code": "UNSUPPORTED_MEDIA_TYPE",
                    "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
                }), 415
        
        return jsonify({
            "message": "An unexpected error occurred",
            "code": "UNEXPECTED_ERROR",
            "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
        }), 500