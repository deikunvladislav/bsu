window.showToast = function(message, type = 'info', title = '') {
    let container = document.getElementById('toast-container');
    if (!container) {
        container = document.createElement('div');
        container.className = 'toast-container';
        container.id = 'toast-container';
        document.body.appendChild(container);
    }
    
    const toast = document.createElement('div');
    toast.className = `toast ${type}`;
    
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
    
    const closeBtn = toast.querySelector('.toast-close');
    closeBtn.addEventListener('click', () => {
        toast.style.animation = 'slideOutRight 0.3s ease forwards';
        setTimeout(() => toast.remove(), 300);
    });
    
    container.appendChild(toast);
    
    const autoRemove = setTimeout(() => {
        if (toast.parentNode) {
            toast.style.animation = 'slideOutRight 0.3s ease forwards';
            setTimeout(() => toast.remove(), 300);
        }
    }, 5000);
    
    toast.addEventListener('mouseenter', () => {
        clearTimeout(autoRemove);
    });
    
    toast.addEventListener('mouseleave', () => {
        setTimeout(() => {
            if (toast.parentNode) {
                toast.style.animation = 'slideOutRight 0.3s ease forwards';
                setTimeout(() => toast.remove(), 300);
            }
        }, 5000);
    });
};

window.showConfirm = function(message, title = 'Confirmation') {
    return new Promise((resolve) => {
        const modal = document.createElement('div');
        modal.className = 'modal confirm-modal';
        modal.style.display = 'flex';
        
        modal.innerHTML = `
            <div class="modal-content" style="max-width: 400px;">
                <div class="modal-header">
                    <h3><i class="fas fa-question-circle"></i> ${title}</h3>
                    <button class="close-modal">&times;</button>
                </div>
                <div class="modal-body">
                    <p>${message}</p>
                    <div class="modal-actions">
                        <button id="confirm-yes" class="btn btn-primary">
                            <i class="fas fa-check"></i> Yes
                        </button>
                        <button id="confirm-no" class="btn btn-secondary">
                            <i class="fas fa-times"></i> No
                        </button>
                    </div>
                </div>
            </div>
        `;
        
        document.body.appendChild(modal);
        
        const closeModal = () => {
            modal.style.animation = 'fadeOut 0.3s ease forwards';
            setTimeout(() => {
                if (modal.parentNode) {
                    modal.remove();
                }
            }, 300);
        };
        
        const closeBtn = modal.querySelector('.close-modal');
        const yesBtn = modal.querySelector('#confirm-yes');
        const noBtn = modal.querySelector('#confirm-no');
        
        const handleYes = () => {
            closeModal();
            resolve(true);
        };
        
        const handleNo = () => {
            closeModal();
            resolve(false);
        };
        
        closeBtn.addEventListener('click', handleNo);
        yesBtn.addEventListener('click', handleYes);
        noBtn.addEventListener('click', handleNo);
        
        modal.addEventListener('click', (e) => {
            if (e.target === modal) {
                handleNo();
            }
        });
        
        const handleEscape = (e) => {
            if (e.key === 'Escape') {
                handleNo();
                document.removeEventListener('keydown', handleEscape);
            }
        };
        
        document.addEventListener('keydown', handleEscape);
        
        const originalCloseModal = closeModal;
        closeModal = () => {
            document.removeEventListener('keydown', handleEscape);
            originalCloseModal();
        };
    });
};

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
        
        .confirm-modal {
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background: rgba(0,0,0,0.5);
            z-index: 10000;
            display: none;
            align-items: center;
            justify-content: center;
            animation: fadeIn 0.3s ease;
        }
        
        .confirm-modal .modal-actions {
            display: flex;
            gap: 10px;
            justify-content: flex-end;
            margin-top: 20px;
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
        
        @keyframes fadeIn {
            from {
                opacity: 0;
            }
            to {
                opacity: 1;
            }
        }
        
        @keyframes fadeOut {
            from {
                opacity: 1;
            }
            to {
                opacity: 0;
            }
        }
        
        [data-theme="dark"] .toast {
            background: #2d3748;
            color: #e2e8f0;
            border-left-color: var(--primary);
        }
        
        [data-theme="dark"] .toast.success {
            background: #064e3b;
            border-left-color: #059669;
        }
        
        [data-theme="dark"] .toast.error {
            background: #7f1d1d;
            border-left-color: #dc2626;
        }
        
        [data-theme="dark"] .toast.info {
            background: #1e3a8a;
            border-left-color: #3b82f6;
        }
        
        [data-theme="dark"] .toast.warning {
            background: #78350f;
            border-left-color: #d97706;
        }
        
        [data-theme="dark"] .toast .toast-title {
            color: #e2e8f0;
        }
        
        [data-theme="dark"] .toast .toast-message {
            color: #cbd5e0;
        }
        
        [data-theme="dark"] .toast-close {
            color: #a0aec0;
        }
        
        [data-theme="dark"] .toast-close:hover {
            background: #4a5568;
            color: #e2e8f0;
        }
    `;
    document.head.appendChild(style);
}