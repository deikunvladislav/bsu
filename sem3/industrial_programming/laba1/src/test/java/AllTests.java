import org.junit.jupiter.api.*;
import java.io.*;
import java.math.BigDecimal;
import java.math.BigInteger;
import static org.junit.jupiter.api.Assertions.*;

public class AllTests {

    /* ===== Тесты для TaylorSinXDivX ===== */

    @Test
    void testComputeDoubleAccuracy() {
        double x = 1.0;
        double eps = 1e-6;
        double[] result = TaylorSinXDivX.computeDouble(x, eps);
        double approx = result[0];
        double exact = Math.sin(x) / x;
        assertEquals(exact, approx, eps, "computeDouble: неточная аппроксимация");
    }

    @Test
    void testComputeBigAccuracy() {
        BigDecimal x = new BigDecimal("1.0");
        int k = 6;
        Object[] result = TaylorSinXDivX.computeBig(x, k);
        BigDecimal approx = (BigDecimal) result[0];
        BigDecimal exact = BigDecimal.valueOf(Math.sin(x.doubleValue()) / x.doubleValue());
        BigDecimal diff = approx.subtract(exact).abs();
        assertTrue(diff.compareTo(BigDecimal.ONE.scaleByPowerOfTen(-k)) <= 0,
                "computeBig: неточная аппроксимация");
    }

    @Test
    void testXEqualsZero() {
        double[] result = TaylorSinXDivX.computeDouble(0.0, 1e-10);
        assertEquals(1.0, result[0], 1e-10, "sin(0)/0 должен быть 1 (предел)");
    }

    @Test
    void testSmallX() {
        double x = 1e-5;
        double[] result = TaylorSinXDivX.computeDouble(x, 1e-12);
        double exact = Math.sin(x) / x;
        assertEquals(exact, result[0], 1e-12);
    }

    @Test
    void testLargeX() {
        double x = 100.0;
        double[] result = TaylorSinXDivX.computeDouble(x, 1e-6);
        double exact = Math.sin(x) / x;
        assertEquals(exact, result[0], 1e-6);
    }

    @Test
    void testTermsCountReasonable() {
        double[] result = TaylorSinXDivX.computeDouble(1.0, 1e-6);
        int terms = (int) result[1];
        assertTrue(terms > 0 && terms < 1000, "Количество слагаемых должно быть разумным");
    }

    /* ===== Тесты для MainSimple ===== */

    @Test
    void testMainSimpleValidInput() throws Exception {
        String input = "1.0\n6\n";
        System.setIn(new ByteArrayInputStream(input.getBytes()));
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        System.setOut(new PrintStream(out));

        MainSimple.main(new String[]{});

        String output = out.toString();
        assertTrue(output.contains("Приближённо"), "Вывод должен содержать слово 'Приближённо'");
    }

    @Test
    void testMainSimpleInvalidX() {
        String input = "abc\n5\n";
        System.setIn(new ByteArrayInputStream(input.getBytes()));
        assertThrows(Exception.class, () -> MainSimple.main(new String[]{}));
    }

    @Test
    void testMainSimpleInvalidK() {
        String input = "1.0\nabc\n";
        System.setIn(new ByteArrayInputStream(input.getBytes()));
        assertThrows(Exception.class, () -> MainSimple.main(new String[]{}));
    }

    /* ===== Тесты для MainBig ===== */

    @Test
    void testMainBigValidInput() throws Exception {
        String input = "1.0\n6\n";
        System.setIn(new ByteArrayInputStream(input.getBytes()));
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        System.setOut(new PrintStream(out));

        MainBig.main(new String[]{});

        String output = out.toString();
        assertTrue(output.contains("Приближённо"), "Вывод должен содержать слово 'Приближённо'");
    }

    @Test
    void testMainBigInvalidX() {
        String input = "abc\n5\n";
        System.setIn(new ByteArrayInputStream(input.getBytes()));
        assertThrows(Exception.class, () -> MainBig.main(new String[]{}));
    }

    @Test
    void testMainBigInvalidK() {
        String input = "1.0\nabc\n";
        System.setIn(new ByteArrayInputStream(input.getBytes()));
        assertThrows(Exception.class, () -> MainBig.main(new String[]{}));
    }
}
