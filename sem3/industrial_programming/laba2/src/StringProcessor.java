import java.text.*;
import java.util.*;
import java.util.regex.*;

public class StringProcessor {
    private final String input;
    private final String delimiters;

    public StringProcessor(String input, String delimiters) {
        this.input = input;
        this.delimiters = delimiters;
    }

    public String process() {
        StringBuilder output = new StringBuilder();

        // Поиск времени в исходной строке
        List<String> timeStrings = Utils.extractTimePatterns(input);

        // Разделение строки на лексемы
        List<String> lexemes = tokenize(input, delimiters);

        // Выделение целых чисел
        List<Integer> numbers = new ArrayList<>();
        for (String lex : lexemes) {
            if (Utils.isDecimalInteger(lex)) {
                numbers.add(Integer.parseInt(lex));
            }
        }

        // Добавление случайного числа
        int random = Utils.generateRandomBefore(numbers);
        StringBuilder modified = new StringBuilder(input);
        modified.insert(0, random + " ");

        // Удаление скобок и лишних разделителей
        String cleaned = Utils.removeParentheses(modified.toString());

        // Валидация времени
        List<Date> validTimes = Utils.validateTimes(timeStrings);
        validTimes.sort(Comparator.naturalOrder());

        // Форматирование времени в ЧЧ:ММ:СС
        SimpleDateFormat timeOnlyFormat = new SimpleDateFormat("HH:mm:ss");
        List<String> formattedTimes = new ArrayList<>();
        for (Date date : validTimes) {
            formattedTimes.add(timeOnlyFormat.format(date));
        }

        // Форматирование процентов и валюты
        NumberFormat percentFormat = NumberFormat.getPercentInstance(new Locale("ru", "BY"));
        NumberFormat currencyFormat = NumberFormat.getCurrencyInstance(new Locale("ru", "BY"));
        String percent = percentFormat.format(0.75);
        String currency = currencyFormat.format(1234.56);

        // Вывод
        output.append("Исходная строка: ").append(input).append("\n");
        output.append("Разделители: ").append(delimiters).append("\n");
        output.append("Лексемы: ").append(lexemes).append("\n");
        output.append("Целые числа (10сс): ").append(numbers).append("\n");
        output.append("Валидные времена: ").append(formattedTimes).append("\n");
        output.append("Модифицированная строка: ").append(cleaned).append("\n");
        output.append("Процент: ").append(percent).append("\n");
        output.append("Валюта: ").append(currency).append("\n");

        return output.toString();
    }

    public List<String> tokenize(String str, String delims) {
        StringTokenizer tokenizer = new StringTokenizer(str, delims);
        List<String> tokens = new ArrayList<>();
        while (tokenizer.hasMoreTokens()) {
            tokens.add(tokenizer.nextToken());
        }
        return tokens;
    }
}