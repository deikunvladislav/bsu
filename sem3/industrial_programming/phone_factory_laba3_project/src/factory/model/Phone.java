package factory.model;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.io.Serializable;

public class Phone extends AbstractEntity implements Serializable {
    private static final long serialVersionUID = 1L;
    
    private int id;
    private String brand;
    private String model;
    private int cameraCount;
    private Date releaseDate;
    private double price;

    public Phone(int id, String brand, String model, int cameraCount, Date releaseDate, double price) {
        this.id = id;
        this.brand = brand;
        this.model = model;
        this.cameraCount = cameraCount;
        this.releaseDate = releaseDate;
        this.price = price;
    }

    public static PhoneBuilder builder() {
        return new PhoneBuilder();
    }

    @Override
    public int getId() { return id; }
    @Override
    public void setId(int id) { this.id = id; }

    public String getBrand() { return brand; }
    public void setBrand(String brand) { this.brand = brand; }

    public String getModel() { return model; }
    public void setModel(String model) { this.model = model; }

    public int getCameraCount() { return cameraCount; }
    public void setCameraCount(int cameraCount) { this.cameraCount = cameraCount; }

    public Date getReleaseDate() { return releaseDate; }
    public void setReleaseDate(Date releaseDate) { this.releaseDate = releaseDate; }

    public double getPrice() { return price; }
    public void setPrice(double price) { this.price = price; }

    @Override
    public String toString() {
        return String.format("ID: %d | %s %s | Cameras: %d | Date: %s | Price: %.2f",
                id, brand, model, cameraCount,
                new SimpleDateFormat("yyyy-MM-dd").format(releaseDate), price);
    }
}