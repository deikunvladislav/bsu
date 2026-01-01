import sys
import os
from sqlalchemy import text
from app import create_app
from app.models import db

def main():
    app = create_app("development")
    
    with app.app_context():
        try:
            print("Checking database...")
            
            db.create_all()
            print("✓ Database tables verified")
            
            result = db.session.execute(text("SELECT 1")).scalar()
            if 1 == result:
                print("✓ Database connection successful")
            else:
                print("⚠ Database connection test returned unexpected result")
                
        except Exception as e:
            print(f"⚠ Database warning: {e}")
            print("Server will start, but database may not be available")
            print("Run 'python init_db.py' to initialize database")
    
    print("\n" + "="*50)
    print("Todo API Server")
    print("="*50)
    print("Starting server on http://localhost:8000")
    print("Press Ctrl+C to stop\n")
    
    print("Available endpoints:")
    print("- http://localhost:8000/health/live")
    print("- http://localhost:8000/health/ready")
    print("- http://localhost:8000/auth/register (POST)")
    print("- http://localhost:8000/auth/login (POST)")
    print("- http://localhost:8000/tasks (GET/POST)")
    print("- http://localhost:8000/tasks/<id> (GET/PUT/PATCH/DELETE)")
    print("- http://localhost:8000/metrics")
    print("="*50 + "\n")
    
    try:
        app.run(
            host="0.0.0.0",
            port=8000,
            debug=app.config.get("DEBUG", True),
            use_reloader=True
        )
    except KeyboardInterrupt:
        print("\n\nServer stopped by user")
        sys.exit(0)
    except Exception as e:
        print(f"\n\nError running server: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()