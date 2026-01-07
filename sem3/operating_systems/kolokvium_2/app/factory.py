from __future__ import annotations
import os
from flask import Flask
from dotenv import load_dotenv
from sqlalchemy import text
from .config import Config, DevelopmentConfig, ProductionConfig, TestingConfig
from .extensions import db, jwt, cache
from .logging_config import setup_logging
from .routes.auth import auth_bp
from .routes.tasks import tasks_bp
from .routes.health import health_bp
from .routes.frontend import frontend_bp
from .error_handlers import setup_error_handlers

def create_app(config_name: str = None) -> Flask:
    load_dotenv()
    
    config_map = {
        'development': DevelopmentConfig,
        'production': ProductionConfig,
        'testing': TestingConfig,
        None: Config,
    }
    
    config_class = config_map.get(config_name, Config)
    config = config_class()
    
    setup_logging(config.LOG_LEVEL)
    
    app = Flask(__name__)
    app.config.from_object(config)
    
    # Set secret key for sessions
    app.secret_key = config.SECRET_KEY
    
    db.init_app(app)
    jwt.init_app(app)
    cache.init_app(app, config={
        'CACHE_TYPE': config.CACHE_TYPE,
        'CACHE_DEFAULT_TIMEOUT': config.CACHE_DEFAULT_TIMEOUT
    })
    
    setup_error_handlers(app)
    
    # Register API blueprints with /api prefix
    app.register_blueprint(auth_bp, url_prefix="/api/auth")
    app.register_blueprint(tasks_bp, url_prefix="/api")
    app.register_blueprint(health_bp, url_prefix="/api/health")
    
    # Register frontend blueprint
    app.register_blueprint(frontend_bp, url_prefix="")
    
    @app.before_request
    def create_tables_if_needed():
        if not hasattr(app, 'tables_created'):
            try:
                with app.app_context():
                    db.create_all()
                    
                    try:
                        db.session.execute(text("SELECT 1")).scalar()
                        app.logger.info("Database connection test successful")
                    except Exception as e:
                        app.logger.error(f"Database connection test failed: {e}")
                    
                    app.tables_created = True
                    app.logger.info("Database tables verified")
            except Exception as e:
                app.logger.error(f"Error creating tables: {e}")
    
    @app.cli.command("init-db")
    def init_db():
        with app.app_context():
            db.create_all()
            print("Database initialized")
    
    @app.cli.command("create-user")
    def create_user():
        import getpass
        username = input("Username: ")
        password = getpass.getpass("Password: ")
        
        from werkzeug.security import generate_password_hash
        from .models import User
        
        with app.app_context():
            user = User(username=username, password_hash=generate_password_hash(password))
            db.session.add(user)
            db.session.commit()
        print(f"User '{username}' created")
    
    return app