from flask import request
from flask_sqlalchemy import SQLAlchemy
from flask_jwt_extended import JWTManager
from flask_caching import Cache
from prometheus_client import Counter, Histogram
import time

db = SQLAlchemy()
jwt = JWTManager()
cache = Cache()

HTTP_REQUESTS_TOTAL = Counter(
    "http_requests_total",
    "Total HTTP requests",
    ["method", "endpoint", "status"],
)

HTTP_REQUEST_DURATION_SECONDS = Histogram(
    "http_request_duration_seconds",
    "HTTP request duration in seconds",
    ["endpoint", "method"],
    buckets=(0.01, 0.025, 0.05, 0.1, 0.25, 0.5, 1.0, 2.5, 5.0, 10.0)
)

TASKS_CREATED_TOTAL = Counter(
    "tasks_created_total",
    "Total tasks created",
    ["user_id"]
)

TASKS_UPDATED_TOTAL = Counter(
    "tasks_updated_total",
    "Total tasks updated",
    ["user_id", "status"]
)

USERS_REGISTERED_TOTAL = Counter(
    "users_registered_total",
    "Total users registered"
)

def setup_metrics_middleware(app):
    @app.before_request
    def before_request():
        request.start_time = time.time()
    
    @app.after_request
    def after_request(response):
        if hasattr(request, 'start_time'):
            duration = time.time() - request.start_time
            
            try:
                HTTP_REQUESTS_TOTAL.labels(
                    method=request.method,
                    endpoint=request.endpoint or 'unknown',
                    status=response.status_code
                ).inc()
                
                HTTP_REQUEST_DURATION_SECONDS.labels(
                    endpoint=request.endpoint or 'unknown',
                    method=request.method
                ).observe(duration)
            except:
                pass 
        
        return response