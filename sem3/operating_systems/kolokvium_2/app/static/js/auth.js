let isLoginSubmitting = false;
let isRegisterSubmitting = false;

function setupLoginForm() {
    const form = document.getElementById('login-form');
    if (!form) return;
    
    form.addEventListener('submit', async function(e) {
        e.preventDefault();
        e.stopPropagation();
        
        if (isLoginSubmitting) {
            return;
        }
        
        const username = document.getElementById('username').value.trim();
        const password = document.getElementById('password').value;
        
        if (!username || !password) {
            showToast('Please fill in all fields', 'warning', 'Validation Error');
            return;
        }
        
        isLoginSubmitting = true;
        const submitBtn = form.querySelector('button[type="submit"]');
        const originalText = submitBtn.innerHTML;
        submitBtn.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Logging in...';
        submitBtn.disabled = true;
        
        try {
            const response = await fetch('/api/auth/login', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({ username, password })
            });
            
            const data = await response.json();
            
            if (response.ok) {
                localStorage.setItem('jwt_token', data.access_token);
                localStorage.setItem('current_user', JSON.stringify(data.user));
                
                showToast('Login successful! Redirecting to dashboard...', 'success', 'Success');
                setTimeout(() => {
                    window.location.href = '/dashboard';
                }, 1500);
            } else {
                throw new Error(data.message || 'Login failed');
            }
        } catch (error) {
            showToast(error.message || 'Login failed. Please try again.', 'error', 'Error');
            submitBtn.innerHTML = originalText;
            submitBtn.disabled = false;
            isLoginSubmitting = false;
        }
    });
}

function setupRegisterForm() {
    const form = document.getElementById('register-form');
    if (!form) return;
    
    form.addEventListener('submit', async function(e) {
        e.preventDefault();
        e.stopPropagation();
        
        if (isRegisterSubmitting) {
            return;
        }
        
        const username = document.getElementById('username').value.trim();
        const password = document.getElementById('password').value;
        const confirmPassword = document.getElementById('confirm-password').value;
        
        if (!username || !password || !confirmPassword) {
            showToast('Please fill in all fields', 'warning', 'Validation Error');
            return;
        }
        
        if (password !== confirmPassword) {
            showToast('Passwords do not match', 'warning', 'Validation Error');
            return;
        }
        
        if (password.length < 6) {
            showToast('Password must be at least 6 characters', 'warning', 'Validation Error');
            return;
        }
        
        isRegisterSubmitting = true;
        const submitBtn = form.querySelector('button[type="submit"]');
        const originalText = submitBtn.innerHTML;
        submitBtn.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Creating account...';
        submitBtn.disabled = true;
        
        try {
            const response = await fetch('/api/auth/register', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({ username, password })
            });
            
            const data = await response.json();
            
            if (response.ok) {
                showToast('Registration successful! Redirecting to login...', 'success', 'Success');
                setTimeout(() => {
                    window.location.href = '/login';
                }, 1500);
            } else {
                throw new Error(data.message || 'Registration failed');
            }
        } catch (error) {
            showToast(error.message || 'Registration failed. Please try again.', 'error', 'Error');
            submitBtn.innerHTML = originalText;
            submitBtn.disabled = false;
            isRegisterSubmitting = false;
        }
    });
}

if (document.getElementById('login-form')) {
    setupLoginForm();
}

if (document.getElementById('register-form')) {
    setupRegisterForm();
}