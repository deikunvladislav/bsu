import org.junit.jupiter.api.Test;
import java.util.*;
import static org.junit.jupiter.api.Assertions.*;

public class UtilsTest {

    @Test
    void testIsDecimalInteger() {
        assertTrue(Utils.isDecimalInteger("123"));
        assertFalse(Utils.isDecimalInteger("abc"));
    }

    @Test
    void testGenerateRandomBefore() {
        int r = Utils.generateRandomBefore(List.of(50));
        assertTrue(r >= 0 && r < 50);
    }

    @Test
    void testRemoveParentheses() {
        assertEquals("abc;123", Utils.removeParentheses("abc;(удалить);123"));
        assertEquals("abc;123", Utils.removeParentheses("abc(удалить);123"));
        assertEquals("abc;123", Utils.removeParentheses("abc;(удалить)123"));
    }

    @Test
    void testExtractTimePatterns() {
        List<String> times = Utils.extractTimePatterns("12-30-45;23-59-59");
        assertEquals(List.of("12-30-45", "23-59-59"), times);
    }

    @Test
    void testValidateTimes() {
        List<Date> valid = Utils.validateTimes(List.of("12-30-45", "99-99-99"));
        assertEquals(1, valid.size());
    }
}