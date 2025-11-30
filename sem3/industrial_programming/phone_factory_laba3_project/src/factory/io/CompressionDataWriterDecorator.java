package factory.io;

import factory.model.Phone;
import java.util.List;
import java.util.zip.*;
import java.io.*;

public class CompressionDataWriterDecorator extends AbstractDataWriterDecorator {
    
    public CompressionDataWriterDecorator(DataWriter wrappedWriter) {
        super(wrappedWriter);
    }

    @Override
    public void write(String filename, List<Phone> data) {
        String compressedFilename = filename + ".gz";
        try {
            File tempFile = File.createTempFile("phone_data", ".tmp");
            wrappedWriter.write(tempFile.getAbsolutePath(), data);
            
            try (FileInputStream fis = new FileInputStream(tempFile);
                FileOutputStream fos = new FileOutputStream(compressedFilename);
                GZIPOutputStream gzos = new GZIPOutputStream(fos)) {
                
                byte[] buffer = new byte[1024];
                int length;
                while ((length = fis.read(buffer)) > 0) {
                    gzos.write(buffer, 0, length);
                }
            }
            
            tempFile.delete();
            
            System.out.println("Data compressed and saved to: " + compressedFilename);
        } catch (IOException e) {
            System.err.println("Error compressing data: " + e.getMessage());
            wrappedWriter.write(filename, data);
        }
    }

    @Override
    public List<Phone> read(String filename) {
        String compressedFilename = filename + ".gz";
        File compressedFile = new File(compressedFilename);
        
        if (compressedFile.exists()) {
            try {
                File tempFile = File.createTempFile("phone_data", ".tmp");
                
                try (FileInputStream fis = new FileInputStream(compressedFilename);
                    GZIPInputStream gzis = new GZIPInputStream(fis);
                    FileOutputStream fos = new FileOutputStream(tempFile)) {
                    
                    byte[] buffer = new byte[1024];
                    int length;
                    while ((length = gzis.read(buffer)) > 0) {
                        fos.write(buffer, 0, length);
                    }
                }
                List<Phone> data = wrappedWriter.read(tempFile.getAbsolutePath());
                
                tempFile.delete();
                
                System.out.println("Data decompressed from: " + compressedFilename);
                return data;
            } catch (IOException e) {
                System.err.println("Error decompressing data: " + e.getMessage());
            }
        }
        return wrappedWriter.read(filename);
    }
}