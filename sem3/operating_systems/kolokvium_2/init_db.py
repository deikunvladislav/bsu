import sys
import os
from sqlalchemy import text, inspect

sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from app import create_app
from app.models import db

def main():
    """Main function to initialize database"""
    print("Initializing database...")
    
    app = create_app("development")
    
    with app.app_context():
        try:
            db_url = app.config['SQLALCHEMY_DATABASE_URI']
            if db_url.startswith('sqlite:///'):
                db_path = db_url.replace('sqlite:///', '')
                print(f"SQLite database path: {db_path}")
                
                if os.path.exists(db_path):
                    print(f"✓ Database file exists: {db_path}")
                    
                    try:
                        result = db.session.execute(text("SELECT 1")).scalar()
                        if result == 1:
                            print("✓ Database connection successful")
                        else:
                            print("⚠ Database connection returned unexpected value")
                        
                        inspector = inspect(db.engine)
                        tables = inspector.get_table_names()
                        
                        if tables:
                            print(f"✓ Found {len(tables)} table(s): {', '.join(tables)}")
                            if 'users' in tables and 'tasks' in tables:
                                print("✓ All required tables exist")
                            else:
                                print("⚠ Some tables missing, creating them...")
                                db.create_all()
                                print("✓ Missing tables created")
                        else:
                            print("✗ No tables found, creating all tables...")
                            db.create_all()
                            print("✓ All tables created")
                    except Exception as e:
                        print(f"✗ Database error: {e}")
                        print("Recreating database...")
                        if os.path.exists(db_path):
                            os.remove(db_path)
                        db.create_all()
                        print("✓ Database recreated successfully")
                else:
                    print(f"Creating new database: {db_path}")
                    db.create_all()
                    print("✓ Database tables created successfully!")
                    print("  Tables created: users, tasks")
            else:
                print(f"Database URL: {db_url}")
                db.create_all()
                print("✓ Database tables created")
                
        except Exception as e:
            print(f"✗ Error initializing database: {e}")
            import traceback
            traceback.print_exc()
            sys.exit(1)
    
    print("\nNext steps:")
    print("1. Run the server: python run.py")
    print("2. Register a user using the API client")
    print("\nDatabase initialization complete!")

if __name__ == "__main__":
    main()