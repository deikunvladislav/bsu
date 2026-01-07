from flask import Blueprint, render_template, session, redirect, url_for, g, request
from datetime import datetime
from ..models import User, Task

frontend_bp = Blueprint("frontend", __name__)

def get_current_user():
    """Get current user from session"""
    user_id = session.get('user_id')
    if user_id:
        return User.query.get(user_id)
    return None

@frontend_bp.before_app_request
def load_user():
    """Load user for all requests"""
    g.current_user = get_current_user()

@frontend_bp.context_processor
def inject_context():
    """Inject variables into all templates"""
    return {
        'current_user': g.get('current_user'),
        'current_time': datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    }

@frontend_bp.route("/")
def index():
    return render_template("index.html")

@frontend_bp.route("/login")
def login():
    if g.current_user:
        return redirect(url_for('frontend.dashboard'))
    return render_template("login.html")

@frontend_bp.route("/register")
def register():
    if g.current_user:
        return redirect(url_for('frontend.dashboard'))
    return render_template("register.html")

@frontend_bp.route("/dashboard")
def dashboard():
    if not g.current_user:
        return redirect(url_for('frontend.login'))
    return render_template("dashboard.html")

@frontend_bp.route("/tasks")
def tasks():
    if not g.current_user:
        return redirect(url_for('frontend.login'))
    return render_template("tasks.html")

@frontend_bp.route("/tasks/<int:task_id>/edit")
def edit_task(task_id):
    if not g.current_user:
        return redirect(url_for('frontend.login'))
    
    # Check if task belongs to user
    task = Task.query.filter_by(id=task_id, user_id=g.current_user.id).first()
    if not task:
        return redirect(url_for('frontend.tasks'))
    
    return render_template("edit_task.html", task_id=task_id)

@frontend_bp.route("/profile")
def profile():
    if not g.current_user:
        return redirect(url_for('frontend.login'))
    return render_template("profile.html")

@frontend_bp.route("/logout")
def logout():
    session.pop('user_id', None)
    return redirect(url_for('frontend.index'))

@frontend_bp.route("/about")
def about():
    return render_template("about.html")

@frontend_bp.route("/api-docs")
def api_docs():
    return render_template("api_docs.html")
