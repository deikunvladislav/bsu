import java.io.*;

public class Main {
    public static void main(String[] args) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("input.txt"));
            String lexemeLine = reader.readLine();
            String delimiters = reader.readLine();
            reader.close();

            StringProcessor processor = new StringProcessor(lexemeLine, delimiters);
            String result = processor.process();

            BufferedWriter writer = new BufferedWriter(new FileWriter("output.txt"));
            writer.write(result);
            writer.close();

            System.out.println("Обработка завершена. Результаты записаны в output.txt");
        } catch (IOException e) {
            System.err.println("Ошибка при работе с файлами: " + e.getMessage());
        }
    }
}