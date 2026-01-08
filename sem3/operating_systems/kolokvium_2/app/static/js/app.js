document.addEventListener('DOMContentLoaded', function() {
    console.log('App initialized');
    
    const token = localStorage.getItem('jwt_token');
    const currentUser = localStorage.getItem('current_user');
    
    updateAuthUI(!!token);
    
    setupGlobalListeners();
});

function updateAuthUI(isAuthenticated) {
    console.log('Auth state:', isAuthenticated ? 'Authenticated' : 'Not authenticated');
}

function setupGlobalListeners() {
    const logoutBtn = document.getElementById('logout-btn');
    if (logoutBtn) {
        logoutBtn.addEventListener('click', function(e) {
            e.preventDefault();
            handleLogout();
        });
    }
    
    const apiStatusElement = document.getElementById('api-status');
    if (apiStatusElement) {
        checkApiStatus();
        setInterval(checkApiStatus, 30000);
    }
}

async function checkApiStatus() {
    try {
        const response = await fetch('/api/health/live');
        const statusElement = document.getElementById('api-status');
        
        if (response.ok) {
            statusElement.textContent = 'Online';
            statusElement.className = 'api-status';
        } else {
            statusElement.textContent = 'Offline';
            statusElement.className = 'api-status offline';
        }
    } catch (error) {
        const statusElement = document.getElementById('api-status');
        statusElement.textContent = 'Offline';
        statusElement.className = 'api-status offline';
    }
}

async function handleLogout() {
    const token = localStorage.getItem('jwt_token');
    
    try {
        if (token) {
            await fetch('/api/auth/logout', {
                method: 'POST',
                headers: {
                    'Authorization': `Bearer ${token}`,
                    'Content-Type': 'application/json'
                }
            });
        }
    } catch (error) {
        console.error('Logout API error:', error);
    } finally {
        localStorage.removeItem('jwt_token');
        localStorage.removeItem('current_user');
        
        window.location.href = '/logout';
    }
}

function getAuthToken() {
    return localStorage.getItem('jwt_token');
}

function getAuthHeaders() {
    const token = getAuthToken();
    const headers = {
        'Content-Type': 'application/json'
    };
    
    if (token) {
        headers['Authorization'] = `Bearer ${token}`;
    }
    
    return headers;
}

async function handleApiResponse(response) {
    if (!response.ok) {
        const error = await response.json().catch(() => ({
            message: 'Server error'
        }));
        
        if (response.status === 401) {
            localStorage.removeItem('jwt_token');
            localStorage.removeItem('current_user');
            window.location.href = '/login';
            return null;
        }
        
        throw new Error(error.message || `API error: ${response.status}`);
    }
    
    return await response.json();
}

function showNotification(message, type = 'info') {
    const notification = document.createElement('div');
    notification.className = `flash-message ${type}`;
    notification.textContent = message;
    
    let container = document.querySelector('.flash-messages');
    if (!container) {
        container = document.createElement('div');
        container.className = 'flash-messages';
        document.body.appendChild(container);
    }
    
    container.appendChild(notification);
    
    setTimeout(() => {
        notification.style.animation = 'slideOutRight 0.3s ease forwards';
        setTimeout(() => notification.remove(), 300);
    }, 5000);
}

window.createTask = async function(taskData) {
    const token = getAuthToken();
    
    if (!token) {
        showNotification('Please login to create tasks', 'error');
        window.location.href = '/login';
        return null;
    }
    
    try {
        const response = await fetch('/api/tasks', {
            method: 'POST',
            headers: getAuthHeaders(),
            body: JSON.stringify(taskData)
        });
        
        const result = await handleApiResponse(response);
        
        if (result) {
            showNotification('Task created successfully!', 'success');
            return result;
        }
    } catch (error) {
        showNotification(error.message || 'Failed to create task', 'error');
        console.error('Create task error:', error);
    }
    
    return null;
};

window.updateTask = async function(taskId, taskData) {
    const token = getAuthToken();
    
    if (!token) {
        showNotification('Please login to update tasks', 'error');
        window.location.href = '/login';
        return null;
    }
    
    try {
        const response = await fetch(`/api/tasks/${taskId}`, {
            method: 'PUT',
            headers: getAuthHeaders(),
            body: JSON.stringify(taskData)
        });
        
        const result = await handleApiResponse(response);
        
        if (result) {
            showNotification('Task updated successfully!', 'success');
            return result;
        }
    } catch (error) {
        showNotification(error.message || 'Failed to update task', 'error');
        console.error('Update task error:', error);
    }
    
    return null;
};

window.deleteTask = async function(taskId) {
    if (!confirm('Are you sure you want to delete this task?')) {
        return false;
    }
    
    const token = getAuthToken();
    
    if (!token) {
        showNotification('Please login to delete tasks', 'error');
        window.location.href = '/login';
        return false;
    }
    
    try {
        const response = await fetch(`/api/tasks/${taskId}`, {
            method: 'DELETE',
            headers: getAuthHeaders()
        });
        
        if (response.ok) {
            showNotification('Task deleted successfully!', 'success');
            return true;
        } else {
            const error = await handleApiResponse(response);
            throw new Error(error?.message || 'Failed to delete task');
        }
    } catch (error) {
        showNotification(error.message || 'Failed to delete task', 'error');
        console.error('Delete task error:', error);
        return false;
    }
};

window.App = {
    getAuthToken,
    getAuthHeaders,
    handleApiResponse,
    showNotification,
    createTask: window.createTask,
    updateTask: window.updateTask,
    deleteTask: window.deleteTask
};