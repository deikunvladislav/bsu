// Global toast notification system
window.showToast = function(message, type = 'info', title = '') {
    // Create toast container if not exists
    let container = document.getElementById('toast-container');
    if (!container) {
        container = document.createElement('div');
        container.className = 'toast-container';
        container.id = 'toast-container';
        document.body.appendChild(container);
    }
    
    const toast = document.createElement('div');
    toast.className = `toast ${type}`;
    
    // Set icon based on type
    let icon = 'fas fa-info-circle';
    switch(type) {
        case 'success': icon = 'fas fa-check-circle'; break;
        case 'error': icon = 'fas fa-exclamation-circle'; break;
        case 'warning': icon = 'fas fa-exclamation-triangle'; break;
        case 'info': 
        default: 
            icon = 'fas fa-info-circle'; 
            break;
    }
    
    // Create toast HTML
    toast.innerHTML = `
        <div class="toast-icon">
            <i class="${icon}"></i>
        </div>
        <div class="toast-content">
            ${title ? `<div class="toast-title">${title}</div>` : ''}
            <div class="toast-message">${message}</div>
        </div>
        <button class="toast-close">
            <i class="fas fa-times"></i>
        </button>
    `;
    
    // Add close event
    const closeBtn = toast.querySelector('.toast-close');
    closeBtn.addEventListener('click', () => {
        toast.style.animation = 'slideOutRight 0.3s ease forwards';
        setTimeout(() => toast.remove(), 300);
    });
    
    container.appendChild(toast);
    
    // Auto remove after 5 seconds
    setTimeout(() => {
        if (toast.parentNode) {
            toast.style.animation = 'slideOutRight 0.3s ease forwards';
            setTimeout(() => toast.remove(), 300);
        }
    }, 5000);
};

// Add styles if not already present
if (!document.getElementById('toast-styles')) {
    const style = document.createElement('style');
    style.id = 'toast-styles';
    style.textContent = `
        .toast-container {
            position: fixed;
            bottom: 20px;
            right: 20px;
            z-index: 9999;
            display: flex;
            flex-direction: column;
            gap: 10px;
        }
        
        .toast {
            background: white;
            border-radius: 8px;
            padding: 15px 20px;
            box-shadow: 0 5px 15px rgba(0,0,0,0.1);
            display: flex;
            align-items: center;
            gap: 12px;
            min-width: 300px;
            max-width: 400px;
            transform: translateX(100%);
            animation: slideInRight 0.3s ease forwards;
            border-left: 4px solid;
        }
        
        .toast.success {
            border-left-color: #06d6a0;
            background: #e6faf3;
        }
        
        .toast.error {
            border-left-color: #ff6b6b;
            background: #ffefef;
        }
        
        .toast.info {
            border-left-color: #667eea;
            background: #f0f4ff;
        }
        
        .toast.warning {
            border-left-color: #ffa500;
            background: #fff5e6;
        }
        
        .toast-icon {
            font-size: 20px;
        }
        
        .toast.success .toast-icon {
            color: #06d6a0;
        }
        
        .toast.error .toast-icon {
            color: #ff6b6b;
        }
        
        .toast.info .toast-icon {
            color: #667eea;
        }
        
        .toast.warning .toast-icon {
            color: #ffa500;
        }
        
        .toast-content {
            flex: 1;
        }
        
        .toast-title {
            font-weight: 600;
            margin-bottom: 4px;
            color: #212529;
        }
        
        .toast-message {
            font-size: 14px;
            color: #6c757d;
        }
        
        .toast-close {
            background: none;
            border: none;
            color: #adb5bd;
            cursor: pointer;
            padding: 5px;
            border-radius: 4px;
        }
        
        .toast-close:hover {
            background: #f8f9fa;
            color: #6c757d;
        }
        
        @keyframes slideInRight {
            from {
                transform: translateX(100%);
                opacity: 0;
            }
            to {
                transform: translateX(0);
                opacity: 1;
            }
        }
        
        @keyframes slideOutRight {
            from {
                transform: translateX(0);
                opacity: 1;
            }
            to {
                transform: translateX(100%);
                opacity: 0;
            }
        }
    `;
    document.head.appendChild(style);
}