from __future__ import annotations
from flask import Blueprint, request, jsonify, current_app, session
from flask_jwt_extended import create_access_token, jwt_required, get_jwt_identity
from ..models import db, User, Task
from ..utils import APIError, validate_password, validate_username, MIN_USERNAME_LENGTH, MAX_USERNAME_LENGTH, MIN_PASSWORD_LENGTH

auth_bp = Blueprint("auth", __name__)

@auth_bp.route("/register", methods=["POST"])
def register():
    data = request.get_json()
    if not data:
        raise APIError("Invalid JSON", 400, "INVALID_JSON")
    
    username = data.get("username")
    password = data.get("password")
    
    if not username or not password:
        raise APIError("Username and password are required", 400, "MISSING_FIELDS")
    
    if not validate_username(username):
        raise APIError(f"Username must be {MIN_USERNAME_LENGTH}-{MAX_USERNAME_LENGTH} characters and contain only letters and numbers", 
                      400, "INVALID_USERNAME")
    
    if not validate_password(password):
        raise APIError(f"Password must be at least {MIN_PASSWORD_LENGTH} characters", 
                      400, "INVALID_PASSWORD")
    
    existing_user = User.query.filter_by(username=username).first()
    if existing_user:
        raise APIError("Username already exists", 409, "USERNAME_EXISTS")
    
    user = User(username=username)
    user.set_password(password)
    
    try:
        db.session.add(user)
        db.session.commit()
        
        return jsonify({
            "message": "User registered successfully",
            "user_id": user.id,
            "username": user.username
        }), 201
        
    except Exception as e:
        db.session.rollback()
        current_app.logger.error(f"Registration error for user {username}: {str(e)}")
        raise APIError("Registration failed", 500, "REGISTRATION_ERROR")

@auth_bp.route("/login", methods=["POST"])
def login():
    data = request.get_json()
    if not data:
        raise APIError("Invalid JSON", 400, "INVALID_JSON")
    
    username = data.get("username")
    password = data.get("password")
    
    if not username or not password:
        raise APIError("Username and password are required", 400, "MISSING_FIELDS")
    
    user = User.query.filter_by(username=username).first()
    
    if not user:
        raise APIError("Invalid username or password", 401, "INVALID_CREDENTIALS")
    
    if not user.check_password(password):
        raise APIError("Invalid username or password", 401, "INVALID_CREDENTIALS")
    
    try:
        access_token = create_access_token(
            identity=str(user.id),
            additional_claims={
                "username": user.username,
                "user_id": user.id
            }
        )
        
        # Set user_id in session for web interface
        session['user_id'] = user.id
        
        return jsonify({
            "access_token": access_token,
            "token_type": "bearer",
            "user": user.to_dict()
        }), 200
        
    except Exception as e:
        current_app.logger.error(f"Token creation error: {str(e)}")
        raise APIError("Login failed", 500, "LOGIN_ERROR")

@auth_bp.route("/logout", methods=["POST"])
def logout_api():
    """API logout endpoint"""
    session.pop('user_id', None)
    return jsonify({"message": "Logged out successfully"}), 200

@auth_bp.route("/profile", methods=["GET"])
@jwt_required()
def get_profile():
    current_user_id = get_jwt_identity()
    user = User.query.get(current_user_id)
    
    if not user:
        raise APIError("User not found", 404, "USER_NOT_FOUND")
    
    return jsonify(user.to_dict()), 200

@auth_bp.route("/change-password", methods=["POST"])
@jwt_required()
def change_password():
    current_user_id = get_jwt_identity()
    data = request.get_json()
    
    if not data:
        raise APIError("Invalid JSON", 400, "INVALID_JSON")
    
    old_password = data.get("old_password")
    new_password = data.get("new_password")
    
    if not old_password or not new_password:
        raise APIError("Old and new password required", 400, "MISSING_FIELDS")
    
    if not validate_password(new_password):
        raise APIError(f"New password must be at least {MIN_PASSWORD_LENGTH} characters", 
                      400, "INVALID_PASSWORD")
    
    user = User.query.get(current_user_id)
    if not user:
        raise APIError("User not found", 404, "USER_NOT_FOUND")
    
    if not user.check_password(old_password):
        raise APIError("Old password is incorrect", 400, "INCORRECT_OLD_PASSWORD")
    
    try:
        user.set_password(new_password)
        db.session.commit()
        
        return jsonify({
            "message": "Password changed successfully"
        }), 200
        
    except Exception as e:
        db.session.rollback()
        current_app.logger.error(f"Password change error: {str(e)}")
        raise APIError("Failed to change password", 500, "PASSWORD_CHANGE_ERROR")

@auth_bp.route("/delete-account", methods=["DELETE"])
@jwt_required()
def delete_user_account():
    """Delete user account"""
    current_user_id = get_jwt_identity()
    user = User.query.get(current_user_id)
    
    if not user:
        raise APIError("User not found", 404, "USER_NOT_FOUND")
    
    try:
        # Delete all user's tasks first
        Task.query.filter_by(user_id=current_user_id).delete()
        # Delete user
        db.session.delete(user)
        db.session.commit()
        
        # Clear session
        session.pop('user_id', None)
        
        return jsonify({
            "message": "Account deleted successfully"
        }), 200
        
    except Exception as e:
        db.session.rollback()
        current_app.logger.error(f"Error deleting account: {str(e)}")
        raise APIError("Failed to delete account", 500, "ACCOUNT_DELETE_ERROR")