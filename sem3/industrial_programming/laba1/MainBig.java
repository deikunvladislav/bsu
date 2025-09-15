import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.math.BigDecimal;
import java.util.Formatter;

public class MainBig {
    public static void main(String[] args) throws Exception {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        System.out.print("Введите x (BigDecimal): ");
        BigDecimal x = new BigDecimal(br.readLine().trim());
        System.out.print("Введите k (натур., точность ε=10^-k): ");
        int k = Integer.parseInt(br.readLine().trim());

        Object[] result = TaylorSinXDivX.computeBig(x, k);
        BigDecimal approx = (BigDecimal) result[0];
        BigDecimal exact  = BigDecimal.valueOf(
            Math.sin(x.doubleValue()) / x.doubleValue()
        );
        java.math.BigInteger terms = (java.math.BigInteger) result[1];

        Formatter fmt = new Formatter();
        fmt.format("Число слагаемых: %#o (oct), %#X (hex)%n", terms, terms);
        fmt.format("k (точность 10^-k): %#o (oct), %#X (hex)%n", k, k);

        int width = k + 3;
        String floatFmt = "%(+#0" + width + "." + (k+1) + "f";
        fmt.format("Приближённо sin(x)/x = " + floatFmt + "%n", approx);
        fmt.format("Точное     sin(x)/x = " + floatFmt + "%n", exact);

        System.out.print(fmt);
        fmt.close();
    }
}