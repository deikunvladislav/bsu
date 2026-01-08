import json

def test_liveness(client):
    response = client.get('/api/health/live')
    assert response.status_code == 200
    data = response.get_json()
    assert 'status' in data
    assert data['status'] in ['alive', 'ok', 'healthy']

def test_readiness_success(client):
    response = client.get('/api/health/ready')
    assert response.status_code in [200, 503]
    
    if response.get_json():
        data = response.get_json()
        assert 'status' in data

def test_health_endpoints_not_allowed_methods(client):
    response = client.post('/api/health/live')
    assert response.status_code == 405
    
    response = client.put('/api/health/ready')
    assert response.status_code == 405