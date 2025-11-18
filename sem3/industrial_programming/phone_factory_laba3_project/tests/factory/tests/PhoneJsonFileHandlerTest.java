package tests.factory.tests;

import factory.io.PhoneJsonFileHandler;
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

class PhoneJsonFileHandlerTest {

    @TempDir
    Path tempDir;

    private Date createDate(String dateStr) throws Exception {
        return new SimpleDateFormat("yyyy-MM-dd").parse(dateStr);
    }

    @Test
    void testReadFromFile() throws Exception {
        File testFile = tempDir.resolve("test_phones.json").toFile();
        String jsonContent = """
            [
                {
                    "id": 1,
                    "brand": "Samsung",
                    "model": "Galaxy S21",
                    "cameraCount": 3,
                    "releaseDate": "2021-01-29",
                    "price": 699.99
                },
                {
                    "id": 2,
                    "brand": "Apple",
                    "model": "iPhone 13",
                    "cameraCount": 2,
                    "releaseDate": "2021-09-24",
                    "price": 799.00
                }
            ]
            """;
        try (FileWriter writer = new FileWriter(testFile)) {
            writer.write(jsonContent);
        }
        PhoneJsonFileHandler handler = new PhoneJsonFileHandler();
        List<Phone> phones = handler.readFromFile(testFile.getAbsolutePath());
        assertEquals(2, phones.size());
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
        File testFile = tempDir.resolve("invalid_phones.json").toFile();
        String jsonContent = """
            [
                {
                    "id": 1,
                    "brand": "Samsung",
                    "model": "Galaxy S21",
                    "cameraCount": 3,
                    "releaseDate": "2021-01-29",
                    "price": 699.99
                },
                {
                    "id": 0,
                    "brand": "Invalid",
                    "model": "Model",
                    "cameraCount": 8,
                    "releaseDate": "2025-01-01",
                    "price": 100000
                }
            ]
            """;
        try (FileWriter writer = new FileWriter(testFile)) {
            writer.write(jsonContent);
        }
        PhoneJsonFileHandler handler = new PhoneJsonFileHandler();
        List<Phone> phones = handler.readFromFile(testFile.getAbsolutePath());
        assertEquals(1, phones.size());
    }

    @Test
    void testReadFromNonExistentFile() {
        PhoneJsonFileHandler handler = new PhoneJsonFileHandler();
        List<Phone> phones = handler.readFromFile("non_existent_file.json");
        assertNotNull(phones);
        assertTrue(phones.isEmpty());
    }

    @Test
    void testWriteToFile() throws Exception {
        File outputFile = tempDir.resolve("output_phones.json").toFile();
        List<Phone> phones = List.of(
            new Phone(1, "Samsung", "Galaxy S21", 3, createDate("2021-01-29"), 699.99),
            new Phone(2, "Apple", "iPhone 13", 2, createDate("2021-09-24"), 799.00)
        );
        PhoneJsonFileHandler handler = new PhoneJsonFileHandler();
        handler.writeToFile(outputFile.getAbsolutePath(), phones);
        assertTrue(outputFile.exists());
        assertTrue(outputFile.length() > 0);
        String content = new String(java.nio.file.Files.readAllBytes(outputFile.toPath()));
        assertTrue(content.contains("["));
        assertTrue(content.contains("{"));
        assertTrue(content.contains("Samsung"));
        assertTrue(content.contains("Apple"));
        assertTrue(content.contains("699.99"));
    }

    @Test
    void testWriteEmptyList() throws Exception {
        File outputFile = tempDir.resolve("empty_phones.json").toFile();
        PhoneJsonFileHandler handler = new PhoneJsonFileHandler();
        handler.writeToFile(outputFile.getAbsolutePath(), List.of());
        assertTrue(outputFile.exists());
        String content = new String(java.nio.file.Files.readAllBytes(outputFile.toPath()));
        assertTrue(content.contains("[]"));
    }
}