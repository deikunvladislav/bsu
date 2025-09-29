import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class StringProcessorTest {

    @Test
    void testTokenize() {
        StringProcessor processor = new StringProcessor("12-30-45;abc;123", ";:-,");
        var tokens = processor.tokenize("12-30-45;abc;123", ";:-,");
        assertEquals(5, tokens.size());
        assertTrue(tokens.contains("abc"));
    }

    @Test
    void testProcessOutputStructure() {
        String input = "12-30-45;abc;(удалить);123;99;hello;23-59-59";
        String delimiters = ";:-,";
        StringProcessor processor = new StringProcessor(input, delimiters);
        String result = processor.process();

        assertTrue(result.contains("Исходная строка"));
        assertTrue(result.contains("Целые числа"));
        assertTrue(result.contains("Валидные времена"));
    }
}