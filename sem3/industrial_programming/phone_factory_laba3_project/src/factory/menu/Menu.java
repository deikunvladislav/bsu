package factory.menu;

import factory.io.*;
import factory.model.Phone;
import factory.storage.AbstractStorage;
import factory.storage.PhoneMapStorage;
import factory.validation.PhoneValidator;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Scanner;
import java.util.function.Predicate;

import org.json.JSONArray;
import org.json.JSONObject;

public class Menu {
    private Scanner scanner = new Scanner(System.in);
    private AbstractStorage<Phone> storage;
    private AbstractFileHandler<Phone> currentFileHandler;
    private PhoneXmlFileHandler xmlFileHandler;
    private PhoneJsonFileHandler jsonFileHandler;

    public Menu(AbstractStorage<Phone> storage, AbstractFileHandler<Phone> fileHandler, 
                PhoneXmlFileHandler xmlFileHandler, PhoneJsonFileHandler jsonFileHandler) {
        this.storage = storage;
        this.currentFileHandler = fileHandler;
        this.xmlFileHandler = xmlFileHandler;
        this.jsonFileHandler = jsonFileHandler;
    }

    public void show() {
        while (true) {
            System.out.println("\n===== PHONE FACTORY MENU =====");
            System.out.println("1. Show all phones");
            System.out.println("2. Add phone");
            System.out.println("3. Delete phone");
            System.out.println("4. Edit phone");
            System.out.println("5. Sort phones");
            System.out.println("6. Demonstrate SortedMap");
            System.out.println("7. Save data (current format)");
            System.out.println("8. Change file format");
            System.out.println("9. Encrypt data");
            System.out.println("10. Create archive");
            System.out.println("11. Demonstrate Decorator Pattern");
            System.out.println("12. Demonstrate Builder Pattern");
            System.out.println("13. Exit");
            System.out.print("Choice: ");
            
            try {
                int choice = scanner.nextInt();
                scanner.nextLine();

                switch (choice) {
                    case 1 -> storage.printAll();
                    case 2 -> addPhone();
                    case 3 -> deletePhone();
                    case 4 -> editPhone();
                    case 5 -> sortPhones();
                    case 6 -> demonstrateSortedMap();
                    case 7 -> saveCurrentFormat();
                    case 8 -> changeFileFormat();
                    case 9 -> encryptData();
                    case 10 -> createArchive();
                    case 11 -> demonstrateDecoratorPattern();
                    case 12 -> demonstrateBuilderPattern();
                    case 13 -> {
                        System.out.println("Exiting program...");
                        return;
                    }
                    default -> System.out.println("Invalid choice! Please try again.");
                }
            } catch (Exception e) {
                System.out.println("Invalid input! Please enter a number.");
                scanner.nextLine();
            }
        }
    }

    private void addPhone() {
        try (PrintWriter logWriter = new PrintWriter(new FileWriter("errors.log", true))) {
            System.out.println("\n--- ADD NEW PHONE ---");
            
            int id = readValidatedInput("ID: ", PhoneValidator::isValidId, 
                "Error: ID must be a positive integer!", Integer::parseInt);
            
            if (storage.findById(id) != null) {
                String errorMsg = "Phone with ID " + id + " already exists!";
                logWriter.println("Error: " + errorMsg);
                System.out.println("Error: " + errorMsg);
                return;
            }

            String brand = readValidatedInput("Brand: ", PhoneValidator::isValidBrand,
                "Error: Brand must contain only letters and spaces (2-50 characters)!", s -> s);

            String model = readValidatedInput("Model: ", PhoneValidator::isValidModel,
                "Error: Model format is invalid!", s -> s);

            int cameras = readValidatedInput("Cameras (0-7): ", PhoneValidator::isValidCameraCount,
                "Error: Camera count must be between 0 and 7!", Integer::parseInt);

            String dateStr = readValidatedInput("Release date (yyyy-MM-dd): ", PhoneValidator::isValidDate,
                "Error: Date must be in format yyyy-MM-dd!", s -> s);
            var date = PhoneValidator.parseDate(dateStr);

            String priceStr = readValidatedInput("Price: ", PhoneValidator::isValidPrice,
                "Error: Price format is invalid!", s -> s);
            double price = PhoneValidator.parsePrice(priceStr);

            if (price <= 0) {
                String errorMsg = "Price must be positive!";
                logWriter.println("Error: " + errorMsg);
                System.out.println("Error: " + errorMsg);
                return;
            }

            Phone phone = new Phone(id, brand, model, cameras, date, price);
            storage.add(phone);
            System.out.println("Phone successfully added!");
            
        } catch (Exception e) {
            try (PrintWriter logWriter = new PrintWriter(new FileWriter("errors.log", true))) {
                String errorMsg = "Data input error in addPhone: " + e.getMessage();
                logWriter.println("Error: " + errorMsg);
                System.err.println("Error: " + errorMsg);
            } catch (IOException ioException) {
                System.err.println("Failed to write to error log: " + ioException.getMessage());
            }
        }
    }

    private <T> T readValidatedInput(String prompt, Predicate<String> validator, 
                                    String errorMessage, java.util.function.Function<String, T> converter) {
        while (true) {
            System.out.print(prompt);
            String input = scanner.nextLine().trim();
            if (validator.test(input)) {
                return converter.apply(input);
            }
            System.out.println(errorMessage);
        }
    }

    private void deletePhone() {
        try (PrintWriter logWriter = new PrintWriter(new FileWriter("errors.log", true))) {
            System.out.print("Enter ID to delete: ");
            int id = scanner.nextInt();
            scanner.nextLine();
            
            Phone phone = storage.findById(id);
            if (phone == null) {
                String errorMsg = "Phone with ID " + id + " not found for deletion";
                logWriter.println("Error: " + errorMsg);
                System.out.println("Error: " + errorMsg);
                return;
            }
            
            storage.delete(id);
            System.out.println("Phone with ID " + id + " deleted successfully.");
        } catch (Exception e) {
            try (PrintWriter logWriter = new PrintWriter(new FileWriter("errors.log", true))) {
                String errorMsg = "Error in deletePhone: " + e.getMessage();
                logWriter.println("Error: " + errorMsg);
                System.err.println("Error: " + errorMsg);
            } catch (IOException ioException) {
                System.err.println("Failed to write to error log: " + ioException.getMessage());
            }
        }
    }

    private void editPhone() {
        try (PrintWriter logWriter = new PrintWriter(new FileWriter("errors.log", true))) {
            System.out.print("Enter phone ID to edit: ");
            int id = scanner.nextInt();
            scanner.nextLine();
            
            Phone p = storage.findById(id);
            if (p == null) {
                String errorMsg = "Phone with ID " + id + " not found for editing";
                logWriter.println("Error: " + errorMsg);
                System.out.println("Error: " + errorMsg);
                return;
            }
        
            System.out.println("Leave field empty to keep current value.");
            
            System.out.println("Current brand: " + p.getBrand());
            String brand = readOptionalInput("New brand: ", PhoneValidator::isValidBrand,
                "Error: Invalid brand format!");
            if (!brand.isEmpty()) p.setBrand(brand);
        
            System.out.println("Current model: " + p.getModel());
            String model = readOptionalInput("New model: ", PhoneValidator::isValidModel,
                "Error: Invalid model format!");
            if (!model.isEmpty()) p.setModel(model);
        
            System.out.println("Current camera count: " + p.getCameraCount());
            String cams = readOptionalInput("New camera count: ", PhoneValidator::isValidCameraCount,
                "Error: Camera count must be between 0 and 7!");
            if (!cams.isEmpty()) {
                try { 
                    p.setCameraCount(Integer.parseInt(cams)); 
                } catch (NumberFormatException e) { 
                    String errorMsg = "Invalid camera count format in edit: " + cams;
                    logWriter.println("Error: " + errorMsg);
                    System.out.println("Error: " + errorMsg);
                }
            }
        
            System.out.println("Current release date: " + PhoneValidator.formatDate(p.getReleaseDate()));
            String dateStr = readOptionalInput("New date (yyyy-MM-dd): ", PhoneValidator::isValidDate,
                "Error: Invalid date format!");
            if (!dateStr.isEmpty()) {
                try { 
                    p.setReleaseDate(PhoneValidator.parseDate(dateStr)); 
                } catch (Exception e) { 
                    String errorMsg = "Invalid date format in edit: " + dateStr;
                    logWriter.println("Error: " + errorMsg);
                    System.out.println("Error: " + errorMsg);
                }
            }
        
            System.out.println("Current price: " + p.getPrice());
            String priceStr = readOptionalInput("New price: ", PhoneValidator::isValidPrice,
                "Error: Invalid price format!");
            if (!priceStr.isEmpty()) {
                try { 
                    p.setPrice(PhoneValidator.parsePrice(priceStr)); 
                } catch (Exception e) { 
                    String errorMsg = "Invalid price format in edit: " + priceStr;
                    logWriter.println("Error: " + errorMsg);
                    System.out.println("Error: " + errorMsg);
                }
            }
        
            storage.update(id, p);
            System.out.println("Record updated:");
            System.out.println(p);
            
        } catch (Exception e) {
            try (PrintWriter logWriter = new PrintWriter(new FileWriter("errors.log", true))) {
                String errorMsg = "Error in editPhone: " + e.getMessage();
                logWriter.println("Error: " + errorMsg);
                System.err.println("Error: " + errorMsg);
            } catch (IOException ioException) {
                System.err.println("Failed to write to error log: " + ioException.getMessage());
            }
        }
    }

    private String readOptionalInput(String prompt, Predicate<String> validator, String errorMessage) {
        while (true) {
            System.out.print(prompt);
            String input = scanner.nextLine().trim();
            if (input.isEmpty() || validator.test(input)) {
                return input;
            }
            System.out.println(errorMessage);
        }
    }

    private void sortPhones() {
        System.out.println("Choose field for sorting:");
        System.out.println("1. By ID (Comparator) ");
        System.out.println("2. By brand (Collections + Comparator) ");
        System.out.println("3. By model (Collections + Lambda) ");
        System.out.println("4. By camera count (Comparator) ");
        System.out.println("5. By release date (Lambda) ");
        System.out.println("6. By price (Lambda) ");

        int choice = scanner.nextInt();
        scanner.nextLine();
        List<Phone> list = storage.getAll();

        switch (choice) {
            case 1 -> list.sort(Comparator.comparingInt(Phone::getId));
            case 2 -> Collections.sort(list, Comparator.comparing(Phone::getBrand, String.CASE_INSENSITIVE_ORDER));
            case 3 -> Collections.sort(list, (a, b) -> a.getModel().compareToIgnoreCase(b.getModel()));
            case 4 -> list.sort(Comparator.comparingInt(Phone::getCameraCount));
            case 5 -> list.sort((a, b) -> a.getReleaseDate().compareTo(b.getReleaseDate()));
            case 6 -> list.sort((a, b) -> Double.compare(a.getPrice(), b.getPrice()));
            default -> {
                System.out.println("Invalid choice!");
                return;
            }
        }
        System.out.println("Phones sorted:");
        for (Phone phone : list) {
            System.out.println(phone);
        }
    }

    private void demonstrateSortedMap() {
        if (storage instanceof PhoneMapStorage) {
            PhoneMapStorage mapStorage = (PhoneMapStorage) storage;
            var sortedMap = mapStorage.getPhoneMap();
            System.out.println("=== SortedMap Demonstration ===");
            System.out.println("Automatic sorting by key (ID):");
            for (var entry : sortedMap.entrySet()) {
                System.out.println("Key: " + entry.getKey() + " | Value: " + entry.getValue());
            }
            System.out.println("First key: " + sortedMap.firstKey());
            System.out.println("Last key: " + sortedMap.lastKey());
        } else {
            System.out.println("SortedMap is only available with Map-based storage");
        }
    }

    private void saveCurrentFormat() {
        if (currentFileHandler instanceof PhoneFileHandler) {
            currentFileHandler.writeToFile("report.txt", storage.getAll());
        } else if (currentFileHandler instanceof PhoneXmlFileHandler) {
            currentFileHandler.writeToFile("report.xml", storage.getAll());
        } else if (currentFileHandler instanceof PhoneJsonFileHandler) {
            currentFileHandler.writeToFile("report.json", storage.getAll());
        }
    }

    private void changeFileFormat() {
        System.out.println("\nChoose new file format:");
        System.out.println("1 - TXT");
        System.out.println("2 - XML");
        System.out.println("3 - JSON");
        
        int choice = scanner.nextInt();
        scanner.nextLine();
        
        switch (choice) {
            case 1:
                currentFileHandler = new PhoneFileHandler();
                System.out.println("File format changed to TXT");
                break;
            case 2:
                currentFileHandler = xmlFileHandler;
                System.out.println("File format changed to XML");
                break;
            case 3:
                currentFileHandler = jsonFileHandler;
                System.out.println("File format changed to JSON");
                break;
            default:
                System.out.println("Invalid choice, format unchanged");
        }
    }

    private void encryptData() {
        System.out.println("\n--- ENCRYPT DATA ---");
        System.out.println("1. Encrypt phone data");
        System.out.println("2. Decrypt phone data and load to storage");
        System.out.print("Choice: ");
        
        int choice = scanner.nextInt();
        scanner.nextLine();
        
        switch (choice) {
            case 1 -> encryptPhoneData();
            case 2 -> decryptAndLoadPhoneData();
            default -> System.out.println("Invalid choice!");
        }
    }
    
    private void encryptPhoneData() {
        try {
            List<Phone> phones = storage.getAll();
        
            JSONArray jsonArray = new JSONArray();
        
            for (Phone phone : phones) {
                JSONObject jsonObject = new JSONObject();
                jsonObject.put("id", phone.getId());
                jsonObject.put("brand", phone.getBrand());
                jsonObject.put("model", phone.getModel());
                jsonObject.put("cameraCount", phone.getCameraCount());
                jsonObject.put("releaseDate", PhoneValidator.formatDate(phone.getReleaseDate()));
                jsonObject.put("price", phone.getPrice());
                jsonArray.put(jsonObject);
            }
        
            String dataToEncrypt = jsonArray.toString();
        
            String encryptedData = factory.io.EncryptionUtil.encrypt(dataToEncrypt);
        
            try (PrintWriter writer = new PrintWriter("encrypted_data.txt")) {
                writer.print(encryptedData);
            }
        
            System.out.println("Data encrypted and saved to encrypted_data.txt");
            System.out.println("Encrypted " + phones.size() + " phones");
        
        } catch (Exception e) {
            System.err.println("Error encrypting data: " + e.getMessage());
            e.printStackTrace(); 
    }
}

    private void decryptAndLoadPhoneData() {
        try {
            StringBuilder encryptedData = new StringBuilder();
            File encryptedFile = new File("encrypted_data.txt");
            if (!encryptedFile.exists()) {
                System.out.println("Encrypted file not found: encrypted_data.txt");
                return;
            }
        
        try (BufferedReader reader = new BufferedReader(new FileReader(encryptedFile))) {
            String line;
            while ((line = reader.readLine()) != null) {
                encryptedData.append(line);
            }
        }
        
        String decryptedData = factory.io.EncryptionUtil.decrypt(encryptedData.toString());
        System.out.println("Data decrypted successfully!");
        
        JSONArray jsonArray = new JSONArray(decryptedData);
        List<Phone> decryptedPhones = new ArrayList<>();
        
        for (int i = 0; i < jsonArray.length(); i++) {
            try {
                JSONObject jsonObject = jsonArray.getJSONObject(i);
                int id = jsonObject.getInt("id");
                String brand = jsonObject.getString("brand");
                String model = jsonObject.getString("model");
                int cameraCount = jsonObject.getInt("cameraCount");
                String dateStr = jsonObject.getString("releaseDate");
                double price = jsonObject.getDouble("price");
                
                java.util.Date releaseDate = PhoneValidator.parseDate(dateStr);
                Phone phone = new Phone(id, brand, model, cameraCount, releaseDate, price);
                decryptedPhones.add(phone);
                
            } catch (Exception e) {
                System.err.println("Error parsing phone data at index " + i + ": " + e.getMessage());
            }
        }
        
        System.out.println("Parsed " + decryptedPhones.size() + " phones from encrypted data");
        
        System.out.println("\nChoose loading option:");
        System.out.println("1. Replace current storage with decrypted data");
        System.out.println("2. Add decrypted data to current storage (skip duplicates)");
        System.out.println("3. Just show decrypted data without loading");
        System.out.print("Choice: ");
        
        int choice = scanner.nextInt();
        scanner.nextLine();
        
        switch (choice) {
            case 1:
                clearStorage();
                for (Phone phone : decryptedPhones) {
                    storage.add(phone);
                }
                System.out.println("Storage replaced with " + decryptedPhones.size() + " decrypted phones");
                break;
            case 2:
                int addedCount = 0;
                int skippedCount = 0;
                for (Phone phone : decryptedPhones) {
                    if (storage.findById(phone.getId()) == null) {
                        storage.add(phone);
                        addedCount++;
                    } else {
                        skippedCount++;
                    }
                }
                System.out.println("Added " + addedCount + " phones, skipped " + skippedCount + " duplicates");
                break;
            case 3:
                System.out.println("\n--- DECRYPTED PHONES ---");
                for (Phone phone : decryptedPhones) {
                    System.out.println(phone);
                }
                break;
            default:
                System.out.println("Invalid choice, data not loaded");
            }
        } catch (Exception e) {
            System.err.println("Error decrypting and loading data: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private void clearStorage() {
        List<Phone> currentPhones = storage.getAll();
        for (Phone phone : currentPhones) {
            storage.delete(phone.getId());
        }
    }
    
    private void createArchive() {
        System.out.println("\n--- CREATE ARCHIVE ---");
        System.out.println("1. Create ZIP archive");
        System.out.println("2. Create JAR archive");
        System.out.print("Choice: ");
        
        int choice = scanner.nextInt();
        scanner.nextLine();
        
        switch (choice) {
            case 1 -> createZipArchive();
            case 2 -> createJarArchive();
            default -> System.out.println("Invalid choice!");
        }
    }
    
    private void createZipArchive() {
        System.out.print("Enter archive name (without extension): ");
        String archiveName = scanner.nextLine();
        
        String[] filesToArchive = {
            "phones.txt", "phones.xml", "phones.json",
            "report.txt", "report.xml", "report.json",
            "errors.log", "encrypted_data.txt"
        };
        
        List<String> existingFiles = new ArrayList<>();
        for (String file : filesToArchive) {
            File fileObj = new File(file);
            if (fileObj.exists()) {
                existingFiles.add(file);
                System.out.println("Found: " + file);
            }
        }
        
        if (existingFiles.isEmpty()) {
            System.out.println("No files found to archive!");
            return;
        }
        
        System.out.println("Creating archive with " + existingFiles.size() + " files...");
        
        factory.io.ArchiveUtil.createZipArchiveFromFiles(
            existingFiles.toArray(new String[0]), 
            archiveName + ".zip"
        );
    }
    
    private void createJarArchive() {
        System.out.print("Enter JAR name (without extension): ");
        String jarName = scanner.nextLine();
        
        String[] filesToInclude = {
            "phones.txt", "phones.xml", "phones.json",
            "report.txt", "report.xml", "report.json"
        };
        
        List<String> existingFiles = new ArrayList<>();
        for (String file : filesToInclude) {
            File fileObj = new File(file);
            if (fileObj.exists()) {
                existingFiles.add(file);
                System.out.println("Found: " + file);
            }
        }
        
        if (existingFiles.isEmpty()) {
            System.out.println("No files found to archive!");
            return;
        }
        
        System.out.println("Creating JAR with " + existingFiles.size() + " files...");
        
        factory.io.ArchiveUtil.createJarArchive(
            existingFiles.toArray(new String[0]), 
            jarName + ".jar"
        );
    }

    private void demonstrateDecoratorPattern() {
        System.out.println("\n--- DEMONSTRATE DECORATOR PATTERN ---");
        System.out.println("1. Basic writer");
        System.out.println("2. Writer with encryption");
        System.out.print("Choice: ");
        
        int choice = scanner.nextInt();
        scanner.nextLine();
        
        DataWriter writer = new SimpleDataWriter();
        
        switch (choice) {
            case 1:
                break;
            case 2:
                writer = new EncryptionDataWriterDecorator(writer);
                break;
            default:
                System.out.println("Invalid choice!");
                return;
        }
        
        writer.write("decorator_demo.txt", storage.getAll());
        System.out.println("File written with selected decorators!");
    }

    private void demonstrateBuilderPattern() {
        System.out.println("\n--- DEMONSTRATE BUILDER PATTERN ---");
        System.out.println("Building phone using Builder Pattern...");
        
        try {
            System.out.print("Enter ID: ");
            int id = scanner.nextInt();
            scanner.nextLine();
            
            System.out.print("Enter brand: ");
            String brand = scanner.nextLine();
            
            System.out.print("Enter model: ");
            String model = scanner.nextLine();
            
            int cameras = readValidatedInput("Enter camera count (0-7): ", 
                PhoneValidator::isValidCameraCount, "Error: Camera count must be between 0 and 7!", Integer::parseInt);
            
            String dateStr = readValidatedInput("Enter release date (yyyy-MM-dd): ", 
                PhoneValidator::isValidDate, "Error: Date must be in format yyyy-MM-dd!", s -> s);
            var date = PhoneValidator.parseDate(dateStr);
            
            String priceStr = readValidatedInput("Enter price: ", 
                PhoneValidator::isValidPrice, "Error: Price format is invalid!", s -> s);
            double price = PhoneValidator.parsePrice(priceStr);

            Phone phone = Phone.builder()
                    .setId(id)
                    .setBrand(brand)
                    .setModel(model)
                    .setCameraCount(cameras)
                    .setReleaseDate(date)
                    .setPrice(price)
                    .build();
            
            storage.add(phone);
            System.out.println("Phone successfully built and added to storage: " + phone);
            
        } catch (Exception e) {
            System.err.println("Error building phone: " + e.getMessage());
        }
    }
}