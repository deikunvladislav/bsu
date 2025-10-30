package factory.io;

import factory.model.Phone;
import factory.validation.PhoneValidator;
import org.json.JSONArray;
import org.json.JSONObject;
import org.json.JSONTokener;
import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class PhoneJsonFileHandler extends AbstractFileHandler<Phone> {

    @Override
    public List<Phone> readFromFile(String filename) {
        List<Phone> phones = new ArrayList<>();

        try (PrintWriter logWriter = new PrintWriter(new FileWriter("errors.log", true))) {
            File file = new File(filename);
            if (!file.exists()) {
                String errorMsg = "JSON file not found: " + filename;
                logWriter.println("Error: " + errorMsg);
                System.err.println("Error: " + errorMsg);
                return phones;
            }

            try (FileReader reader = new FileReader(filename)) {
                JSONTokener tokener = new JSONTokener(reader);
                JSONArray jsonArray = new JSONArray(tokener);

                for (int i = 0; i < jsonArray.length(); i++) {
                    JSONObject jsonObject = jsonArray.getJSONObject(i);
                    
                    String idStr = String.valueOf(jsonObject.getInt("id"));
                    String brand = jsonObject.getString("brand");
                    String model = jsonObject.getString("model");
                    String camerasStr = String.valueOf(jsonObject.getInt("cameraCount"));
                    String dateStr = jsonObject.getString("releaseDate");
                    String priceStr = String.format("%.2f", jsonObject.getDouble("price"));

                    try {
                        if (!PhoneValidator.isValidId(idStr)) throw new IllegalArgumentException("Invalid ID");
                        if (!PhoneValidator.isValidBrand(brand)) throw new IllegalArgumentException("Invalid brand");
                        if (!PhoneValidator.isValidModel(model)) throw new IllegalArgumentException("Invalid model");
                        if (!PhoneValidator.isValidCameraCount(camerasStr)) throw new IllegalArgumentException("Invalid camera count");
                        if (!PhoneValidator.isValidDate(dateStr)) throw new IllegalArgumentException("Invalid date");
                        if (!PhoneValidator.isValidPrice(priceStr)) throw new IllegalArgumentException("Invalid price");

                        int id = Integer.parseInt(idStr);
                        int cameras = Integer.parseInt(camerasStr);
                        var date = PhoneValidator.parseDate(dateStr);
                        double price = PhoneValidator.parsePrice(priceStr);

                        phones.add(new Phone(id, brand, model, cameras, date, price));

                    } catch (Exception e) {
                        String errorMsg = "Error reading JSON object: " + e.getMessage() + " | data: " + 
                                            String.join(";", idStr, brand, model, camerasStr, dateStr, priceStr);
                        logWriter.println("Error: " + errorMsg);
                        System.err.println("Error: " + errorMsg);
                    }
                }
            }
        } catch (Exception e) {
            try (PrintWriter logWriter = new PrintWriter(new FileWriter("errors.log", true))) {
                String errorMsg = "Error reading JSON file: " + filename + " - " + e.getMessage();
                logWriter.println("Error: " + errorMsg);
                System.err.println("Error: " + errorMsg);
            } catch (IOException ioException) {
                System.err.println("Failed to write to error log: " + ioException.getMessage());
            }
        }

        return phones;
    }

    @Override
    public void writeToFile(String filename, List<Phone> data) {
        try (PrintWriter pw = new PrintWriter(new FileWriter(filename))) {
            JSONArray jsonArray = new JSONArray();

            for (Phone p : data) {
                JSONObject jsonObject = new JSONObject();
                jsonObject.put("id", p.getId());
                jsonObject.put("brand", p.getBrand());
                jsonObject.put("model", p.getModel());
                jsonObject.put("cameraCount", p.getCameraCount());
                jsonObject.put("releaseDate", PhoneValidator.formatDate(p.getReleaseDate()));
                jsonObject.put("price", p.getPrice());
                
                jsonArray.put(jsonObject);
            }

            pw.print(jsonArray.toString(4));
            System.out.println("Data successfully saved to JSON file: " + filename);
        } catch (IOException e) {
            System.err.println("Error writing JSON file: " + filename + " - " + e.getMessage());
        }
    }
}