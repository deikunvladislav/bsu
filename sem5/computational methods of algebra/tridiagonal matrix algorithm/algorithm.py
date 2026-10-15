# Input data
# Матрица коэффициентов A и вектор правой части f
A = [
    [ 0.9384,  0.0000,  0.0000,  0.0000,  0.0000],
    [-0.0575,  0.6128,  0.0000,  0.0000,  0.0000],
    [ 0.0000, -0.1724,  1.1107,  0.0211,  0.0000],
    [ 0.0000,  0.0000, -0.1398,  1.1107,  0.0000],
    [ 0.0000,  0.0000,  0.0000, -0.0230,  0.8043]
]
f = [2.9951, -3.3187, 2.6676, 3.3398, -3.9181]
n = len(f)

a = [0.0] * n
c = [0.0] * n
b = [0.0] * n

for i in range(n):
    c[i] = A[i][i]
    if i > 0:
        a[i] = -A[i][i - 1]
    if i < n - 1:
        b[i] = -A[i][i + 1]

# --- ПРАВАЯ ПРОГОНКА ---
alpha = [0.0] * n
beta  = [0.0] * n

# Прямой ход 
alpha[0] = b[0] / c[0]
beta[0]  = f[0] / c[0]

for i in range(1, n):
    den = c[i] - alpha[i - 1] * a[i]
    alpha[i] = b[i] / den
    beta[i]  = (f[i] + a[i] * beta[i - 1]) / den

# Обратный ход 
x = [0.0] * n
x[n - 1] = beta[n - 1]
for i in range(n - 2, -1, -1):
    x[i] = alpha[i] * x[i + 1] + beta[i]

# --- НЕВЯЗКА r = A*x - f ---
r = [0.0] * n
for i in range(n):
    s = 0.0
    for j in range(n):
        s += A[i][j] * x[j]
    r[i] = s - f[i]

# --- OUTPUT ---
print("\n=== Final Result ===")
print("\nVector of unknowns x:")
for i in range(n):
    print(f"x[{i}] = {x[i]:.6f}")

print("\n=== Residual ===")
print("Residual r = Ax - f:")
for i in range(n):
    print(f"  r[{i}] = {r[i]:.2e}")
    
    
print("\n=== Stability tests ===")
print("nmax |alpha_i| =", round(max(abs(a) for a in alpha), 6))

print("\n=== Dens ===")
alpha[0] = b[0] / c[0]
beta[0]  = f[0] / c[0]
for i in range(1, n):
    den = c[i] - alpha[i - 1] * a[i]
    alpha[i] = b[i] / den
    beta[i]  = (f[i] + a[i] * beta[i - 1]) / den
    print(f"den[{i}] = {den:.6f}\t")
    # print(f"alpha[{i}] = {alpha[i]:.6f}\t")
    # print(f"beta[{i}] = {beta[i]:.6f}\t")