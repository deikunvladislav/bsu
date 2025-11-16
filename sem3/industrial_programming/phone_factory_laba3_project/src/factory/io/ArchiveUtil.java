package factory.io;

import java.io.*;
import java.util.jar.JarEntry;
import java.util.jar.JarOutputStream;
import java.util.zip.*;

public class ArchiveUtil {
    
    public static void createJarArchive(String[] filesToInclude, String jarFilePath) {
        try (FileOutputStream fos = new FileOutputStream(jarFilePath);
            JarOutputStream jos = new JarOutputStream(fos)) {
            
            for (String filePath : filesToInclude) {
                File file = new File(filePath);
                if (file.exists()) {
                    addFileToJar(file, jos);
                }
            }
            System.out.println("JAR archive created successfully: " + jarFilePath);
            
        } catch (IOException e) {
            System.err.println("Error creating JAR archive: " + e.getMessage());
        }
    }
    
    private static void addFileToJar(File file, JarOutputStream jos) throws IOException {
        try (FileInputStream fis = new FileInputStream(file)) {
            JarEntry jarEntry = new JarEntry(file.getName());
            jos.putNextEntry(jarEntry);
            
            byte[] bytes = new byte[1024];
            int length;
            while ((length = fis.read(bytes)) >= 0) {
                jos.write(bytes, 0, length);
            }
            jos.closeEntry();
        }
    }

    public static void createZipArchiveFromFiles(String[] filesToInclude, String zipFilePath) {
        try (FileOutputStream fos = new FileOutputStream(zipFilePath);
            ZipOutputStream zos = new ZipOutputStream(fos)) {
            
            for (String filePath : filesToInclude) {
                File file = new File(filePath);
                if (file.exists()) {
                    addFileToZip(file, zos);
                }
            }
            
            System.out.println("ZIP archive created successfully: " + zipFilePath);
        } catch (IOException e) {
            System.err.println("Error creating ZIP archive: " + e.getMessage());
        }
    }
    
    private static void addFileToZip(File file, ZipOutputStream zos) throws IOException {
        try (FileInputStream fis = new FileInputStream(file)) {
            ZipEntry zipEntry = new ZipEntry(file.getName());
            zos.putNextEntry(zipEntry);
            
            byte[] bytes = new byte[1024];
            int length;
            while ((length = fis.read(bytes)) >= 0) {
                zos.write(bytes, 0, length);
            }
            zos.closeEntry();
        }
    }
}