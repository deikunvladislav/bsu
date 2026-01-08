```markdown
# Kolokvium 2 - Flask REST API with JWT Authentication & Web Interface

**Belarusian State University, Faculty of Applied Mathematics and Computer Science, Department of Programming Technology**  
**Topic:** "Creation of REST API service with use of Python, Flask, JWT Authentication and CRUD principles"

## 📋 Project Overview

Todo API is a production-ready web application that combines a modern Flask REST API with JWT authentication and a responsive web interface. The system provides comprehensive task management functionality with real-time updates, user authentication, and administrative features. The project demonstrates full-stack web development practices using Flask, SQLAlchemy, JWT, and modern frontend technologies.

## 📁 Project Structure

```bash
kolokvium_2/
├── app/                            # Main application package
│   ├── routes/                     # API route handlers
│   │   ├── auth.py                 # Authentication endpoints (registration, login, profile)
│   │   ├── frontend.py             # Web interface routes
│   │   ├── health.py               # Health check endpoints
│   │   ├── metrics.py              # Prometheus metrics
│   │   └── tasks.py                # Task management endpoints (CRUD operations)
│   ├── static/                     # Static files
│   │   ├── css/                    # Stylesheets
│   │   │   ├── dashboard.css       # Dashboard-specific styles
│   │   │   └── style.css          # Main stylesheet
│   │   └── js/                     # JavaScript files
│   │       ├── app.js             # Core application logic
│   │       ├── auth.js            # Authentication handling
│   │       ├── dashboard.js       # Dashboard functionality
│   │       ├── tasks.js           # Task management logic
│   │       └── toast.js           # Notification system
│   ├── templates/                  # HTML templates (Jinja2)
│   │   ├── about.html             # About page
│   │   ├── api_docs.html          # API documentation
│   │   ├── base.html              # Base template with navigation
│   │   ├── dashboard.html         # User dashboard
│   │   ├── edit_tasks.html        # Task editing interface
│   │   ├── error.html             # Error pages
│   │   ├── index.html             # Landing page
│   │   ├── login.html             # Login form
│   │   ├── profile.html           # User profile
│   │   ├── register.html          # Registration form
│   │   └── tasks.html             # Task management interface
│   ├── __init__.py                # Application initialization
│   ├── config.py                  # Configuration management
│   ├── error_handlers.py          # Global error handling
│   ├── extensions.py              # Flask extensions setup
│   ├── factory.py                 # Application factory pattern
│   ├── logging_config.py          # Structured JSON logging
│   ├── models.py                  # SQLAlchemy models (User, Task)
│   ├── schemas.py                 # Marshmallow schemas
│   ├── utils.py                   # Utility functions
│   └── validators.py              # Input validation
├── migrations/                    # Database migrations
│   └── env.py                     # Alembic environment
├── tests/                         # Comprehensive test suite
│   ├── conftest.py                # Pytest fixtures
│   ├── test_auth.py               # Authentication tests
│   ├── test_cli.py                # CLI command tests
│   ├── test_config.py             # Configuration tests
│   ├── test_error_handlers.py     # Error handler tests
│   ├── test_frontend.py           # Web interface tests
│   ├── test_health.py             # Health endpoint tests
│   ├── test_integration.py        # Integration tests
│   ├── test_metrics.py            # Metrics tests
│   ├── test_models.py             # Model tests
│   ├── test_tasks.py              # Task CRUD tests
│   └── test_utils.py              # Utility function tests
├── .env.example                   # Environment variables template
├── alembic.ini                    # Alembic configuration
├── build_run_test.bat             # Windows build automation script
├── docker-compose.yml             # Docker Compose configuration
├── Dockerfile                     # Docker image definition
├── init_db.py                     # Database initialization script
├── requirements.txt               # Python dependencies
├── run.py                         # Application entry point
├── todo_api_menu.bat              # Legacy console API client
└── README.md                      # This documentation file
```

## 🏗️ Building the Project

### Prerequisites
- **Python 3.11+** with pip package manager
- **Modern web browser** (Chrome, Firefox, Edge, etc.)
- **Git** for version control
- **Docker & Docker Compose** (optional, for containerized deployment)

### Build Instructions

**Quick Start (Windows):**
```bash
# Clone and navigate to project directory
cd kolokvium_2

# Start server (auto-installs dependencies, runs tests)
.\build_run_test.bat

# Access web interface in browser at:
# http://localhost:8000
```

**Manual Installation:**
```bash
# Install Python dependencies
pip install -r requirements.txt

# Initialize database
python init_db.py

# Start development server
python run.py

# Run test suite
python -m pytest tests/ -v
```

**Docker Deployment:**
```bash
# Build and run with Docker Compose
docker-compose up --build

# Access at http://localhost:8000
```

## 🔧 Implementation Details

### 1. Backend Architecture

**Core Components:**
- **Flask Application Factory**: Modular app creation with blueprints
- **SQLAlchemy ORM**: Database abstraction with model relationships
- **JWT Authentication**: Secure token-based authentication with Flask-JWT-Extended
- **RESTful API Design**: Clean endpoints following REST principles
- **Error Handling**: Centralized error handling with custom APIError class
- **Caching**: Redis/SimpleCache integration for performance
- **Logging**: Structured JSON logging for production monitoring

**Database Models:**
```python
# User model for authentication
class User(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(80), unique=True, nullable=False)
    password_hash = db.Column(db.String(200), nullable=False)
    created_at = db.Column(db.DateTime, default=datetime.utcnow)
    updated_at = db.Column(db.DateTime, default=datetime.utcnow, onupdate=datetime.utcnow)
    tasks = db.relationship('Task', backref='user', lazy=True, cascade='all, delete-orphan')

# Task model for task management
class Task(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    title = db.Column(db.String(200), nullable=False)
    description = db.Column(db.Text)
    status = db.Column(db.String(20), default='todo')
    user_id = db.Column(db.Integer, db.ForeignKey('user.id'), nullable=False)
    created_at = db.Column(db.DateTime, default=datetime.utcnow)
    updated_at = db.Column(db.DateTime, default=datetime.utcnow, onupdate=datetime.utcnow)
```

**Task Status System:**
- `todo`: Task pending completion (red indicator)
- `in_progress`: Task actively being worked on (orange indicator)
- `done`: Task completed (green indicator)

### 2. Frontend Architecture

**Core Components:**
- **Responsive Design**: Mobile-friendly interface with CSS Grid/Flexbox
- **JavaScript Modules**: Modular JS with separation of concerns
- **AJAX Communication**: Real-time updates without page reloads
- **Theme System**: Light/dark mode with localStorage persistence
- **Toast Notifications**: Non-intrusive feedback system
- **Modal System**: Reusable modal dialogs for forms

**Key JavaScript Files:**
- **app.js**: Core application logic, API communication, global functions
- **auth.js**: Authentication forms (login/register) with validation
- **dashboard.js**: Dashboard statistics and task overview
- **tasks.js**: Task management with search, filter, pagination
- **toast.js**: Notification system with multiple types (success, error, warning, info)

**Template System:**
- **Base Template**: Common layout with navigation and footer
- **Template Inheritance**: DRY principle with Jinja2 inheritance
- **Context Processing**: Automatic user data injection
- **Conditional Rendering**: User-specific content based on authentication

### 3. API Endpoints

**Authentication (auth.py):**
```
POST   /api/auth/register      - User registration
POST   /api/auth/login         - User login (JWT token)
POST   /api/auth/logout        - User logout
GET    /api/auth/profile       - Get user profile
POST   /api/auth/change-password - Change password
DELETE /api/auth/delete-account  - Delete user account
```

**Task Management (tasks.py):**
```
GET    /api/tasks              - List tasks (with filtering, pagination)
POST   /api/tasks              - Create new task
GET    /api/tasks/<id>         - Get specific task
PUT    /api/tasks/<id>         - Update entire task
PATCH  /api/tasks/<id>         - Partial task update
DELETE /api/tasks/<id>         - Delete task
GET    /api/tasks/stats        - Get task statistics
DELETE /api/tasks/delete-all   - Delete all user tasks
```

**System Monitoring:**
```
GET    /api/health/live        - Liveness probe
GET    /api/health/ready       - Readiness probe
GET    /api/metrics            - Prometheus metrics
```

### 4. Security Features

**Authentication Security:**
- Password hashing with bcrypt and salt
- JWT tokens with 30-minute expiration
- Token refresh mechanism
- Secure session management

**Authorization Controls:**
- User-specific data isolation
- Ownership verification on all operations
- Role-based access (future extensibility)

**Input Validation:**
- Server-side validation for all inputs
- SQL injection prevention via SQLAlchemy
- XSS protection with template escaping
- CSRF protection implementation

## 🌐 Web Interface Features

### 1. User Authentication Interface

**Registration Page:**
- Username validation (3-80 characters, alphanumeric)
- Password strength requirements (minimum 6 characters)
- Real-time feedback with form validation
- Secure password confirmation

**Login Page:**
- Remember me functionality
- Error message display for failed attempts
- Redirect to dashboard on successful login
- Session persistence

### 2. Dashboard

**Real-time Statistics:**
- Task counts by status (todo, in_progress, done)
- Total tasks counter
- Visual status indicators with color coding

**Quick Actions:**
- Create new task button
- View all tasks link
- Refresh dashboard data
- Task creation modal

**Recent Tasks:**
- Last 10 created/updated tasks
- Status badges with color coding
- Task descriptions with truncation
- Creation timestamps

### 3. Task Management Interface

**Task List Features:**
- Grid/List view toggle
- Advanced filtering by status
- Real-time search in titles and descriptions
- Multiple sorting options (date, title, status)
- Pagination with page controls

**Task Cards:**
- Color-coded status borders
- Title and description display
- Creation/update timestamps
- Quick action buttons (edit, delete)
- Hover effects and animations

**Task Creation/Editing:**
- Modal-based forms
- Title and description fields
- Status selection dropdown
- Form validation with error messages
- Success/error notifications

### 4. User Profile

**Profile Information:**
- Username display
- Account creation date
- Task statistics overview

**Account Management:**
- Password change functionality
- Account deletion with confirmation
- Data export (future feature)

### 5. Responsive Design

**Mobile Optimization:**
- Collapsible navigation menu
- Touch-friendly buttons and controls
- Adaptive grid layouts
- Font size scaling

**Theme System:**
- Light/dark mode toggle
- System preference detection
- Theme persistence across sessions
- Smooth transitions

## 🧪 Testing

### Test Structure

**Authentication Tests (test_auth.py):**
```python
def test_register_success(client, db_session):
    # Tests successful user registration
    pass

def test_login_invalid_credentials(client, test_user):
    # Tests login with incorrect credentials
    pass

def test_change_password_success(client, auth_headers):
    # Tests password change functionality
    pass
```

**Task Tests (test_tasks.py):**
```python
def test_create_task_success(client, auth_headers):
    # Tests task creation with valid data
    pass

def test_get_tasks_with_status_filter(client, auth_headers, test_task):
    # Tests task filtering by status
    pass

def test_update_task_full(client, auth_headers, test_task):
    # Tests complete task update
    pass
```

**Integration Tests (test_integration.py):**
```python
def test_full_user_workflow(client, db_session):
    # Tests complete user journey from registration to task management
    pass

def test_user_isolation(client, test_user, second_user):
    # Tests data isolation between users
    pass
```

**Other Test Suites:**
- **test_models.py**: Database model tests
- **test_error_handlers.py**: Error response tests
- **test_health.py**: Health endpoint tests
- **test_frontend.py**: Web interface tests
- **test_utils.py**: Utility function tests

### Running Tests

```bash
# Run all tests with verbose output
pytest tests/ -v

# Run specific test category
pytest tests/test_auth.py -v
pytest tests/test_tasks.py -v
pytest tests/test_integration.py -v

# Run with coverage reporting
pytest tests/ --cov=app --cov-report=html
```

### Test Fixtures

**Database Fixtures:**
- Temporary SQLite database for testing
- Automatic cleanup after tests
- Unique user generation to prevent conflicts

**Authentication Fixtures:**
- Pre-authenticated user tokens
- Authorization headers for API requests
- Multiple user support for isolation tests

## 📖 Usage Example

### Step 1: Start the Application
```bash
python run.py
# Server starts on http://localhost:8000
```

### Step 2: Register New Account
1. Navigate to `http://localhost:8000/register`
2. Fill in registration form:
   - Username: `testuser`
   - Password: `securepassword123`
   - Confirm Password: `securepassword123`
3. Click "Create Account"
4. System redirects to login page

### Step 3: Login to Dashboard
1. Navigate to `http://localhost:8000/login`
2. Enter credentials:
   - Username: `testuser`
   - Password: `securepassword123`
3. Click "Login"
4. Redirected to dashboard with welcome message

### Step 4: Create First Task
1. Click "Create New Task" button on dashboard
2. Fill in task details:
   - Title: "Complete project documentation"
   - Description: "Write README and API documentation"
   - Status: "todo"
3. Click "Create Task"
4. Task appears in recent tasks list

### Step 5: Manage Tasks
1. Click "Tasks" in navigation menu
2. View all tasks in grid layout
3. Use search bar to filter tasks
4. Click filter buttons to show specific statuses
5. Click edit button to modify task
6. Click delete button to remove task

### Step 6: View Statistics
1. Dashboard shows real-time statistics:
   - Total tasks: 5
   - Todo: 2
   - In Progress: 1
   - Done: 2
2. Click refresh button to update statistics

### Step 7: User Profile
1. Click "Profile" in navigation
2. View account information
3. Change password if needed
4. Delete account (with confirmation)

## 🔒 Security Implementation

### Password Security
- **bcrypt hashing**: Password storage with salt
- **Minimum length**: 6 character requirement
- **Validation**: Server-side password validation
- **No plaintext storage**: Hashed passwords only

### JWT Implementation
- **Token expiration**: 30-minute validity period
- **Secure storage**: HTTP-only cookies (optional)
- **Token validation**: Signature verification on each request
- **Claim validation**: User ID and username in token payload

### API Security
- **Endpoint protection**: @jwt_required() decorator
- **Input sanitization**: All user input validated
- **SQL injection prevention**: Parameterized queries
- **Rate limiting**: Basic implementation for login endpoints

### Session Management
- **Secure sessions**: Encrypted session data
- **Automatic logout**: Token expiration
- **Multi-device support**: Independent sessions per device
- **Logout functionality**: Complete session cleanup

## 🎨 Frontend Technologies

### CSS Features
- **CSS Variables**: Theme colors and spacing
- **Flexbox/Grid**: Modern layout techniques
- **Animations**: Smooth transitions and hover effects
- **Media Queries**: Responsive design breakpoints
- **Custom Properties**: Dark/light theme variables

### JavaScript Features
- **ES6 Modules**: Modular code organization
- **Async/Await**: Asynchronous API calls
- **DOM Manipulation**: Dynamic content updates
- **Event Handling**: User interaction management
- **LocalStorage**: Client-side data persistence

### FontAwesome Integration
- **Icon System**: Consistent iconography throughout
- **Status Indicators**: Visual status representation
- **Action Buttons**: Clear visual cues for actions
- **Loading States**: Spinner animations during operations

## 🐳 Docker Deployment

### Docker Configuration
```dockerfile
# Multi-stage build for optimized image size
FROM python:3.11-slim as builder

# Install build dependencies
RUN apt-get update && apt-get install -y \
    gcc \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy requirements and install
COPY requirements.txt .
RUN pip install --user -r requirements.txt

# Final stage
FROM python:3.11-slim

# Create non-root user
RUN useradd -m -u 1000 appuser

WORKDIR /app

# Copy installed packages
COPY --from=builder /root/.local /home/appuser/.local
COPY --chown=appuser:appuser . .

USER appuser

# Health check
HEALTHCHECK --interval=30s --timeout=3s --start-period=5s --retries=3 \
    CMD curl -f http://localhost:8000/api/health/live || exit 1

EXPOSE 8000

CMD ["python", "run.py"]
```

### Docker Compose Setup
```yaml
version: '3.8'
services:
  web:
    build: .
    ports:
      - "8000:8000"
    environment:
      - DATABASE_URL=sqlite:///instance/todo.db
      - SECRET_KEY=${SECRET_KEY}
      - JWT_SECRET_KEY=${JWT_SECRET_KEY}
    volumes:
      - ./instance:/app/instance
    restart: unless-stopped

  # Optional Redis for caching
  redis:
    image: redis:alpine
    restart: unless-stopped
```

### Production Deployment Checklist
- [ ] Set strong SECRET_KEY and JWT_SECRET_KEY
- [ ] Configure production database (PostgreSQL recommended)
- [ ] Enable HTTPS with SSL certificates
- [ ] Set up proper logging and monitoring
- [ ] Configure backup strategy for database
- [ ] Implement rate limiting and DDoS protection
- [ ] Set up CI/CD pipeline for automatic deployments

## 🔮 Future Enhancements

### Planned Features
1. **Real-time Collaboration**: Shared task lists and team functionality
2. **File Attachments**: Upload and manage files with tasks
3. **Task Categories & Tags**: Advanced task organization
4. **Calendar Integration**: Due dates and scheduling
5. **Email Notifications**: Task reminders and updates
6. **Mobile Application**: Native iOS/Android apps
7. **Advanced Analytics**: Task completion trends and productivity metrics
8. **Import/Export**: Data migration from other task managers

### Technical Improvements
1. **WebSocket Support**: Real-time updates across clients
2. **GraphQL API**: Alternative to REST for complex queries
3. **Microservices Architecture**: Split into auth, tasks, notifications services
4. **Advanced Caching**: Redis for session storage and API responses
5. **Load Balancing**: Multiple application instances
6. **CDN Integration**: Static asset delivery optimization

## 🤝 Development Notes

### Code Organization
- **Separation of Concerns**: Clear division between API, frontend, and business logic
- **Modular Design**: Blueprint-based route organization
- **Dependency Injection**: Configurable components for testing
- **Type Hints**: Comprehensive Python type annotations

### Testing Strategy
- **Unit Tests**: Individual component testing
- **Integration Tests**: End-to-end workflow testing
- **Frontend Tests**: JavaScript functionality testing
- **Performance Tests**: API response time monitoring

### Error Handling
- **Consistent Error Responses**: Standardized JSON error format
- **Graceful Degradation**: System remains usable during partial failures
- **User-Friendly Messages**: Clear error messages for end users
- **Detailed Logging**: Comprehensive error logging for developers

### Performance Optimization
- **Database Indexing**: Optimized query performance
- **Lazy Loading**: On-demand resource loading
- **Asset Minification**: Optimized CSS and JavaScript
- **Caching Strategy**: Reduced database load

## 📄 License

*Educational Project* - Belarusian State University, Faculty of Applied Mathematics and Computer Science, Applied Mathematics.