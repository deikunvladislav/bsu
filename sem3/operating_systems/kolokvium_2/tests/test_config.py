import os
import pytest

def test_config_defaults():
    try:
        from app.config import Config
        config = Config()
        assert hasattr(config, 'SECRET_KEY')
        assert hasattr(config, 'SQLALCHEMY_DATABASE_URI')
    except ImportError:
        pytest.skip("Config module not available")

def test_development_config():
    try:
        from app.config import DevelopmentConfig
        config = DevelopmentConfig()
        assert config.DEBUG is True
        assert config.TESTING is False
    except ImportError:
        pytest.skip("DevelopmentConfig not available")

def test_testing_config():
    try:
        from app.config import TestingConfig
        config = TestingConfig()
        assert config.TESTING is True
    except ImportError:
        pytest.skip("TestingConfig not available")