package factory.menu;

import factory.io.PhoneFileHandler;
import factory.model.Phone;
import factory.storage.AbstractStorage;
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
            System.out.println("\n===== МЕНЮ =====");
            System.out.println("1. Показать все телефоны");
            System.out.println("2. Добавить телефон");
            System.out.println("3. Удалить телефон");
            System.out.println("4. Сохранить в файл");
            System.out.println("5. Редактировать телефон");
            System.out.println("6. Сортировать телефоны");
            System.out.println("7. Выйти");
            System.out.print("Выбор: ");
            int choice = scanner.nextInt();
            scanner.nextLine();

            switch (choice) {
                case 1 -> storage.printAll();
                case 2 -> addPhone();
                case 3 -> deletePhone();
                case 4 -> fileHandler.writeToFile("report.txt", storage.getAll());
                case 5 -> editPhone();
                case 6 -> sortPhones();
                case 7 -> {
                    System.out.println("Выход из программы...");
                    return;
                }
                default -> System.out.println("Неверный выбор!");
            }
        }
    }

    private void addPhone() {
        try {
            System.out.print("ID: ");
            int id = scanner.nextInt();
            scanner.nextLine();
            System.out.print("Бренд: ");
            String brand = scanner.nextLine();
            System.out.print("Модель: ");
            String model = scanner.nextLine();
            System.out.print("Камер: ");
            int cameras = scanner.nextInt();
            scanner.nextLine();
            System.out.print("Дата выпуска (yyyy-MM-dd): ");
            String dateStr = scanner.nextLine();
            Date date = new SimpleDateFormat("yyyy-MM-dd").parse(dateStr);
            System.out.print("Цена: ");
            double price = scanner.nextDouble();

            Phone phone = new Phone(id, brand, model, cameras, date, price);
            storage.add(phone);
            System.out.println("Телефон успешно добавлен!");
        } catch (Exception e) {
            System.err.println("Ошибка ввода данных! Попробуйте снова.");
            scanner.nextLine();
        }
    }

    private void deletePhone() {
        System.out.print("Введите ID для удаления: ");
        int id = scanner.nextInt();
        storage.delete(id);
        System.out.println("Телефон с ID " + id + " удалён (если существовал).");
    }

    private void sortPhones() {
        System.out.println("Выберите поле для сортировки:");
        System.out.println("1. По ID (Comparator) ");
        System.out.println("2. По бренду (Collections + Comparator) ");
        System.out.println("3. По модели (Collections + Лямбда выражение) ");
        System.out.println("4. По числу камер (Comparator) ");
        System.out.println("5. По дате выпуска (Лямбда выражение) ");
        System.out.println("6. По цене (лямбда выражение) ");

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
                System.out.println("Неверный выбор!");
                return;
            }
        }
        System.out.println("Телефоны отсортированы:");
        for (Phone phone : list) {
            System.out.println(phone);
        }
    }

    private void editPhone() {
        System.out.print("Введите ID телефона для редактирования: ");
        int id = scanner.nextInt();
        scanner.nextLine();
        Phone p = storage.findById(id);
        if (p == null) {
            System.out.println("Телефон с ID " + id + " не найден.");
            return;
        }
    
        System.out.println("Оставьте поле пустым, чтобы не менять значение.");
        System.out.println("Текущий бренд: " + p.getBrand());
        System.out.print("Новый бренд: ");
        String brand = scanner.nextLine().trim();
        if (!brand.isEmpty()) p.setBrand(brand);
    
        System.out.println("Текущая модель: " + p.getModel());
        System.out.print("Новая модель: ");
        String model = scanner.nextLine().trim();
        if (!model.isEmpty()) p.setModel(model);
    
        System.out.println("Текущее число камер: " + p.getCameraCount());
        System.out.print("Новые камеры: ");
        String cams = scanner.nextLine().trim();
        if (!cams.isEmpty()) {
            try { p.setCameraCount(Integer.parseInt(cams)); }
            catch (NumberFormatException e) { System.out.println("Неверный формат числа камер, значение не изменено."); }
        }
    
        System.out.println("Текущая дата выпуска: " + new SimpleDateFormat("yyyy-MM-dd").format(p.getReleaseDate()));
        System.out.print("Новая дата (yyyy-MM-dd): ");
        String dateStr = scanner.nextLine().trim();
        if (!dateStr.isEmpty()) {
            try { p.setReleaseDate(new SimpleDateFormat("yyyy-MM-dd").parse(dateStr)); }
            catch (Exception e) { System.out.println("Неверный формат даты, значение не изменено."); }
        }
    
        System.out.println("Текущая цена: " + p.getPrice());
        System.out.print("Новая цена (формат 12345,67): ");
        String priceStr = scanner.nextLine().trim();
        if (!priceStr.isEmpty()) {
            try { p.setPrice(Double.parseDouble(priceStr.replace(",", "."))); }
            catch (NumberFormatException e) { System.out.println("Неверный формат цены, значение не изменено."); }
        }
    
        storage.update(id, p);
        System.out.println("Запись обновлена:");
        System.out.println(p);
    }
}