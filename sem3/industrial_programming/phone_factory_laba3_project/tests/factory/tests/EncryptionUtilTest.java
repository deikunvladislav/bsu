package tests.factory.tests;

import factory.io.EncryptionUtil;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.ValueSource;
import static org.junit.jupiter.api.Assertions.*;

class EncryptionUtilTest {

    @ParameterizedTest
    @ValueSource(strings = {"Hello, World!", "Test with special chars: !@#$%^&*()", "Русский текст", "", "1234567890", "Line1\nLine2\nLine3"})
    void testEncryptionDecryption(String originalText) {
        String encrypted = EncryptionUtil.encrypt(originalText);
        String decrypted = EncryptionUtil.decrypt(encrypted);
        assertNotNull(encrypted);
        assertNotEquals(originalText, encrypted);
        assertEquals(originalText, decrypted);
    }

    @Test
    void testLongStringEncryption() {
        StringBuilder longString = new StringBuilder();
        for (int i = 0; i < 1000; i++) longString.append("A");
        String originalText = longString.toString();
        String encrypted = EncryptionUtil.encrypt(originalText);
        String decrypted = EncryptionUtil.decrypt(encrypted);
        assertNotNull(encrypted);
        assertNotEquals(originalText, encrypted);
        assertEquals(originalText, decrypted);
    }

    @Test
    void testEncryptedDataIsConsistent() {
        String text = "Consistent test";
        String encrypted1 = EncryptionUtil.encrypt(text);
        String encrypted2 = EncryptionUtil.encrypt(text);
        assertEquals(encrypted1, encrypted2);
    }

    @Test
    void testDecryptInvalidData() {
        String invalidData = "invalid-encrypted-data";
        assertThrows(RuntimeException.class, () -> EncryptionUtil.decrypt(invalidData));
    }

    @Test
    void testEmptyString() {
        String original = "";
        String encrypted = EncryptionUtil.encrypt(original);
        String decrypted = EncryptionUtil.decrypt(encrypted);
        assertEquals(original, decrypted);
    }

    @Test
    void testNullSafety() {
        assertThrows(RuntimeException.class, () -> EncryptionUtil.encrypt(null));
        assertThrows(RuntimeException.class, () -> EncryptionUtil.decrypt(null));
    }

    @Test
    void testEncryptionWithNewKey() {
        String testData = "Test data for new key validation";
        String encrypted = EncryptionUtil.encrypt(testData);
        String decrypted = EncryptionUtil.decrypt(encrypted);
        
        assertNotNull(encrypted);
        assertNotEquals(testData, encrypted);
        assertEquals(testData, decrypted);
    }
}