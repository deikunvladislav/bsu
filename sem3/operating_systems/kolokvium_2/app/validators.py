from __future__ import annotations
from .models import TaskStatus
from .utils import APIError

def validate_task_status(status_str: str) -> TaskStatus:
    try:
        return TaskStatus(status_str)
    except ValueError:
        valid_statuses = ', '.join(TaskStatus.get_all())
        raise APIError(
            f"Invalid status. Must be one of: {valid_statuses}",
            400,
            "INVALID_STATUS"
        )

def validate_task_data(data: dict, require_all: bool = False) -> tuple:
    errors = []
    
    if require_all:
        required_fields = ['title', 'status']
        for field in required_fields:
            if field not in data:
                errors.append(f"Missing required field: {field}")
    
    if 'title' in data and data['title'] is not None:
        if not isinstance(data['title'], str) or len(data['title'].strip()) == 0:
            errors.append("Title must be a non-empty string")
    
    if 'status' in data and data['status'] is not None:
        try:
            validate_task_status(data['status'])
        except APIError as e:
            errors.append(str(e.message))
    
    if errors:
        raise APIError(
            "Task validation failed",
            400,
            "VALIDATION_ERROR",
            errors=errors
        )
    
    return data.get('title', '').strip() if 'title' in data else None, \
           data.get('description', ''), \
           data.get('status')

def validate_username_format(username: str) -> bool:
    return username.isalnum()