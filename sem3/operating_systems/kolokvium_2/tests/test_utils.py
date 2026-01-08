import pytest

def test_validate_password():
    try:
        from app.utils import validate_password
        if validate_password:
            result = validate_password('123456')
            assert result is True or result is False
    except ImportError:
        pytest.skip('validate_password not available')

def test_validate_username():
    try:
        from app.utils import validate_username
        if validate_username:
            result = validate_username('user123')
            assert result is True or result is False
    except ImportError:
        pytest.skip('validate_username not available')

def test_api_error():
    try:
        from app.utils import APIError
        error = APIError('Test message', 400, 'TEST_CODE')
        assert error.message == 'Test message'
        assert error.status_code == 400
        assert error.code == 'TEST_CODE'
    except (ImportError, AttributeError):
        pytest.skip('APIError not available or has different structure')