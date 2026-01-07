from __future__ import annotations
from flask import jsonify, request, render_template
from sqlalchemy.exc import SQLAlchemyError
from datetime import datetime, timezone
from .utils import APIError
from flask_jwt_extended.exceptions import JWTExtendedException
from jinja2 import TemplateNotFound

def setup_error_handlers(app):
    
    @app.errorhandler(APIError)
    def handle_api_error(error):
        current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        if request.path.startswith('/api/') or request.is_json:
            response = jsonify(error.to_dict())
            response.status_code = error.status_code
            return response
        else:
            return render_template('error.html', 
                                 error=error,
                                 status_code=error.status_code,
                                 current_time=current_time), error.status_code
    
    @app.errorhandler(JWTExtendedException)
    def handle_jwt_error(error):
        current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        return jsonify({
            "message": str(error),
            "code": "JWT_ERROR",
            "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
        }), 401
    
    @app.errorhandler(400)
    def handle_bad_request(error):
        current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        if request.path.startswith('/api/') or request.is_json:
            return jsonify({
                "message": "Bad request",
                "code": "BAD_REQUEST",
                "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
            }), 400
        else:
            return render_template('error.html', 
                                 error_message="Bad request",
                                 status_code=400,
                                 current_time=current_time), 400
    
    @app.errorhandler(404)
    def handle_not_found(error):
        current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        if request.path.startswith('/api/') or request.is_json:
            return jsonify({
                "message": "Resource not found",
                "code": "NOT_FOUND",
                "path": request.path,
                "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
            }), 404
        else:
            return render_template('error.html', 
                                 error_message="Page not found",
                                 status_code=404,
                                 current_time=current_time), 404
    
    @app.errorhandler(405)
    def handle_method_not_allowed(error):
        current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        return jsonify({
            "message": "Method not allowed",
            "code": "METHOD_NOT_ALLOWED",
            "allowed_methods": error.valid_methods,
            "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
        }), 405
    
    @app.errorhandler(415)
    def handle_unsupported_media_type(error):
        current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        return jsonify({
            "message": "Unsupported media type. Use application/json",
            "code": "UNSUPPORTED_MEDIA_TYPE",
            "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
        }), 415
    
    @app.errorhandler(500)
    def handle_internal_error(error):
        current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        app.logger.error(f"Internal server error: {error}")
        
        if request.path.startswith('/api/') or request.is_json:
            return jsonify({
                "message": "Internal server error",
                "code": "INTERNAL_ERROR",
                "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
            }), 500
        else:
            return render_template('error.html', 
                                 error_message="Internal server error",
                                 status_code=500,
                                 current_time=current_time), 500
    
    @app.errorhandler(SQLAlchemyError)
    def handle_database_error(error):
        current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        app.logger.error(f"Database error: {error}")
        
        if request.path.startswith('/api/') or request.is_json:
            return jsonify({
                "message": "Database error occurred",
                "code": "DATABASE_ERROR",
                "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
            }), 500
        else:
            return render_template('error.html', 
                                 error_message="Database error",
                                 status_code=500,
                                 current_time=current_time), 500
    
    @app.errorhandler(TemplateNotFound)
    def handle_template_not_found(error):
        current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        app.logger.error(f"Template not found: {error}")
        
        if request.path.startswith('/api/') or request.is_json:
            return jsonify({
                "message": "Template not found",
                "code": "TEMPLATE_NOT_FOUND",
                "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
            }), 404
        else:
            return render_template('error.html',
                                 error_message="Page not found",
                                 status_code=404,
                                 current_time=current_time), 404
    
    @app.errorhandler(Exception)
    def handle_generic_error(error):
        current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        app.logger.error(f"Unhandled error: {error}")
        
        if hasattr(error, 'code'):
            if error.code == 415:
                return jsonify({
                    "message": "Unsupported media type. Use application/json",
                    "code": "UNSUPPORTED_MEDIA_TYPE",
                    "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
                }), 415
        
        if request.path.startswith('/api/') or request.is_json:
            return jsonify({
                "message": "An unexpected error occurred",
                "code": "UNEXPECTED_ERROR",
                "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
            }), 500
        else:
            return render_template('error.html',
                                 error_message="An unexpected error occurred",
                                 status_code=500,
                                 current_time=current_time), 500