package tests.factory.tests;

import factory.io.PhoneFileHandler;
import factory.model.Phone;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import java.io.File;
import java.io.FileWriter;
import java.nio.file.Path;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import static org.junit.jupiter.api.Assertions.*;

class PhoneFileHandlerTest {

    @TempDir
    Path tempDir;

    private Date createDate(String dateStr) throws Exception {
        return new SimpleDateFormat("yyyy-MM-dd").parse(dateStr);
    }

    @Test
    void testReadFromFile() throws Exception {
        File testFile = tempDir.resolve("test_phones.txt").toFile();
        try (FileWriter writer = new FileWriter(testFile)) {
            writer.write("1;Samsung;Galaxy S21;3;2021-01-29;699.99\n");
            writer.write("2;Apple;iPhone 13;2;2021-09-24;799.00\n");
            writer.write("3;Xiaomi;Redmi Note 10;4;2021-03-04;299.99\n");
        }
        PhoneFileHandler handler = new PhoneFileHandler();
        List<Phone> phones = handler.readFromFile(testFile.getAbsolutePath());
        assertEquals(3, phones.size());
        Phone first = phones.get(0);
        assertEquals(1, first.getId());
        assertEquals("Samsung", first.getBrand());
        assertEquals("Galaxy S21", first.getModel());
        assertEquals(3, first.getCameraCount());
        assertEquals(createDate("2021-01-29"), first.getReleaseDate());
        assertEquals(699.99, first.getPrice(), 0.001);
    }

    @Test
    void testReadFromFileWithInvalidData() throws Exception {
        File testFile = tempDir.resolve("invalid_phones.txt").toFile();
        try (FileWriter writer = new FileWriter(testFile)) {
            writer.write("1;Samsung;Galaxy S21;3;2021-01-29;699.99\n");
            writer.write("0;Invalid;Model;8;2025-01-01;100000\n");
            writer.write("2;Apple;iPhone 13;2;2021-09-24;799.00\n");
        }
        PhoneFileHandler handler = new PhoneFileHandler();
        List<Phone> phones = handler.readFromFile(testFile.getAbsolutePath());
        assertEquals(2, phones.size());
    }

    @Test
    void testReadFromNonExistentFile() {
        PhoneFileHandler handler = new PhoneFileHandler();
        List<Phone> phones = handler.readFromFile("non_existent_file.txt");
        assertNotNull(phones);
        assertTrue(phones.isEmpty());
    }

    @Test
    void testWriteToFile() throws Exception {
        File outputFile = tempDir.resolve("output_phones.txt").toFile();
        List<Phone> phones = List.of(
            new Phone(1, "Samsung", "Galaxy S21", 3, createDate("2021-01-29"), 699.99),
            new Phone(2, "Apple", "iPhone 13", 2, createDate("2021-09-24"), 799.00)
        );
        PhoneFileHandler handler = new PhoneFileHandler();
        handler.writeToFile(outputFile.getAbsolutePath(), phones);
        assertTrue(outputFile.exists());
        assertTrue(outputFile.length() > 0);
        List<Phone> readPhones = handler.readFromFile(outputFile.getAbsolutePath());
        assertEquals(2, readPhones.size());
    }

    @Test
    void testWriteEmptyList() throws Exception {
        File outputFile = tempDir.resolve("empty_phones.txt").toFile();
        PhoneFileHandler handler = new PhoneFileHandler();
        handler.writeToFile(outputFile.getAbsolutePath(), List.of());
        assertTrue(outputFile.exists());
    }

    @Test
    void testReadFromFileWithIOException() {
        PhoneFileHandler handler = new PhoneFileHandler();
        List<Phone> phones = handler.readFromFile(System.getProperty("user.home"));
        assertNotNull(phones);
        assertTrue(phones.isEmpty());
    }

    @Test
    void testWriteToFileWithIOException() {
        PhoneFileHandler handler = new PhoneFileHandler();
        List<Phone> phones = List.of(new Phone(1, "Test", "Model", 2, new Date(), 100.0));
        assertDoesNotThrow(() -> handler.writeToFile("/invalid/path/file.txt", phones));
    }
}