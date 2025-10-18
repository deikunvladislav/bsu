package factory;

import factory.io.PhoneFileHandler;
import factory.menu.Menu;
import factory.storage.AbstractStorage;
import factory.storage.PhoneListStorage;
import factory.storage.PhoneMapStorage;
import factory.model.Phone;

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        System.out.println("Выберите тип хранилища:");
        System.out.println("1 - List based storage");
        System.out.println("2 - Map based storage");
        
        Scanner scanner = new Scanner(System.in);
        int choice = scanner.nextInt();
        
        AbstractStorage<Phone> storage;
        if (choice == 2) {
            storage = new PhoneMapStorage();
        } else {
            storage = new PhoneListStorage();
        }
        
        PhoneFileHandler fileHandler = new PhoneFileHandler();

        loadDataFromFile(storage, fileHandler, "phones.txt");

        Menu menu = new Menu(storage, fileHandler);
        menu.show();
    }
    
    private static void loadDataFromFile(AbstractStorage<Phone> storage, 
                                       PhoneFileHandler fileHandler, 
                                       String filename) {
        java.util.List<Phone> phones = fileHandler.readFromFile(filename);
        for (Phone phone : phones) {
            storage.add(phone);
        }
    }
}