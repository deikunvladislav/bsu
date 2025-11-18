package tests.factory.tests;

import factory.io.ArchiveUtil;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;
import static org.junit.jupiter.api.Assertions.*;

class ArchiveUtilTest {

    @TempDir
    Path tempDir;

    @Test
    void testCreateZipArchiveFromFiles() throws IOException {
        File file1 = tempDir.resolve("test1.txt").toFile();
        try (FileWriter writer = new FileWriter(file1)) {
            writer.write("Test content 1");
        }
        File file2 = tempDir.resolve("test2.txt").toFile();
        try (FileWriter writer = new FileWriter(file2)) {
            writer.write("Test content 2");
        }
        
        String zipPath = tempDir.resolve("test.zip").toString();
        String[] filesToInclude = { file1.getAbsolutePath(), file2.getAbsolutePath() };
        
        ArchiveUtil.createZipArchiveFromFiles(filesToInclude, zipPath);
        
        File zipFile = new File(zipPath);
        assertTrue(zipFile.exists());
        assertTrue(zipFile.length() > 0);
        
        try (ZipInputStream zis = new ZipInputStream(Files.newInputStream(zipFile.toPath()))) {
            ZipEntry entry;
            int fileCount = 0;
            while ((entry = zis.getNextEntry()) != null) {
                if (!entry.isDirectory()) fileCount++;
            }
            assertEquals(2, fileCount);
        }
    }

    @Test
    void testCreateJarArchive() throws IOException {
        File file1 = tempDir.resolve("test1.txt").toFile();
        try (FileWriter writer = new FileWriter(file1)) {
            writer.write("Test content 1");
        }
        File file2 = tempDir.resolve("test2.txt").toFile();
        try (FileWriter writer = new FileWriter(file2)) {
            writer.write("Test content 2");
        }
        
        String jarPath = tempDir.resolve("test.jar").toString();
        String[] filesToInclude = { file1.getAbsolutePath(), file2.getAbsolutePath() };
        
        ArchiveUtil.createJarArchive(filesToInclude, jarPath);
        
        File jarFile = new File(jarPath);
        assertTrue(jarFile.exists());
        assertTrue(jarFile.length() > 0);
        
        try (ZipInputStream zis = new ZipInputStream(Files.newInputStream(jarFile.toPath()))) {
            ZipEntry entry;
            int fileCount = 0;
            while ((entry = zis.getNextEntry()) != null) {
                if (!entry.isDirectory()) fileCount++;
            }
            assertEquals(2, fileCount);
        }
    }

    @Test
    void testCreateZipArchiveFromFilesWithNonExistentFiles() {
        File tempFile = tempDir.resolve("existing.txt").toFile();
        try (FileWriter writer = new FileWriter(tempFile)) {
            writer.write("Test content");
        } catch (IOException e) {
            fail("Failed to create temp file");
        }
        
        String zipPath = tempDir.resolve("test.zip").toString();
        String[] filesToInclude = { tempFile.getAbsolutePath(), "non_existent_file.txt" };
        
        assertDoesNotThrow(() -> ArchiveUtil.createZipArchiveFromFiles(filesToInclude, zipPath));
        
        File zipFile = new File(zipPath);
        assertTrue(zipFile.exists());
        
        try (ZipInputStream zis = new ZipInputStream(Files.newInputStream(zipFile.toPath()))) {
            ZipEntry entry;
            int fileCount = 0;
            while ((entry = zis.getNextEntry()) != null) {
                if (!entry.isDirectory()) fileCount++;
            }
            assertEquals(1, fileCount); 
        } catch (IOException e) {
            fail("Error reading zip file: " + e.getMessage());
        }
    }

    @Test
    void testCreateJarWithNonExistentFiles() {
        File tempFile = tempDir.resolve("existing.txt").toFile();
        try (FileWriter writer = new FileWriter(tempFile)) {
            writer.write("Test content");
        } catch (IOException e) {
            fail("Failed to create temp file");
        }
    
        String jarPath = tempDir.resolve("test.jar").toString();
        String[] filesToInclude = { tempFile.getAbsolutePath(), "non_existent_file2.txt" };
        
        assertDoesNotThrow(() -> ArchiveUtil.createJarArchive(filesToInclude, jarPath));
        
        File jarFile = new File(jarPath);
        assertTrue(jarFile.exists());
        
        try (ZipInputStream zis = new ZipInputStream(Files.newInputStream(jarFile.toPath()))) {
            ZipEntry entry;
            int fileCount = 0;
            while ((entry = zis.getNextEntry()) != null) {
                if (!entry.isDirectory()) fileCount++;
            }
            assertEquals(1, fileCount);
        } catch (IOException e) {
            fail("Error reading jar file: " + e.getMessage());
        }
    }

    @Test
    void testCreateZipArchiveFromEmptyFileList() {
        String zipPath = tempDir.resolve("empty.zip").toString();
        String[] emptyFiles = {};
        
        assertDoesNotThrow(() -> ArchiveUtil.createZipArchiveFromFiles(emptyFiles, zipPath));
        
        File zipFile = new File(zipPath);
        assertTrue(zipFile.exists());
        
        try (ZipInputStream zis = new ZipInputStream(Files.newInputStream(zipFile.toPath()))) {
            ZipEntry entry;
            int fileCount = 0;
            while ((entry = zis.getNextEntry()) != null) {
                if (!entry.isDirectory()) fileCount++;
            }
            assertEquals(0, fileCount);
        } catch (IOException e) {
            fail("Error reading zip file: " + e.getMessage());
        }
    }
}