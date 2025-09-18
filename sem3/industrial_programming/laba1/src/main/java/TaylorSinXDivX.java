import java.math.BigDecimal;
import java.math.BigInteger;
import java.math.MathContext;

public class TaylorSinXDivX {

    // Порог, при котором ряд Тейлора становится численно неустойчивым для double
    private static final double DOUBLE_TAYLOR_THRESHOLD = 10.0;

    public static double[] computeDouble(double x, double epsilon) {
        // Предел sin(0)/0 = 1
        if (x == 0.0) {
            return new double[]{1.0, 1};
        }

        // Для больших |x| используем прямой расчёт — ряд Тейлора численно плох
        if (Math.abs(x) > DOUBLE_TAYLOR_THRESHOLD) {
            return new double[]{Math.sin(x) / x, 1};
        }

        double term = 1.0;
        double sum = term;
        int n = 0;
        while (Math.abs(term) >= epsilon) {
            term *= -1.0 * x * x / ((2.0 * n + 2.0) * (2.0 * n + 3.0));
            sum += term;
            n++;
            // Страховка от бесконечного цикла при экстремальных epsilon
            if (n > 1_000_000) break;
        }
        return new double[]{sum, n + 1};
    }

    public static Object[] computeBig(BigDecimal x, int k) {
        // Предел sin(0)/0 = 1
        if (x.compareTo(BigDecimal.ZERO) == 0) {
            return new Object[]{BigDecimal.ONE, BigInteger.ONE};
        }

        MathContext mc = new MathContext(k + 5);
        BigDecimal eps = BigDecimal.ONE.scaleByPowerOfTen(-k);

        // Для очень больших |x| (по модулю) используем double-путь ради стабильности,
        // затем возвращаем BigDecimal с нужным контекстом.
        if (x.abs().compareTo(BigDecimal.TEN) > 0) {
            double xv = x.doubleValue();
            double val = Math.sin(xv) / xv;
            BigDecimal approx = new BigDecimal(val, mc);
            return new Object[]{approx, BigInteger.ONE};
        }

        BigDecimal term = BigDecimal.ONE;
        BigDecimal sum = term;
        int n = 0;
        while (term.abs().compareTo(eps) >= 0) {
            BigInteger a = BigInteger.valueOf(2L * n + 2L);
            BigInteger b = BigInteger.valueOf(2L * n + 3L);
            BigDecimal denom = new BigDecimal(a.multiply(b));
            term = term.multiply(x, mc)
                       .multiply(x, mc)
                       .divide(denom, mc)
                       .negate();
            sum = sum.add(term, mc);
            n++;
            if (n > 1_000_000) break;
        }
        return new Object[]{sum, BigInteger.valueOf(n + 1)};
    }
}
