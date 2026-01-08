import pytest
import tempfile
import os
import sys
from pathlib import Path
import uuid

project_root = Path(__file__).parent.parent
sys.path.insert(0, str(project_root))

from app import create_app, db
from app.models import User, Task
from flask_jwt_extended import create_access_token

@pytest.fixture(scope='session')
def app():
    db_fd, db_path = tempfile.mkstemp()
    
    app = create_app('testing')
    
    app.config.update({
        'SQLALCHEMY_DATABASE_URI': f'sqlite:///{db_path}',
        'WTF_CSRF_ENABLED': False,
        'TESTING': True,
        'SECRET_KEY': 'test-secret-key',
        'JWT_SECRET_KEY': 'test-jwt-secret-key',
    })
    
    with app.app_context():
        db.create_all()
    
    yield app
    
    os.close(db_fd)
    os.unlink(db_path)

@pytest.fixture
def client(app):
    return app.test_client()

@pytest.fixture
def runner(app):
    return app.test_cli_runner()

@pytest.fixture
def db_session(app):
    with app.app_context():
        db.drop_all()
        db.create_all()
        
        db.session.begin_nested()
        
        yield db.session
        
        db.session.rollback()

@pytest.fixture
def test_user(db_session):
    unique_id = str(uuid.uuid4().hex)[:8] 
    username = f'testuser{unique_id}'  
    
    user = User(username=username)
    user.set_password('testpass123')
    db_session.add(user)
    db_session.commit()
    return user

@pytest.fixture
def test_task(db_session, test_user):
    task = Task(
        title='Test Task',
        description='Test Description',
        status='todo',
        user_id=test_user.id
    )
    db_session.add(task)
    db_session.commit()
    return task

@pytest.fixture
def auth_token(test_user, app):
    with app.app_context():
        token = create_access_token(identity=str(test_user.id))
        return token

@pytest.fixture
def auth_headers(auth_token):
    return {
        'Authorization': f'Bearer {auth_token}',
        'Content-Type': 'application/json'
    }

@pytest.fixture
def second_user(db_session):
    unique_id = str(uuid.uuid4().hex)[:8]
    username = f'seconduser{unique_id}'
    
    user = User(username=username)
    user.set_password('secondpass123')
    db_session.add(user)
    db_session.commit()
    return user

@pytest.fixture
def second_user_token(second_user, app):
    with app.app_context():
        token = create_access_token(identity=str(second_user.id))
        return token

@pytest.fixture
def second_user_headers(second_user_token):
    return {
        'Authorization': f'Bearer {second_user_token}',
        'Content-Type': 'application/json'
    }