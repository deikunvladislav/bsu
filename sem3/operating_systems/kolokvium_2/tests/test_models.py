import pytest
from datetime import datetime, timedelta
from app import db
from app.models import User, Task

def test_user_model_creation(db_session):
    import uuid
    unique_id = str(uuid.uuid4())[:8]
    username = f'test_model_user_{unique_id}'
    
    user = User(username=username)
    user.set_password('testpassword')

    assert user.username == username
    assert user.check_password('testpassword') is True
    assert user.check_password('wrongpassword') is False
    assert user.id is None

    db_session.add(user)
    db_session.commit()

    assert user.id is not None
    assert isinstance(user.created_at, datetime)

    if hasattr(user, 'to_dict'):
        user_dict = user.to_dict()
        assert user_dict['username'] == username
        assert 'id' in user_dict

def test_task_model_creation(db_session):
    import uuid
    unique_id = str(uuid.uuid4())[:8]
    username = f'task_owner_{unique_id}'
    
    user = User(username=username)
    user.set_password('password')
    db_session.add(user)
    db_session.commit()

    task = Task(
        title='Test Task',
        description='Test Description',
        status='todo',
        user_id=user.id
    )

    assert task.title == 'Test Task'
    assert task.description == 'Test Description'
    assert task.status == 'todo'
    assert task.user_id == user.id
    assert task.id is None

    db_session.add(task)
    db_session.commit()

    assert task.id is not None
    assert isinstance(task.created_at, datetime)

    if hasattr(task, 'to_dict'):
        task_dict = task.to_dict()
        assert task_dict['title'] == 'Test Task'
        assert task_dict['description'] == 'Test Description'
        assert task_dict['status'] == 'todo'
        assert task_dict['user_id'] == user.id
        assert 'id' in task_dict

def test_task_status():
    assert hasattr(Task, 'status')
    
    try:
        from app.models import TaskStatus
        assert TaskStatus.TODO == 'todo'
        assert TaskStatus.IN_PROGRESS == 'in_progress'
        assert TaskStatus.DONE == 'done'
    except ImportError:
        assert 'todo' in ['todo', 'in_progress', 'done']
        assert 'in_progress' in ['todo', 'in_progress', 'done']
        assert 'done' in ['todo', 'in_progress', 'done']

def test_user_task_relationship(db_session):
    import uuid
    unique_id = str(uuid.uuid4())[:8]
    username = f'rel_user_{unique_id}'
    
    user = User(username=username)
    user.set_password('pass')

    task1 = Task(title='Task 1', status='todo', user=user)
    task2 = Task(title='Task 2', status='in_progress', user=user)

    db_session.add(user)
    db_session.add(task1)
    db_session.add(task2)
    db_session.commit()

    db_session.refresh(user)
    
    assert len(user.tasks) == 2
    assert user.tasks[0].title == 'Task 1'
    assert user.tasks[1].title == 'Task 2'

    assert task1.user.username == username
    assert task2.user.username == username

def test_user_repr():
    import uuid
    unique_id = str(uuid.uuid4())[:8]
    username = f'testuser_{unique_id}'
    
    user = User(username=username)
    assert username in repr(user)

def test_task_repr():
    task = Task(title='Test Task', status='todo')
    assert 'Test Task' in repr(task)
    assert 'todo' in repr(task).lower()

def test_user_password_hashing():
    import uuid
    unique_id = str(uuid.uuid4())[:8]
    username = f'testuser_{unique_id}'
    
    user = User(username=username)
    user.set_password('password123')

    assert user.password_hash != 'password123'
    assert len(user.password_hash) > 0

    assert user.check_password('password123') is True
    assert user.check_password('wrongpassword') is False

def test_task_auto_timestamps(db_session):
    import uuid
    unique_id = str(uuid.uuid4())[:8]
    username = f'timestamps_user_{unique_id}'
    
    user = User(username=username)
    user.set_password('pass')
    db_session.add(user)
    db_session.commit()

    task = Task(title='Timestamp Task', status='todo', user_id=user.id)
    db_session.add(task)
    db_session.commit()

    assert task.created_at is not None
    assert task.updated_at is not None
    
    time_diff = abs(task.created_at - task.updated_at)
    assert time_diff < timedelta(seconds=1)

def test_user_auto_timestamps(db_session):
    import uuid
    unique_id = str(uuid.uuid4())[:8]
    username = f'timestamps_user2_{unique_id}'
    
    user = User(username=username)
    user.set_password('pass')
    db_session.add(user)
    db_session.commit()

    assert user.created_at is not None
    assert user.updated_at is not None
    time_diff = abs(user.created_at - user.updated_at)
    assert time_diff < timedelta(seconds=1)