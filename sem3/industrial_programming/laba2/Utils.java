import java.text.*;
import java.util.*;
import java.util.regex.*;

public class Utils {
    public static boolean isDecimalInteger(String s) {
        try {
            Integer.parseInt(s);
            return true;
        } catch (NumberFormatException e) {
            return false;
        }
    }

    public static int generateRandomBefore(List<Integer> numbers) {
        Random rand = new Random();
        if (numbers.isEmpty()) return rand.nextInt(100);
        return rand.nextInt(numbers.get(0));
    }

    public static String removeParentheses(String s) {
        Pattern pattern = Pattern.compile("([;:,-])?\\([^)]*\\)([;:,-])?");
        return pattern.matcher(s).replaceAll(match -> {
            String left = match.group(1);
            String right = match.group(2);
            return (left != null && right != null) ? left : "";
        });
    }

    public static List<String> extractTimePatterns(String input) {
        List<String> times = new ArrayList<>();
        Pattern timePattern = Pattern.compile("\\b\\d{2}-\\d{2}-\\d{2}\\b");
        Matcher matcher = timePattern.matcher(input);
        while (matcher.find()) {
            times.add(matcher.group());
        }
        return times;
    }

    public static List<Date> validateTimes(List<String> timeStrings) {
        List<Date> valid = new ArrayList<>();
        SimpleDateFormat sdf = new SimpleDateFormat("HH-mm-ss");
        sdf.setLenient(false);
        for (String time : timeStrings) {
            try {
                valid.add(sdf.parse(time));
            } catch (ParseException ignored) {}
        }
        return valid;
    }
}