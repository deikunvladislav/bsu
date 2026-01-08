from __future__ import annotations
import os
from dataclasses import dataclass, field
from datetime import timedelta

@dataclass(frozen=True)
class Config:
    SECRET_KEY: str = os.getenv("SECRET_KEY", "dev-secret-change-me-in-production")
    SESSION_COOKIE_NAME: str = "todo_session"
    SESSION_COOKIE_SECURE: bool = False
    SESSION_COOKIE_HTTPONLY: bool = True
    SESSION_COOKIE_SAMESITE: str = "Lax"
    
    SQLALCHEMY_DATABASE_URI: str = os.getenv("DATABASE_URL", "sqlite:///todo.db")
    SQLALCHEMY_TRACK_MODIFICATIONS: bool = False
    
    JWT_SECRET_KEY: str = os.getenv("JWT_SECRET_KEY", "jwt-secret-change-me-in-production")
    JWT_ACCESS_TOKEN_EXPIRES: timedelta = timedelta(minutes=30)
    JWT_TOKEN_LOCATION: list = field(default_factory=lambda: ["headers"])
    JWT_HEADER_NAME: str = "Authorization"
    JWT_HEADER_TYPE: str = "Bearer"
    JWT_COOKIE_SECURE: bool = False
    JWT_COOKIE_CSRF_PROTECT: bool = False
    JWT_COOKIE_SAMESITE: str = "Lax"
    
    LOG_LEVEL: str = os.getenv("LOG_LEVEL", "INFO")
    
    CACHE_TYPE: str = "SimpleCache"
    CACHE_DEFAULT_TIMEOUT: int = 300
    
    BCRYPT_LOG_ROUNDS: int = 12
    
    API_TITLE: str = "Todo API"
    API_VERSION: str = "1.0.0"
    OPENAPI_VERSION: str = "3.0.2"

@dataclass(frozen=True)
class DevelopmentConfig(Config):
    DEBUG: bool = True
    TESTING: bool = False
    LOG_LEVEL: str = "DEBUG"
    CACHE_TYPE: str = "SimpleCache"
    JWT_TOKEN_LOCATION: list = field(default_factory=lambda: ["headers"])
    JWT_COOKIE_SECURE: bool = False

@dataclass(frozen=True)
class ProductionConfig(Config):
    DEBUG: bool = False
    TESTING: bool = False
    LOG_LEVEL: str = "WARNING"
    CACHE_TYPE: str = "RedisCache"
    CACHE_REDIS_URL: str = os.getenv("REDIS_URL", "redis://localhost:6379/0")
    SESSION_COOKIE_SECURE: bool = True
    JWT_COOKIE_SECURE: bool = True

@dataclass(frozen=True)
class TestingConfig(Config):
    DEBUG: bool = False
    TESTING: bool = True
    SQLALCHEMY_DATABASE_URI: str = "sqlite:///:memory:"
    CACHE_TYPE: str = "NullCache"
    JWT_ACCESS_TOKEN_EXPIRES: timedelta = timedelta(seconds=5)