from __future__ import annotations
from marshmallow import Schema, fields, validates, ValidationError
from .models import TaskStatus

class UserRegisterSchema(Schema):
    username = fields.Str(required=True, min_length=3, max_length=80)
    password = fields.Str(required=True, min_length=6, max_length=128)
    
    @validates("username")
    def validate_username(self, value):
        if not value.isalnum():
            raise ValidationError("Username must contain only letters and numbers")

class UserLoginSchema(Schema):
    username = fields.Str(required=True)
    password = fields.Str(required=True)

class TaskCreateSchema(Schema):
    title = fields.Str(required=True, min_length=1, max_length=200)
    description = fields.Str(required=False, allow_none=True, default="")
    status = fields.Str(required=True)
    
    @validates("status")
    def validate_status(self, value):
        if value not in TaskStatus.get_all():
            raise ValidationError(f"Status must be one of: {', '.join(TaskStatus.get_all())}")

class TaskUpdateSchema(TaskCreateSchema):
    pass

class TaskPatchSchema(Schema):
    title = fields.Str(required=False, min_length=1, max_length=200)
    description = fields.Str(required=False, allow_none=True)
    status = fields.Str(required=False)
    
    @validates("status")
    def validate_status(self, value):
        if value and value not in TaskStatus.get_all():
            raise ValidationError(f"Status must be one of: {', '.join(TaskStatus.get_all())}")
    
    @validates("title")
    def validate_title(self, value):
        if value and len(value.strip()) == 0:
            raise ValidationError("Title cannot be empty or only whitespace")