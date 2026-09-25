# Input data
A = [
    [0.9384,  0.0000, -0.2451,  0.1724,  0.2873],
    [-0.0575, 0.6128,  0.0000, -0.1168,  0.0383],
    [0.0192, -0.1724,  1.1107,  0.0211,  0.0670],
    [0.0575,  0.0000, -0.1398,  1.1107,  0.0000],
    [0.0383, -0.0575,  0.2777, -0.0230,  0.8043]
]
b = [2.9951, -3.3187, 2.6676, 3.3398, -3.9181]
n = 5

# Augmented matrix [A | b | E]
Aug = [[0.0] * (2 * n + 1) for _ in range(n)]
for i in range(n):
    for j in range(n):
        Aug[i][j] = A[i][j]
    Aug[i][n] = b[i]
    Aug[i][n + 1 + i] = 1.0

# Track variable permutation (column swaps)
col_perm = list(range(n))

swaps = 0
pivots = []

# --- FORWARD ELIMINATION ---
for k in range(n):
    # find max element in the row
    max_val = abs(Aug[k][k])
    pivot_col = k
    for j in range(k + 1, n):
        if abs(Aug[k][j]) > max_val:
            max_val = abs(Aug[k][j])
            pivot_col = j

    # Swap columns if needed
    if pivot_col != k:
        for i in range(n):
            Aug[i][k], Aug[i][pivot_col] = Aug[i][pivot_col], Aug[i][k]
            Aug[i][n + 1 + k], Aug[i][n + 1 + pivot_col] = Aug[i][n + 1 + pivot_col], Aug[i][n + 1 + k]
        col_perm[k], col_perm[pivot_col] = col_perm[pivot_col], col_perm[k]
        swaps += 1  # Column swap changes determinant sign

    pivot = Aug[k][k]
    pivots.append(pivot)

    # Divide k-th row by pivot
    for j in range(k, 2 * n + 1):
        Aug[k][j] = Aug[k][j] / pivot

    # Eliminate x_k below k-th row
    for i in range(k + 1, n):
        factor = Aug[i][k]
        for j in range(k, 2 * n + 1):
            Aug[i][j] = Aug[i][j] - factor * Aug[k][j]

# Determinant
det = 1.0
if swaps % 2 != 0:
    det = -1.0
for p in pivots:
    det *= p

# --- BACK SUBSTITUTION ---
# Solve for y (permuted variables)
y = [0.0] * n
y[n - 1] = Aug[n - 1][n]
for i in range(n - 2, -1, -1):
    s = Aug[i][n]
    for j in range(i + 1, n):
        s -= Aug[i][j] * y[j]
    y[i] = s

# Recover x using permutation
x = [0.0] * n
for i in range(n):
    x[col_perm[i]] = y[i]

# Solve for inverse matrix Z (inverse of permuted matrix)
Z = [[0.0] * n for _ in range(n)]
for col in range(n):
    y_vec = [0.0] * n
    y_vec[n - 1] = Aug[n - 1][n + 1 + col]
    for i in range(n - 2, -1, -1):
        s = Aug[i][n + 1 + col]
        for j in range(i + 1, n):
            s -= Aug[i][j] * y_vec[j]
        y_vec[i] = s
    for i in range(n):
        Z[i][col] = y_vec[i]

# Recover A_inv
A_inv = [[0.0] * n for _ in range(n)]
for i in range(n):
    for j in range(n):
        A_inv[i][col_perm[j]] = Z[i][j]

# r = Ax - b
r_x = [0.0] * n
for i in range(n):
    s = 0.0
    for j in range(n):
        s += A[i][j] * x[j]
    r_x[i] = s - b[i]

# r_inv = A * A_inv - E
r_inv = [[0.0] * n for _ in range(n)]
for i in range(n):
    for j in range(n):
        s = 0.0
        for k in range(n):
            s += A[i][k] * A_inv[k][j]
        identity = 1.0 if i == j else 0.0
        r_inv[i][j] = s - identity

# --- OUTPUT ---
print("\n=== Final Results ===")
print("\nVector of unknowns x:")
for i in range(n):
    print(f"x[{i}] = {x[i]:.6f}")

print(f"\nDeterminant of matrix A: {det:.6f}")

print("\nInverse matrix A^(-1):")
for row in A_inv:
    print("  [" + ", ".join(f"{val:.6f}" for val in row) + "]")

print("\n=== Residuals ===")
print("Residual r = Ax - b:")
for i in range(n):
    print(f"  r_x[{i}] = {r_x[i]:.2e}")

print("\nResidual r_inv = A*A^(-1) - E:")
for row in r_inv:
    print("  [" + ", ".join(f"{val:.2e}" for val in row) + "]")