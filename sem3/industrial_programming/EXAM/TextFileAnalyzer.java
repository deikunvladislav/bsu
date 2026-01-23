import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

public class TextFileAnalyzer {
    
    public static void main(String[] args) {
        Path filePath = Paths.get("input.txt");
        
        try {
            String content = Files.readString(filePath);
            
            System.out.println("Анализ текстового файла\n");
            
            // Извлекаем email-адреса для исключения их из анализа слов
            List<String> emails = extractEmails(content);
            
            // Удаляем email-адреса из текста для анализа слов
            String textWithoutEmails = content;
            for (String email : emails) {
                textWithoutEmails = textWithoutEmails.replace(email, " ");
            }
            
            // 1. Общее количество слов
            List<String> allWords = extractWords(textWithoutEmails);
            System.out.println("1. Общее количество слов: " + allWords.size());
            
            // 2. Количество уникальных слов
            Set<String> uniqueWords = new HashSet<>();
            Map<String, Integer> wordFrequency = new HashMap<>();
            
            for (String word : allWords) {
                String lowerWord = word.toLowerCase();
                uniqueWords.add(lowerWord);
                wordFrequency.put(lowerWord, wordFrequency.getOrDefault(lowerWord, 0) + 1);
            }
            System.out.println("2. Количество уникальных слов: " + uniqueWords.size());
            
            // 3. Топ-10 самых частых слов
            System.out.println("3. Топ-10 самых частых слов:");
            List<Map.Entry<String, Integer>> sortedWords = wordFrequency.entrySet()
                    .stream()
                    .sorted((e1, e2) -> e2.getValue().compareTo(e1.getValue()))
                    .limit(10)
                    .collect(Collectors.toList());
            
            for (int i = 0; i < sortedWords.size(); i++) {
                Map.Entry<String, Integer> entry = sortedWords.get(i);
                System.out.printf("   %d. '%s' - %d раз(а)\n", i + 1, entry.getKey(), entry.getValue());
            }
            
            // 4. Количество предложений (с обработкой email и многоточий)
            int sentenceCount = countSentences(content, emails);
            System.out.println("4. Количество предложений: " + sentenceCount);
            
            // 5. Email-адреса
            System.out.println("5. Найденные email-адреса:");
            if (emails.isEmpty()) {
                System.out.println("   Email-адреса не найдены.");
            } else {
                for (int i = 0; i < emails.size(); i++) {
                    System.out.printf("   %d. %s\n", i + 1, emails.get(i));
                }
            }
            
        } catch (IOException e) {
            System.out.println("Ошибка чтения файла: " + e.getMessage());
        }
    }
    
    private static List<String> extractWords(String text) {
        List<String> words = new ArrayList<>();
        // Используем улучшенное регулярное выражение для слов
        // Ищем последовательности букв (русских и английских) и дефисов
        Pattern pattern = Pattern.compile("\\b(?:[a-zA-Zа-яА-ЯёЁ]+-)*[a-zA-Zа-яА-ЯёЁ]+\\b");
        Matcher matcher = pattern.matcher(text);
        
        while (matcher.find()) {
            String word = matcher.group();
            // Убираем дефисы в начале и конце слова
            word = word.replaceAll("^-+|-$", "");
            if (!word.isEmpty()) {
                words.add(word);
            }
        }
        return words;
    }
    
    private static int countSentences(String text, List<String> emails) {
        // Создаем копию текста для обработки
        String processedText = text;
        
        // Заменяем email-адреса на метки без точек, чтобы точки в email не ломали подсчет
        for (String email : emails) {
            String placeholder = email.replace(".", "[dot]");
            processedText = processedText.replace(email, placeholder);
        }
        
        // Обработка многоточий (...)
        processedText = processedText.replace("...", "…");
        
        // Обработка чисел с точками (45.6, 7.8)
        processedText = processedText.replaceAll("\\b\\d+\\.\\d+\\b", "NUMBER");
        
        // Теперь ищем предложения
        Pattern pattern = Pattern.compile("[^.!?…]+[.!?…]");
        Matcher matcher = pattern.matcher(processedText);
        
        int count = 0;
        while (matcher.find()) {
            String sentence = matcher.group().trim();
            // Проверяем, что предложение не пустое и содержит буквы
            if (!sentence.isEmpty() && sentence.matches(".*[a-zA-Zа-яА-ЯёЁ].*")) {
                count++;
            }
        }
        
        // Если в конце текста нет знака препинания, но есть слова
        String remaining = processedText.replaceAll("[^.!?…]+[.!?…]", "").trim();
        if (!remaining.isEmpty() && remaining.matches(".*[a-zA-Zа-яА-ЯёЁ].*")) {
            count++;
        }
        
        return count;
    }
    
    private static List<String> extractEmails(String text) {
        List<String> emails = new ArrayList<>();
        String emailRegex = "[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}";
        Pattern pattern = Pattern.compile(emailRegex);
        Matcher matcher = pattern.matcher(text);
        
        while (matcher.find()) {
            emails.add(matcher.group());
        }
        return emails;
    }
}