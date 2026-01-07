// Tasks JavaScript

function loadDashboardStats() {
    const token = localStorage.getItem('jwt_token');
    if (!token) return;
    
    fetch('/api/tasks/stats', {
        headers: {
            'Authorization': `Bearer ${token}`
        }
    })
    .then(response => {
        if (!response.ok) throw new Error('Failed to load stats');
        return response.json();
    })
    .then(stats => {
        document.getElementById('todo-count').textContent = stats.todo || 0;
        document.getElementById('progress-count').textContent = stats.in_progress || 0;
        document.getElementById('done-count').textContent = stats.done || 0;
        document.getElementById('total-count').textContent = stats.total || 0;
    })
    .catch(error => {
        console.error('Failed to load dashboard stats:', error);
    });
}

function loadRecentTasks() {
    const token = localStorage.getItem('jwt_token');
    if (!token) return;
    
    fetch('/api/tasks?limit=5&offset=0', {
        headers: {
            'Authorization': `Bearer ${token}`
        }
    })
    .then(response => {
        if (!response.ok) throw new Error('Failed to load tasks');
        return response.json();
    })
    .then(tasks => {
        const container = document.getElementById('recent-tasks-list');
        if (!container) return;
        
        if (tasks.length === 0) {
            container.innerHTML = `
                <div class="empty-state">
                    <i class="fas fa-clipboard-list"></i>
                    <h3>No tasks yet</h3>
                    <p>Create your first task to get started!</p>
                </div>
            `;
            return;
        }
        
        container.innerHTML = tasks.map(task => `
            <div class="task-item task-${task.status}" data-id="${task.id}">
                <div class="task-checkbox">
                    <input type="checkbox" ${task.status === 'done' ? 'checked' : ''} 
                           onchange="toggleTaskStatus(${task.id}, this.checked)">
                </div>
                <div class="task-content">
                    <div class="task-header">
                        <h4 class="task-title">${escapeHtml(task.title)}</h4>
                        <span class="task-status-badge status-${task.status}">
                            ${task.status.replace('_', ' ')}
                        </span>
                    </div>
                    ${task.description ? `<p class="task-description">${escapeHtml(task.description)}</p>` : ''}
                    <div class="task-meta">
                        <span class="task-date">
                            <i class="far fa-calendar"></i> 
                            ${new Date(task.created_at).toLocaleDateString()}
                        </span>
                    </div>
                </div>
            </div>
        `).join('');
    })
    .catch(error => {
        console.error('Failed to load recent tasks:', error);
        const container = document.getElementById('recent-tasks-list');
        if (container) {
            container.innerHTML = `<div class="error">Failed to load tasks</div>`;
        }
    });
}

// Helper function to escape HTML
function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}