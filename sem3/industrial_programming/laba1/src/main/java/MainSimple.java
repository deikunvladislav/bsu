import java.util.Formatter;
import java.util.Locale;
import java.util.Scanner;

public class MainSimple {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        scanner.useLocale(Locale.US); // Чтобы "1.0" стабильно парсилось независимо от системной локали

        System.out.print("Введите x (double): ");
        double x = scanner.nextDouble();
        System.out.print("Введите k (натур., точность ε=10^-k): ");
        int k = scanner.nextInt();
        scanner.close();

        double eps = Math.pow(10, -k);
        double[] result = TaylorSinXDivX.computeDouble(x, eps);
        double approx = result[0];
        int terms = (int) result[1];
        double exact  = Math.sin(x) / x;

        Formatter fmt = new Formatter(Locale.US); // вывод тоже в предсказуемой локали
        fmt.format("Число слагаемых: %#o (oct), %#X (hex)%n", terms, terms);
        fmt.format("k (точность 10^-k): %#o (oct), %#X (hex)%n", k, k);

        int width = k + 3;
        // Корректная спецификация формата для числа с плавающей точкой
        String floatFmt = "%(+#0" + width + "." + (k + 1) + "f";
        fmt.format("Приближённо sin(x)/x = " + floatFmt + "%n", approx);
        fmt.format("Точное     sin(x)/x = " + floatFmt + "%n", exact);

        System.out.print(fmt);
        fmt.close();
    }
}
