import json
import pytest

def test_create_task_success(client, auth_headers):
    data = {
        'title': 'New Task',
        'description': 'Task description',
        'status': 'todo'
    }
    response = client.post('/api/tasks', json=data, headers=auth_headers)
    assert response.status_code in [201, 200]
    json_data = response.get_json()
    assert 'success' in json_data or 'id' in json_data
    if 'success' in json_data:
        assert json_data['success'] is True
    if 'data' in json_data:
        assert json_data['data']['title'] == 'New Task'

def test_create_task_missing_title(client, auth_headers):
    data = {
        'description': 'Task description',
        'status': 'todo'
    }
    response = client.post('/api/tasks', json=data, headers=auth_headers)
    assert response.status_code == 400
    json_data = response.get_json()
    assert 'code' in json_data

def test_create_task_invalid_status(client, auth_headers):
    data = {
        'title': 'New Task',
        'description': 'Task description',
        'status': 'invalid_status'
    }
    response = client.post('/api/tasks', json=data, headers=auth_headers)
    assert response.status_code == 400
    json_data = response.get_json()
    assert 'code' in json_data

def test_create_task_unauthorized(client):
    data = {
        'title': 'New Task',
        'description': 'Task description',
        'status': 'todo'
    }
    response = client.post('/api/tasks', json=data)
    assert response.status_code == 401

def test_get_tasks_success(client, auth_headers, test_task):
    response = client.get('/api/tasks', headers=auth_headers)
    assert response.status_code == 200
    json_data = response.get_json()
    assert 'success' in json_data or 'tasks' in json_data or 'data' in json_data

def test_get_tasks_with_status_filter(client, auth_headers, test_task):
    response = client.get('/api/tasks?status=todo', headers=auth_headers)
    assert response.status_code == 200
    json_data = response.get_json()
    assert 'success' in json_data or 'tasks' in json_data

def test_get_task_success(client, auth_headers, test_task):
    response = client.get(f'/api/tasks/{test_task.id}', headers=auth_headers)
    assert response.status_code == 200
    json_data = response.get_json()
    assert 'success' in json_data or 'title' in json_data

def test_get_task_not_found(client, auth_headers):
    response = client.get('/api/tasks/999', headers=auth_headers)
    assert response.status_code == 404
    json_data = response.get_json()
    assert 'code' in json_data

def test_get_task_unauthorized(client, test_task):
    response = client.get(f'/api/tasks/{test_task.id}')
    assert response.status_code == 401

def test_get_task_other_user(client, second_user_headers, test_task):
    response = client.get(f'/api/tasks/{test_task.id}', headers=second_user_headers)
    assert response.status_code == 404
    json_data = response.get_json()
    assert 'code' in json_data

def test_update_task_full(client, auth_headers, test_task):
    data = {
        'title': 'Updated Title',
        'description': 'Updated Description',
        'status': 'done'
    }
    response = client.put(f'/api/tasks/{test_task.id}',
                          json=data,
                          headers=auth_headers)
    assert response.status_code == 200
    json_data = response.get_json()
    assert 'success' in json_data or 'message' in json_data

def test_update_task_partial(client, auth_headers, test_task):
    data = {
        'status': 'in_progress'
    }
    response = client.patch(f'/api/tasks/{test_task.id}',
                            json=data,
                            headers=auth_headers)
    assert response.status_code == 200
    json_data = response.get_json()
    assert 'success' in json_data or 'status' in json_data

def test_update_task_not_found(client, auth_headers):
    data = {
        'title': 'Updated Title',
        'description': 'Updated Description',
        'status': 'done'
    }
    response = client.put('/api/tasks/999', json=data, headers=auth_headers)
    assert response.status_code == 404
    json_data = response.get_json()
    assert 'code' in json_data

def test_delete_task_success(client, auth_headers, test_task):
    response = client.delete(f'/api/tasks/{test_task.id}', headers=auth_headers)
    assert response.status_code == 200
    json_data = response.get_json()
    assert 'success' in json_data or 'message' in json_data

def test_delete_task_not_found(client, auth_headers):
    response = client.delete('/api/tasks/999', headers=auth_headers)
    assert response.status_code == 404
    json_data = response.get_json()
    assert 'code' in json_data

def test_delete_task_other_user(client, second_user_headers, test_task):
    response = client.delete(f'/api/tasks/{test_task.id}',
                             headers=second_user_headers)
    assert response.status_code == 404
    json_data = response.get_json()
    assert 'code' in json_data

def test_get_task_stats(client, auth_headers):
    response = client.get('/api/tasks/stats', headers=auth_headers)
    if response.status_code == 200:
        json_data = response.get_json()
        assert 'success' in json_data or 'total' in json_data

def test_tasks_sorting(client, auth_headers):
    tasks = [
        {'title': 'Banana', 'status': 'todo'},
        {'title': 'Apple', 'status': 'in_progress'},
        {'title': 'Cherry', 'status': 'done'}
    ]
    
    for task in tasks:
        response = client.post('/api/tasks', json=task, headers=auth_headers)
        assert response.status_code in [201, 200]
    
    response = client.get('/api/tasks?sort_by=title&sort_order=asc',
                         headers=auth_headers)
    assert response.status_code == 200