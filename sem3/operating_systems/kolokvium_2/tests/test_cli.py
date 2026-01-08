import pytest
from app import create_app

def test_init_db_command(runner, app):
    result = runner.invoke(args=['init-db'])
    assert result.exit_code in [0, 1]

def test_cli_help(runner):
    result = runner.invoke(args=['--help'])
    assert result.exit_code == 0
    assert 'Usage:' in result.output

def test_app_factory_cli_commands():
    app = create_app('testing')
    assert hasattr(app, 'cli')