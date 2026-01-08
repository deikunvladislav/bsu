import json

def test_404_handler(client):
    response = client.get('/nonexistent-endpoint')
    assert response.status_code == 404
    if response.get_json():
        json_data = response.get_json()
        assert 'error' in json_data or 'code' in json_data

def test_405_handler(client):
    response = client.put('/api/health/live')
    assert response.status_code == 405
    if response.get_json():
        json_data = response.get_json()
        assert 'error' in json_data or 'code' in json_data

def test_400_handler(client):
    response = client.post('/api/auth/login',
                          data='invalid json',
                          content_type='application/json')
    assert response.status_code in [400, 415, 422]