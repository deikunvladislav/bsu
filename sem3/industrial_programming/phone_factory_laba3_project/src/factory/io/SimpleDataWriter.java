package factory.io;

import factory.model.Phone;
import java.util.List;
import java.io.*;
import java.util.ArrayList;

public class SimpleDataWriter implements DataWriter {
    
    @Override
    public void write(String filename, List<Phone> data) {
        try (PrintWriter writer = new PrintWriter(new FileWriter(filename))) {
            for (Phone phone : data) {
                writer.println(phone.toString());
            }
            System.out.println("Data written to: " + filename);
        } catch (IOException e) {
            System.err.println("Error writing to file: " + e.getMessage());
        }
    }

    @Override
    public List<Phone> read(String filename) {
        List<Phone> phones = new ArrayList<>();
        File file = new File(filename);
        if (!file.exists()) {
            return phones;
        }
        
        System.out.println("Reading from: " + filename);
        return phones;
    }
}