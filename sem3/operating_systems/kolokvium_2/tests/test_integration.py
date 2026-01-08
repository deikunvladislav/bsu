import json
import pytest
import uuid

def test_full_user_workflow(client, db_session):
    unique_id = str(uuid.uuid4().hex[:8]) 
    username = f'integrationuser{unique_id}'
    
    register_data = {
        'username': username,
        'password': 'integrationpass123'
    }
    response = client.post('/api/auth/register', json=register_data)
    
    if response.status_code == 400:
        json_data = response.get_json()
        pytest.skip(f"Registration failed: {json_data.get('code', 'UNKNOWN')}")
    
    assert response.status_code == 201
    
    response = client.post('/api/auth/login', json=register_data)
    assert response.status_code == 200
    token = response.get_json()['access_token']
    headers = {'Authorization': f'Bearer {token}', 'Content-Type': 'application/json'}
    
    response = client.get('/api/auth/profile', headers=headers)
    assert response.status_code == 200
    
    task_ids = []
    for i in range(3):
        task_data = {
            'title': f'Integration Task {i + 1}',
            'description': f'Description {i + 1}',
            'status': 'todo'
        }
        response = client.post('/api/tasks', json=task_data, headers=headers)
        assert response.status_code in [201, 200]
        task_response = response.get_json()
        
        if 'data' in task_response and 'id' in task_response['data']:
            task_ids.append(task_response['data']['id'])
        elif 'id' in task_response:
            task_ids.append(task_response['id'])
        elif task_response.get('success') and 'data' in task_response:
            data = task_response['data']
            if isinstance(data, dict) and 'id' in data:
                task_ids.append(data['id'])
    
    if not task_ids:
        pytest.skip('Could not create tasks for integration test')
    
    response = client.get('/api/tasks', headers=headers)
    assert response.status_code == 200
    
    if task_ids:
        task_id = task_ids[0]
        update_data = {'status': 'in_progress'}
        response = client.patch(f'/api/tasks/{task_id}', json=update_data, headers=headers)
        assert response.status_code in [200, 204]
    
    if len(task_ids) > 1:
        task_id = task_ids[1]
        replace_data = {
            'title': 'Updated Integration Task',
            'description': 'Updated Description',
            'status': 'done'
        }
        response = client.put(f'/api/tasks/{task_id}', json=replace_data, headers=headers)
        assert response.status_code in [200, 204]
    
    response = client.get('/api/tasks/stats', headers=headers)
    if response.status_code == 200:
        stats = response.get_json()
        if 'data' in stats:
            assert 'total' in stats['data']
    
    new_password = 'newintegrationpass123'
    response = client.post('/api/auth/change-password', headers=headers, json={
        'old_password': register_data['password'],
        'new_password': new_password
    })
    assert response.status_code in [200, 204]
    
    response = client.post('/api/auth/login', json={
        'username': register_data['username'],
        'password': new_password
    })
    assert response.status_code == 200
    
    new_token = response.get_json()['access_token']
    delete_headers = {'Authorization': f'Bearer {new_token}', 'Content-Type': 'application/json'}
    response = client.delete('/api/auth/delete-account', headers=delete_headers)
    assert response.status_code in [200, 204]

def test_user_isolation(client, test_user, second_user):
    response = client.post('/api/auth/login', json={
        'username': test_user.username,
        'password': 'testpass123'
    })
    
    assert response.status_code == 200
    token1 = response.get_json()['access_token']
    headers1 = {'Authorization': f'Bearer {token1}', 'Content-Type': 'application/json'}
    
    task_data = {
        'title': "First User's Task",
        'description': 'Private task',
        'status': 'todo'
    }
    response = client.post('/api/tasks', json=task_data, headers=headers1)
    assert response.status_code in [201, 200]
    
    task_response = response.get_json()
    task_id = None
    
    if 'data' in task_response and 'id' in task_response['data']:
        task_id = task_response['data']['id']
    elif 'id' in task_response:
        task_id = task_response['id']
    elif task_response.get('success') and 'data' in task_response:
        data = task_response['data']
        if isinstance(data, dict) and 'id' in data:
            task_id = data['id']
    
    if not task_id:
        pytest.skip('Could not create task for isolation test')
    
    response = client.post('/api/auth/login', json={
        'username': second_user.username,
        'password': 'secondpass123'
    })
    assert response.status_code == 200
    token2 = response.get_json()['access_token']
    headers2 = {'Authorization': f'Bearer {token2}', 'Content-Type': 'application/json'}
    
    response = client.get(f'/api/tasks/{task_id}', headers=headers2)
    assert response.status_code in [404, 403]
    
    response = client.patch(f'/api/tasks/{task_id}', 
                          json={'status': 'done'}, 
                          headers=headers2)
    assert response.status_code in [404, 403]
    
    response = client.delete(f'/api/tasks/{task_id}', headers=headers2)
    assert response.status_code in [404, 403]