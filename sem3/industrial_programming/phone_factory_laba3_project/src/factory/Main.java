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
        System.out.println("Choose storage type:");
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

        java.util.List<Phone> phones = fileHandler.readFromFile("phones.txt");
        for (Phone phone : phones) {
            storage.add(phone);
        }

        Menu menu = new Menu(storage, fileHandler);
        menu.show();
        scanner.close();
    }
}