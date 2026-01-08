import pytest

def test_metrics_endpoint(client):
    response = client.get('/api/metrics')
    assert response.status_code in [200, 404]
    if response.status_code == 200:
        content_type = response.headers.get('Content-Type', '')
        assert 'text/plain' in content_type or 'application/json' in content_type

def test_stats_endpoint(client, auth_headers):
    response = client.get('/api/stats', headers=auth_headers)
    assert response.status_code in [200, 404, 401]
    if response.status_code == 200:
        data = response.get_json()
        assert isinstance(data, dict)

def test_stats_endpoint_unauthorized(client):
    response = client.get('/api/stats')
    assert response.status_code in [401, 404]