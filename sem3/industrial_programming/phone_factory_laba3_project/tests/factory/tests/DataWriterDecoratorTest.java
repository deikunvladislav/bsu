package tests.factory.tests;

import factory.io.*;
import factory.model.Phone;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import java.io.File;
import java.nio.file.Path;
import java.util.Date;
import java.util.List;
import static org.junit.jupiter.api.Assertions.*;

class DataWriterDecoratorTest {

    @TempDir
    Path tempDir;

    @Test
    void testFileHandlerAdapter() {
        File testFile = tempDir.resolve("test.txt").toFile();

        List<Phone> phones = List.of(
            new Phone(1, "Test", "Model", 2, new Date(), 100.0)
        );

        DataWriter writer = new FileHandlerAdapter(new PhoneFileHandler());
        
        writer.write(testFile.getAbsolutePath(), phones);
        
        assertTrue(testFile.exists());
        
        List<Phone> readPhones = writer.read(testFile.getAbsolutePath());
        
        assertNotNull(readPhones);
    }

    @Test
    void testDecoratorChain() {
        DataWriter baseWriter = new FileHandlerAdapter(new PhoneFileHandler());
        DataWriter decoratedWriter = new CompressionDataWriterDecorator(
            new EncryptionDataWriterDecorator(baseWriter)
        );

        assertNotNull(decoratedWriter);
        
        assertDoesNotThrow(() -> {
            DataWriter writer = new CompressionDataWriterDecorator(
                new EncryptionDataWriterDecorator(
                    new FileHandlerAdapter(new PhoneFileHandler())
                )
            );
        });
    }
    
    @Test
    void testBasicDataWriterOperation() {
        File testFile = tempDir.resolve("basic_test.txt").toFile();
        DataWriter writer = new FileHandlerAdapter(new PhoneFileHandler());
        
        List<Phone> phones = List.of(
            new Phone(1, "Brand1", "Model1", 2, new Date(), 200.0),
            new Phone(2, "Brand2", "Model2", 3, new Date(), 300.0)
        );
        
        assertDoesNotThrow(() -> writer.write(testFile.getAbsolutePath(), phones));
        
        assertDoesNotThrow(() -> {
            List<Phone> result = writer.read(testFile.getAbsolutePath());
            assertNotNull(result);
        });
    }
}