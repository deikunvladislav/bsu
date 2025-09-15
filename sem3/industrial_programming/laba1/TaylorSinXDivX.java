import java.math.BigDecimal;
import java.math.BigInteger;
import java.math.MathContext;

public class TaylorSinXDivX {
    public static double[] computeDouble(double x, double epsilon) {
        double term = 1.0, sum = term;
        int n = 0;
        while (Math.abs(term) >= epsilon) {
            term *= -1.0 * x * x / ((2.0*n + 2.0)*(2.0*n + 3.0));
            sum  += term; n++;
        }
        return new double[]{ sum, n+1 };
    }

    public static Object[] computeBig(BigDecimal x, int k) {
        MathContext mc = new MathContext(k + 5);
        BigDecimal eps = BigDecimal.ONE.scaleByPowerOfTen(-k);
        BigDecimal term = BigDecimal.ONE, sum = term;
        BigInteger count = BigInteger.ONE;
        int n = 0;
        while (term.abs().compareTo(eps) >= 0) {
            BigInteger a = BigInteger.valueOf(2L*n + 2L);
            BigInteger b = BigInteger.valueOf(2L*n + 3L);
            BigDecimal denom = new BigDecimal(a.multiply(b));
            term = term.multiply(x, mc)
                       .multiply(x, mc)
                       .divide(denom, mc)
                       .negate();
            sum = sum.add(term, mc);
            n++;
            count = BigInteger.valueOf(n + 1);
        }
        return new Object[]{ sum, count };
    }
}
