package tests.factory.tests;

import factory.io.PhoneXmlFileHandler;
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

class PhoneXmlFileHandlerTest {

    @TempDir
    Path tempDir;

    private Date createDate(String dateStr) throws Exception {
        return new SimpleDateFormat("yyyy-MM-dd").parse(dateStr);
    }

    @Test
    void testReadFromFile() throws Exception {
        File testFile = tempDir.resolve("test_phones.xml").toFile();
        String xmlContent = """
            <phones>
                <phone>
                    <id>1</id>
                    <brand>Samsung</brand>
                    <model>Galaxy S21</model>
                    <cameraCount>3</cameraCount>
                    <releaseDate>2021-01-29</releaseDate>
                    <price>699.99</price>
                </phone>
                <phone>
                    <id>2</id>
                    <brand>Apple</brand>
                    <model>iPhone 13</model>
                    <cameraCount>2</cameraCount>
                    <releaseDate>2021-09-24</releaseDate>
                    <price>799.00</price>
                </phone>
            </phones>
            """;
        try (FileWriter writer = new FileWriter(testFile)) {
            writer.write(xmlContent);
        }
        PhoneXmlFileHandler handler = new PhoneXmlFileHandler();
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
        File testFile = tempDir.resolve("invalid_phones.xml").toFile();
        String xmlContent = """
            <phones>
                <phone>
                    <id>1</id>
                    <brand>Samsung</brand>
                    <model>Galaxy S21</model>
                    <cameraCount>3</cameraCount>
                    <releaseDate>2021-01-29</releaseDate>
                    <price>699.99</price>
                </phone>
                <phone>
                    <id>0</id>
                    <brand>Invalid</brand>
                    <model>Model</model>
                    <cameraCount>8</cameraCount>
                    <releaseDate>2025-01-01</releaseDate>
                    <price>100000</price>
                </phone>
            </phones>
            """;
        try (FileWriter writer = new FileWriter(testFile)) {
            writer.write(xmlContent);
        }
        PhoneXmlFileHandler handler = new PhoneXmlFileHandler();
        List<Phone> phones = handler.readFromFile(testFile.getAbsolutePath());
        assertEquals(1, phones.size());
    }

    @Test
    void testReadFromNonExistentFile() {
        PhoneXmlFileHandler handler = new PhoneXmlFileHandler();
        List<Phone> phones = handler.readFromFile("non_existent_file.xml");
        assertNotNull(phones);
        assertTrue(phones.isEmpty());
    }

    @Test
    void testWriteToFile() throws Exception {
        File outputFile = tempDir.resolve("output_phones.xml").toFile();
        List<Phone> phones = List.of(
            new Phone(1, "Samsung", "Galaxy S21", 3, createDate("2021-01-29"), 699.99),
            new Phone(2, "Apple", "iPhone 13", 2, createDate("2021-09-24"), 799.00)
        );
        PhoneXmlFileHandler handler = new PhoneXmlFileHandler();
        handler.writeToFile(outputFile.getAbsolutePath(), phones);
        assertTrue(outputFile.exists());
        assertTrue(outputFile.length() > 0);
        String content = new String(java.nio.file.Files.readAllBytes(outputFile.toPath()));
        assertTrue(content.contains("<phones>"));
        assertTrue(content.contains("<phone>"));
        assertTrue(content.contains("Samsung"));
        assertTrue(content.contains("Apple"));
    }

    @Test
    void testWriteEmptyList() throws Exception {
        File outputFile = tempDir.resolve("empty_phones.xml").toFile();
        PhoneXmlFileHandler handler = new PhoneXmlFileHandler();
        
        handler.writeToFile(outputFile.getAbsolutePath(), List.of());        
        
        assertTrue(outputFile.exists(), "File should exist");
        assertTrue(outputFile.length() > 0, "File should not be empty");        
        
        List<Phone> readPhones = handler.readFromFile(outputFile.getAbsolutePath());
        assertNotNull(readPhones, "Read phones should not be null");
        assertTrue(readPhones.isEmpty(), "Read phones should be empty");        
        
        String content = new String(java.nio.file.Files.readAllBytes(outputFile.toPath()));
        assertTrue(content.contains("phones"), "Should contain phones element");
    }
}