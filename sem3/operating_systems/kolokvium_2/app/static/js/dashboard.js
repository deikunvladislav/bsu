async function handleTaskSubmit(e) {
    e.preventDefault();
    
    const title = document.getElementById('task-title').value.trim();
    const description = document.getElementById('task-description').value.trim();
    const status = document.getElementById('task-status').value;
    
    if (!title) {
        showNotification('Please enter a task title', 'error');
        return;
    }
    
    const token = localStorage.getItem('jwt_token');
    
    if (!token) {
        showNotification('You are not logged in. Please login again.', 'error');
        window.location.href = '/login';
        return;
    }
    
    const submitBtn = e.target.querySelector('.btn-submit');
    const originalText = submitBtn.innerHTML;
    submitBtn.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Creating...';
    submitBtn.disabled = true;
    
    try {
        const response = await fetch('/api/tasks', {
            method: 'POST',
            headers: {
                'Authorization': `Bearer ${token}`,
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                title: title,
                description: description,
                status: status
            })
        });
        
        const data = await response.json();
        
        if (response.ok) {
            showNotification('Task created successfully!', 'success');
            
            const modal = document.getElementById('task-modal');
            const taskForm = document.getElementById('task-form');
            
            if (modal) modal.style.display = 'none';
            if (taskForm) taskForm.reset();
            
            loadDashboardData();
        } else {
            const errorMsg = data.message || 'Failed to create task';
            const errorCode = data.code || 'UNKNOWN_ERROR';
            
            if (errorCode === 'INVALID_STATUS') {
                showNotification(`Invalid status. Valid values are: todo, in_progress, done`, 'error');
            } else if (errorCode === 'INVALID_TITLE') {
                showNotification('Task title cannot be empty', 'error');
            } else {
                showNotification(errorMsg, 'error');
            }
            
            console.error('Task creation error:', errorCode, errorMsg);
        }
    } catch (error) {
        console.error('Network error creating task:', error);
        showNotification('Network error. Please check your connection and try again.', 'error');
    } finally {
        submitBtn.innerHTML = originalText;
        submitBtn.disabled = false;
    }
}