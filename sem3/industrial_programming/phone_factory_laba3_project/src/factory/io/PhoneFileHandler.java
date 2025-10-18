package factory.io;

import factory.model.Phone;
import java.io.*;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.regex.Pattern;

public class PhoneFileHandler extends AbstractFileHandler<Phone> {
    private SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");

    private static final Pattern ID_PATTERN = Pattern.compile("^[0-9]+$");
    private static final Pattern BRAND_PATTERN = Pattern.compile("^[A-Za-zА-Яа-я ]+$");
    private static final Pattern MODEL_PATTERN = Pattern.compile("^[A-Za-zА-Яа-я0-9 ]+$");
    private static final Pattern CAMERA_PATTERN = Pattern.compile("^[0-7]{1}$");
    private static final Pattern DATE_PATTERN = Pattern.compile("^\\d{4}-\\d{2}-\\d{2}$");
    private static final Pattern PRICE_PATTERN = Pattern.compile("^\\d{1,5},\\d{2}$");

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
                    logWriter.println("Ошибка: неверное количество полей -> " + line);
                    System.err.println("Ошибка: неверное количество полей -> " + line);
                    continue;
                }

                String idStr = parts[0].trim();
                String brand = parts[1].trim();
                String model = parts[2].trim();
                String camerasStr = parts[3].trim();
                String dateStr = parts[4].trim();
                String priceStr = parts[5].trim();

                try {
                    if (!ID_PATTERN.matcher(idStr).matches()) throw new IllegalArgumentException("Некорректный ID");
                    if (!BRAND_PATTERN.matcher(brand).matches()) throw new IllegalArgumentException("Некорректный бренд");
                    if (!MODEL_PATTERN.matcher(model).matches()) throw new IllegalArgumentException("Некорректная модель");
                    if (!CAMERA_PATTERN.matcher(camerasStr).matches()) throw new IllegalArgumentException("Некорректное число камер");
                    if (!DATE_PATTERN.matcher(dateStr).matches()) throw new IllegalArgumentException("Некорректная дата");
                    if (!PRICE_PATTERN.matcher(priceStr).matches()) throw new IllegalArgumentException("Некорректная цена");

                    int id = Integer.parseInt(idStr);
                    int cameras = Integer.parseInt(camerasStr);
                    Date date = sdf.parse(dateStr);
                    double price = Double.parseDouble(priceStr.replace(",", "."));

                    phones.add(new Phone(id, brand, model, cameras, date, price));

                } catch (Exception e) {
                    logWriter.println("Ошибка: " + e.getMessage() + " | строка: " + line);
                    System.err.println("Ошибка: " + e.getMessage() + " | строка: " + line);
                }
            }

        } catch (IOException e) {
            e.printStackTrace();
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
                        p.getCameraCount(), sdf.format(p.getReleaseDate()), p.getPrice());
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}