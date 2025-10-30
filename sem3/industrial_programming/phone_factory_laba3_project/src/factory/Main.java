package factory;

import factory.io.*;
import factory.menu.Menu;
import factory.storage.AbstractStorage;
import factory.storage.PhoneListStorage;
import factory.storage.PhoneMapStorage;
import factory.model.Phone;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        System.out.println("Choose storage type:");
        System.out.println("1 - List based storage");
        System.out.println("2 - Map based storage");
        int storageChoice = scanner.nextInt();
        
        AbstractStorage<Phone> storage;
        if (storageChoice == 2) {
            storage = new PhoneMapStorage();
        } else {
            storage = new PhoneListStorage();
        }
        
        System.out.println("\nChoose file format for initial data load:");
        System.out.println("1 - TXT (phones.txt)");
        System.out.println("2 - XML (phones.xml)");
        System.out.println("3 - JSON (phones.json)");
        int fileFormatChoice = scanner.nextInt();
        scanner.nextLine();
        
        AbstractFileHandler<Phone> fileHandler = null;
        String filename = "";
        
        switch (fileFormatChoice) {
            case 1:
                fileHandler = new PhoneFileHandler();
                filename = "phones.txt";
                break;
            case 2:
                fileHandler = new PhoneXmlFileHandler();
                filename = "phones.xml";
                break;
            case 3:
                fileHandler = new PhoneJsonFileHandler();
                filename = "phones.json";
                break;
            default:
                System.out.println("Invalid choice, using TXT by default");
                fileHandler = new PhoneFileHandler();
                filename = "phones.txt";
        }
        
        System.out.println("Loading data from " + filename + "...");
        
        java.util.List<Phone> phones = fileHandler.readFromFile(filename);
        for (Phone phone : phones) {
            storage.add(phone);
        }
        
        System.out.println("Successfully loaded " + phones.size() + " phones");
        
        Menu menu = new Menu(storage, fileHandler, new PhoneXmlFileHandler(), new PhoneJsonFileHandler());
        menu.show();
        scanner.close();
    }
}