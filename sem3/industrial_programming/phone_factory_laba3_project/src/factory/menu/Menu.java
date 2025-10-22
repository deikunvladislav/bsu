package factory.menu;

import factory.io.PhoneFileHandler;
import factory.model.Phone;
import factory.storage.AbstractStorage;
import factory.storage.PhoneMapStorage;
import java.text.SimpleDateFormat;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.List;
import java.util.Scanner;

public class Menu {
    private Scanner scanner = new Scanner(System.in);
    private AbstractStorage<Phone> storage;
    private PhoneFileHandler fileHandler;

    public Menu(AbstractStorage<Phone> storage, PhoneFileHandler fileHandler) {
        this.storage = storage;
        this.fileHandler = fileHandler;
    }

    public void show() {
        while (true) {
            System.out.println("\n===== MENU =====");
            System.out.println("1. Show all phones");
            System.out.println("2. Add phone");
            System.out.println("3. Delete phone");
            System.out.println("4. Save to file");
            System.out.println("5. Edit phone");
            System.out.println("6. Sort phones");
            System.out.println("7. Demonstrate SortedMap");
            System.out.println("8. Exit");
            System.out.print("Choice: ");
            int choice = scanner.nextInt();
            scanner.nextLine();

            switch (choice) {
                case 1 -> storage.printAll();
                case 2 -> addPhone();
                case 3 -> deletePhone();
                case 4 -> fileHandler.writeToFile("report.txt", storage.getAll());
                case 5 -> editPhone();
                case 6 -> sortPhones();
                case 7 -> demonstrateSortedMap();
                case 8 -> {
                    System.out.println("Exiting program...");
                    return;
                }
                default -> System.out.println("Invalid choice!");
            }
        }
    }

    private void addPhone() {
        try {
            System.out.print("ID: ");
            int id = scanner.nextInt();
            scanner.nextLine();
            System.out.print("Brand: ");
            String brand = scanner.nextLine();
            System.out.print("Model: ");
            String model = scanner.nextLine();
            System.out.print("Cameras: ");
            int cameras = scanner.nextInt();
            scanner.nextLine();
            System.out.print("Release date (yyyy-MM-dd): ");
            String dateStr = scanner.nextLine();
            Date date = new SimpleDateFormat("yyyy-MM-dd").parse(dateStr);
            System.out.print("Price: ");
            double price = scanner.nextDouble();

            Phone phone = new Phone(id, brand, model, cameras, date, price);
            storage.add(phone);
            System.out.println("Phone successfully added!");
        } catch (Exception e) {
            System.err.println("Data input error! Try again.");
            scanner.nextLine();
        }
    }

    private void deletePhone() {
        System.out.print("Enter ID to delete: ");
        int id = scanner.nextInt();
        storage.delete(id);
        System.out.println("Phone with ID " + id + " deleted (if existed).");
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

    private void editPhone() {
        System.out.print("Enter phone ID to edit: ");
        int id = scanner.nextInt();
        scanner.nextLine();
        Phone p = storage.findById(id);
        if (p == null) {
            System.out.println("Phone with ID " + id + " not found.");
            return;
        }
    
        System.out.println("Leave field empty to keep current value.");
        System.out.println("Current brand: " + p.getBrand());
        System.out.print("New brand: ");
        String brand = scanner.nextLine().trim();
        if (!brand.isEmpty()) p.setBrand(brand);
    
        System.out.println("Current model: " + p.getModel());
        System.out.print("New model: ");
        String model = scanner.nextLine().trim();
        if (!model.isEmpty()) p.setModel(model);
    
        System.out.println("Current camera count: " + p.getCameraCount());
        System.out.print("New camera count: ");
        String cams = scanner.nextLine().trim();
        if (!cams.isEmpty()) {
            try { p.setCameraCount(Integer.parseInt(cams)); }
            catch (NumberFormatException e) { System.out.println("Invalid camera count format, value not changed."); }
        }
    
        System.out.println("Current release date: " + new SimpleDateFormat("yyyy-MM-dd").format(p.getReleaseDate()));
        System.out.print("New date (yyyy-MM-dd): ");
        String dateStr = scanner.nextLine().trim();
        if (!dateStr.isEmpty()) {
            try { p.setReleaseDate(new SimpleDateFormat("yyyy-MM-dd").parse(dateStr)); }
            catch (Exception e) { System.out.println("Invalid date format, value not changed."); }
        }
    
        System.out.println("Current price: " + p.getPrice());
        System.out.print("New price (format 12345.67): ");
        String priceStr = scanner.nextLine().trim();
        if (!priceStr.isEmpty()) {
            try { p.setPrice(Double.parseDouble(priceStr.replace(",", "."))); }
            catch (NumberFormatException e) { System.out.println("Invalid price format, value not changed."); }
        }
    
        storage.update(id, p);
        System.out.println("Record updated:");
        System.out.println(p);
    }
}