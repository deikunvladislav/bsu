import os
import re
import warnings
import numpy as np
import pandas as pd
import pyreadstat
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import seaborn as sns
from scipy import stats

warnings.filterwarnings('ignore')

SAV_PATH = r'C:\Users\HP\source\repos\bsu\sem5\data science\laba 1\Annual 2005-2011_START.sav'
TARGET_OUTLIER_FRACTION = (0.05, 0.08)
INVERSE_RATIOS = {'k5', 'k6', 'k7'}

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
RESULT_DIR = os.path.join(SCRIPT_DIR, 'результаты')
TABLE_DIR  = os.path.join(RESULT_DIR, 'таблицы')
FIG_DIR    = os.path.join(RESULT_DIR, 'графики')
os.makedirs(TABLE_DIR, exist_ok=True)
os.makedirs(FIG_DIR, exist_ok=True)

def save_table(df, name, index=False):
    xlsx_path = os.path.join(TABLE_DIR, name + '.xlsx')
    try:
        df.to_excel(xlsx_path, index=index, engine='openpyxl')
    except Exception as e:
        print(f"  ! Ошибка сохранения {xlsx_path}: {e}")

def load_sav(path):
    if not os.path.exists(path):
        raise FileNotFoundError(f"Файл не найден: {path}")
    print(f"Чтение файла: {path}")
    df, meta = pyreadstat.read_sav(path)
    print(f"Загружено: {df.shape[0]} строк, {df.shape[1]} столбцов")
    return df, meta

def extract_financial_ratios(df):
    df_k = df.copy()
    for c in df_k.columns:
        df_k[c] = pd.to_numeric(df_k[c], errors='coerce')
    df_k = df_k.replace([np.inf, -np.inf], np.nan)
    print(f"Столбцов: {len(df_k.columns)}")
    return df_k

def descriptive_stats(df):
    rows = []
    for col in df.columns:
        x = df[col].dropna()
        if len(x) == 0: continue
        rows.append({
            'Коэффициент': col, 'n': len(x), 'Min': x.min(), 'Max': x.max(),
            'Mean': x.mean(), 'Median': x.median(), 'Std': x.std(ddof=1),
            'Асимметрия': stats.skew(x), 'Эксцесс': stats.kurtosis(x),
        })
    return pd.DataFrame(rows)

def chi2_normality(x, bins=12):
    n = len(x)
    mu, sigma = x.mean(), x.std(ddof=1)
    probs = np.linspace(0, 1, bins + 1)
    edges = stats.norm.ppf(probs, loc=mu, scale=sigma)
    edges[0], edges[-1] = -np.inf, np.inf
    observed, _ = np.histogram(x, bins=edges)
    expected = n * np.diff(probs)
    obs, exp = observed.copy(), expected.copy()
    while len(exp) > 2 and (exp < 5).any():
        idx = int(np.where(exp < 5)[0][0])
        if idx == 0:
            obs[1] += obs[0]; exp[1] += exp[0]
            obs, exp = np.delete(obs, 0), np.delete(exp, 0)
        else:
            obs[idx-1] += obs[idx]; exp[idx-1] += exp[idx]
            obs, exp = np.delete(obs, idx), np.delete(exp, idx)
    chi2 = float(np.sum((obs - exp) ** 2 / exp))
    df_chi = max(len(obs) - 3, 1)
    return chi2, 1 - stats.chi2.cdf(chi2, df_chi)

def normality_tests(df):
    rows = []
    for col in df.columns:
        x = df[col].dropna().values
        if len(x) < 3:
            continue

        try:
            W, p_sh = stats.shapiro(x)
        except Exception as e:
            print(f"  ! Ошибка Шапиро для {col}: {e}")
            W, p_sh = np.nan, np.nan

        std_val = x.std(ddof=1)
        if std_val == 0 or np.isnan(std_val):
            print(f"  ! Предупреждение: std=0 для {col}, KS пропущен.")
            ks_s, p_ks = np.nan, np.nan
        else:
            try:
                ks_s, p_ks = stats.kstest(
                    x,
                    lambda v: stats.norm.cdf(v, loc=x.mean(), scale=std_val)
                )
            except Exception as e:
                print(f"  ! Ошибка KS для {col}: {e}")
                ks_s, p_ks = np.nan, np.nan

        try:
            chi2, p_chi2 = chi2_normality(x)
        except Exception as e:
            print(f"  ! Ошибка Chi2 для {col}: {e}")
            chi2, p_chi2 = np.nan, np.nan

        rows.append({
            'Коэффициент': col,
            'Shapiro_W': W,
            'Shapiro_p': p_sh,
            'KS_stat': ks_s,
            'KS_p': p_ks,
            'Chi2': chi2,
            'Chi2_p': p_chi2,
            'Нормально_0.05': bool((p_sh > 0.05) and (p_ks > 0.05) and (p_chi2 > 0.05))
        })
    return pd.DataFrame(rows)

def save_histograms(df, fig_dir, prefix=''):
    for col in df.columns:
        x = df[col].dropna()
        if len(x) < 2: continue
        plt.figure(figsize=(6, 4))
        sns.histplot(x, stat='density', bins=30, alpha=0.5, color='steelblue')
        xx = np.linspace(x.min(), x.max(), 200)
        plt.plot(xx, stats.norm.pdf(xx, x.mean(), x.std(ddof=1)), 'r-', lw=1.5)
        plt.title(f'Гистограмма {col}')
        plt.xlabel(col); plt.ylabel('Плотность')
        plt.savefig(os.path.join(fig_dir, f'{prefix}hist_{col}.png'), dpi=150, bbox_inches='tight')
        plt.close()

def save_boxplots(df, fig_dir, prefix=''):
    for col in df.columns:
        x = df[col].dropna()
        if len(x) < 2: continue
        plt.figure(figsize=(6, 3))
        sns.boxplot(x=x, color='lightgreen')
        plt.title(f'Boxplot {col}')
        plt.xlabel(col)
        plt.savefig(os.path.join(fig_dir, f'{prefix}box_{col}.png'), dpi=150, bbox_inches='tight')
        plt.close()

def normality_flags(df):
    tests = normality_tests(df)
    return dict(zip(tests['Коэффициент'], tests['Нормально_0.05']))

def mixed_corr_and_pvalues(df, normal_map):
    cols = df.columns
    corr = pd.DataFrame(np.nan, index=cols, columns=cols, dtype=float)
    pvals = pd.DataFrame(np.nan, index=cols, columns=cols, dtype=float)
    methods = pd.DataFrame('', index=cols, columns=cols, dtype=object)

    for i, c1 in enumerate(cols):
        corr.loc[c1, c1] = 1.0
        pvals.loc[c1, c1] = 0.0
        methods.loc[c1, c1] = '—'

        for j in range(i + 1, len(cols)):
            c2 = cols[j]
            common = df[[c1, c2]].dropna().index

            if len(common) < 3:
                r, p, m = np.nan, np.nan, 'NA'
            else:
                if normal_map.get(c1, False) and normal_map.get(c2, False):
                    m = 'pearson'
                else:
                    m = 'spearman'

                x = df.loc[common, c1].astype(float).values
                y = df.loc[common, c2].astype(float).values

                try:
                    if m == 'pearson':
                        r, p = stats.pearsonr(x, y)
                    else:
                        r, p = stats.spearmanr(x, y)
                except Exception as e:
                    print(f"  ! Ошибка корреляции {c1}-{c2} ({m}): {e}")
                    r, p = np.nan, np.nan

            corr.loc[c1, c2] = corr.loc[c2, c1] = r
            pvals.loc[c1, c2] = pvals.loc[c2, c1] = p
            methods.loc[c1, c2] = methods.loc[c2, c1] = m

    return corr, pvals, methods

def find_strong_correlations(corr, pvals, methods=None, threshold=0.7):
    pairs = []
    cols = corr.columns

    for i in range(len(cols)):
        for j in range(i + 1, len(cols)):
            r = corr.iloc[i, j]
            if pd.isna(r):
                continue

            if abs(r) > threshold:
                row = {
                    'K1': cols[i],
                    'K2': cols[j],
                    'r': r,
                    'p': pvals.iloc[i, j],
                    '|r|': abs(r)
                }
                if methods is not None:
                    row['Метод'] = methods.iloc[i, j]
                pairs.append(row)

    if pairs:
        return pd.DataFrame(pairs).sort_values('|r|', ascending=False)

    cols_out = ['K1', 'K2', 'r', 'p', '|r|']
    if methods is not None:
        cols_out.append('Метод')
    return pd.DataFrame(columns=cols_out)

def plot_corr_heatmap(corr, path, title):
    plt.figure(figsize=(16, 13))
    sns.heatmap(corr, cmap='RdBu_r', center=0, vmin=-1, vmax=1,
                annot=True, fmt='.2f', annot_kws={'size': 7},
                square=True, linewidths=0.5, linecolor='white',
                cbar_kws={'shrink': 0.8, 'label': 'Correlation coefficient'})
    plt.title(title, fontsize=14)
    plt.xticks(rotation=45, ha='right', fontsize=9)
    plt.yticks(rotation=0, fontsize=9)
    plt.tight_layout()
    plt.savefig(path, dpi=150, bbox_inches='tight')
    plt.close()

def hampel_bounds_for_series(series, target=TARGET_OUTLIER_FRACTION):
    x = series.dropna().values
    if len(x) == 0: return np.nan, np.nan, 5.2
    med = float(np.median(x))
    mad = float(np.median(np.abs(x - med)))
    if mad == 0: mad = float(np.std(x, ddof=1))
    if mad == 0: return float(x.min()), float(x.max()), 5.2
    c_opt = None
    for c in np.linspace(1.0, 20.0, 400):
        frac = float(np.mean((x < med - c * mad) | (x > med + c * mad)))
        if target[0] <= frac <= target[1]:
            c_opt = c
            break
    if c_opt is None:
        c_opt = 5.2
    return float(med - c_opt * mad), float(med + c_opt * mad), float(c_opt)

def compute_bounds_all(df_k):
    print("\n=== Границы цензурирования (тест Хампеля) ===")
    bounds, rows = {}, []
    for col in df_k.columns:
        lower, upper, c = hampel_bounds_for_series(df_k[col])
        bounds[col] = (lower, upper, c)
        x = df_k[col].dropna()
        frac = float(np.mean((x < lower) | (x > upper))) if len(x) else np.nan
        rows.append({
            'Коэффициент': col, 'Медиана': np.median(x) if len(x) else np.nan,
            'MAD': np.median(np.abs(x - np.median(x))) if len(x) else np.nan,
            'c': c, 'K_min': lower, 'K_max': upper, 'Доля за границами': frac
        })
        print(f"  {col:>4s}: c={c:4.2f}, Kmin={lower:10.4f}, Kmax={upper:10.4f}, выбросов={frac:.2%}")
    return bounds, pd.DataFrame(rows)

def censor_data(df_k, bounds):
    df_c = df_k.copy()
    for col, (lower, upper, _) in bounds.items():
        df_c[col] = df_c[col].clip(lower=lower, upper=upper)
    return df_c

def normalize_data(df_c, bounds):
    df_n = df_c.copy()
    for col in df_n.columns:
        lower, upper, _ = bounds[col]
        if upper - lower == 0 or np.isnan(upper - lower):
            df_n[col] = np.nan
            continue
        if col.lower() in INVERSE_RATIOS:
            df_n[col] = (upper - df_n[col]) / (upper - lower)
        else:
            df_n[col] = (df_n[col] - lower) / (upper - lower)
        df_n[col] = df_n[col].clip(0, 1)
    return df_n

def main():
    df, _ = load_sav(SAV_PATH)
    df_k = extract_financial_ratios(df)
    save_table(df_k, '01_исходные_коэффициенты')

    print("\n=== Анализ СЫРЫХ данных ===")
    save_table(descriptive_stats(df_k), '02_описательные_статистики_исходные')
    save_table(normality_tests(df_k), '03_проверка_нормальности_исходные')
    save_histograms(df_k, FIG_DIR, prefix='raw_')
    save_boxplots(df_k, FIG_DIR, prefix='raw_')

    print("\n=== Цензурирование и нормировка ===")
    bounds, bounds_info = compute_bounds_all(df_k)
    save_table(bounds_info, '04_границы_цензурирования')

    df_censored = censor_data(df_k, bounds)
    save_table(df_censored, '05_цензурированные')

    df_norm = normalize_data(df_censored, bounds)
    save_table(df_norm, '06_нормированные')
    print(f"Цензурирование и нормировка выполнены. Строк: {len(df_norm)}")

    print("\n=== Корреляции по НОРМИРОВАННЫМ данным ===")
    print("Для нормальных пар — Pearson, для остальных — Spearman")

    normal_flags = normality_flags(df_k)

    save_table(
        pd.DataFrame({
            'Коэффициент': list(normal_flags.keys()),
            'Нормально_0.05': list(normal_flags.values())
        }),
        '10_нормальность_для_выбора_корреляции'
    )

    corr_n, pvals_n, methods_n = mixed_corr_and_pvalues(df_norm, normal_flags)

    save_table(corr_n, '07_матрица_корреляций_нормированные', index=True)
    save_table(pvals_n, '08_p_value_корреляций_нормированные', index=True)
    save_table(methods_n, '09_методы_корреляций_нормированные', index=True)

    strong_n = find_strong_correlations(corr_n, pvals_n, methods_n, threshold=0.7)
    save_table(strong_n, '11_сильные_корреляции_нормированные')

    plot_corr_heatmap(
        corr_n,
        os.path.join(FIG_DIR, 'тепловая_карта_нормированные.png'),
        'Матрица корреляций: Pearson для нормальных пар, Spearman для остальных'
    )

    print(f"\n=== ГОТОВО ===\nТаблицы: {TABLE_DIR}\nГрафики: {FIG_DIR}")

if __name__ == '__main__':
    main()