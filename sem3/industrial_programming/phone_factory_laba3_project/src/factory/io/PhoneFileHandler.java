package factory.io;

import factory.model.Phone;
import factory.validation.PhoneValidator;
import java.io.*;
import java.util.*;

public class PhoneFileHandler extends AbstractFileHandler<Phone> {

    @Override
    public List<Phone> readFromFile(String filename) {
        List<Phone> phones = new ArrayList<>();

        try (BufferedReader br = new BufferedReader(new FileReader(filename));
             PrintWriter logWriter = new PrintWriter(new FileWriter("errors.log", true))) {

            Iterator<String> linesIt = br.lines().iterator();
            while (linesIt.hasNext()) {
                String line = linesIt.next();
                String[] parts = line.split(";");
                if (parts.length != 6) {
                    logWriter.println("Error: invalid field count -> " + line);
                    System.err.println("Error: invalid field count -> " + line);
                    continue;
                }

                String idStr = parts[0].trim();
                String brand = parts[1].trim();
                String model = parts[2].trim();
                String camerasStr = parts[3].trim();
                String dateStr = parts[4].trim();
                String priceStr = parts[5].trim();

                try {
                    if (!PhoneValidator.isValidId(idStr)) throw new IllegalArgumentException("Invalid ID");
                    if (!PhoneValidator.isValidBrand(brand)) throw new IllegalArgumentException("Invalid brand");
                    if (!PhoneValidator.isValidModel(model)) throw new IllegalArgumentException("Invalid model");
                    if (!PhoneValidator.isValidCameraCount(camerasStr)) throw new IllegalArgumentException("Invalid camera count");
                    if (!PhoneValidator.isValidDate(dateStr)) throw new IllegalArgumentException("Invalid date");
                    if (!PhoneValidator.isValidPrice(priceStr)) throw new IllegalArgumentException("Invalid price");

                    int id = Integer.parseInt(idStr);
                    int cameras = Integer.parseInt(camerasStr);
                    Date date = PhoneValidator.parseDate(dateStr);
                    double price = PhoneValidator.parsePrice(priceStr);

                    phones.add(new Phone(id, brand, model, cameras, date, price));

                } catch (Exception e) {
                    logWriter.println("Error: " + e.getMessage() + " | line: " + line);
                    System.err.println("Error: " + e.getMessage() + " | line: " + line);
                }
            }

        } catch (IOException e) {
            System.err.println("Error reading file: " + filename + " - " + e.getMessage());
        }

        return phones;
    }

    @Override
    public void writeToFile(String filename, List<Phone> data) {
        try (PrintWriter pw = new PrintWriter(new FileWriter(filename))) {
            Iterator<Phone> it = data.iterator();
            while (it.hasNext()) {
                Phone p = it.next();
                pw.printf("%d;%s;%s;%d;%s;%.2f%n",
                        p.getId(), p.getBrand(), p.getModel(),
                        p.getCameraCount(), PhoneValidator.formatDate(p.getReleaseDate()), p.getPrice());
            }
            System.out.println("Data successfully saved to " + filename);
        } catch (IOException e) {
            System.err.println("Error writing to file: " + filename + " - " + e.getMessage());
        }
    }
}