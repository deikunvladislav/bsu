package factory.io;

import factory.model.Phone;

import java.util.ArrayList;
import java.util.List;
import java.io.*;

public class EncryptionDataWriterDecorator extends AbstractDataWriterDecorator {
    
    public EncryptionDataWriterDecorator(DataWriter wrappedWriter) {
        super(wrappedWriter);
    }

    @Override
    public void write(String filename, List<Phone> data) {
        String encryptedFilename = filename + ".enc";
        try {
            StringBuilder jsonData = new StringBuilder();
            jsonData.append("[");
            for (int i = 0; i < data.size(); i++) {
                Phone phone = data.get(i);
                jsonData.append(String.format(
                    "{\"id\":%d,\"brand\":\"%s\",\"model\":\"%s\",\"cameraCount\":%d,\"releaseDate\":\"%s\",\"price\":%.2f}",
                    phone.getId(), phone.getBrand(), phone.getModel(), phone.getCameraCount(),
                    new java.text.SimpleDateFormat("yyyy-MM-dd").format(phone.getReleaseDate()),
                    phone.getPrice()
                ));
                if (i < data.size() - 1) {
                    jsonData.append(",");
                }
            }
            jsonData.append("]");
            
            String encryptedData = EncryptionUtil.encrypt(jsonData.toString());
            
            try (PrintWriter writer = new PrintWriter(new FileWriter(encryptedFilename))) {
                writer.print(encryptedData);
            }
            
            System.out.println("Data encrypted and saved to: " + encryptedFilename);
        } catch (Exception e) {
            System.err.println("Error encrypting data: " + e.getMessage());
            wrappedWriter.write(filename, data);
        }
    }

    @Override
    public List<Phone> read(String filename) {
        String encryptedFilename = filename + ".enc";
        File encryptedFile = new File(encryptedFilename);
        
        if (encryptedFile.exists()) {
            try {
                StringBuilder encryptedData = new StringBuilder();
                try (BufferedReader reader = new BufferedReader(new FileReader(encryptedFile))) {
                    String line;
                    while ((line = reader.readLine()) != null) {
                        encryptedData.append(line);
                    }
                }
                
                String decryptedData = EncryptionUtil.decrypt(encryptedData.toString());
                System.out.println("Data decrypted from: " + encryptedFilename);
                
                List<Phone> phones = new ArrayList<>();
                return phones;
            } catch (Exception e) {
                System.err.println("Error decrypting data: " + e.getMessage());
            }
        }
        
        return wrappedWriter.read(filename);
    }
}