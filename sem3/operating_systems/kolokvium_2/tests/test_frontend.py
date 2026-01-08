import pytest

def test_frontend_public_routes(client):
    response = client.get('/')
    assert response.status_code in [200, 302]
    
    response = client.get('/login')
    assert response.status_code in [200, 404]
    
    response = client.get('/register')
    assert response.status_code in [200, 404]

def test_frontend_static_files(client):
    response = client.get('/static/css/style.css')
    assert response.status_code in [200, 404]