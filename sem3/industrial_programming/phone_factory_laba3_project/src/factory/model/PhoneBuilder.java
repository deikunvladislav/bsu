package factory.model;

import java.util.Date;

public class PhoneBuilder {
    private int id;
    private String brand;
    private String model;
    private int cameraCount;
    private Date releaseDate;
    private double price;

    public PhoneBuilder setId(int id) {
        this.id = id;
        return this;
    }

    public PhoneBuilder setBrand(String brand) {
        this.brand = brand;
        return this;
    }

    public PhoneBuilder setModel(String model) {
        this.model = model;
        return this;
    }

    public PhoneBuilder setCameraCount(int cameraCount) {
        this.cameraCount = cameraCount;
        return this;
    }

    public PhoneBuilder setReleaseDate(Date releaseDate) {
        this.releaseDate = releaseDate;
        return this;
    }

    public PhoneBuilder setPrice(double price) {
        this.price = price;
        return this;
    }

    public Phone build() {
        validate();
        return new Phone(id, brand, model, cameraCount, releaseDate, price);
    }

    private void validate() {
        if (id <= 0) {
            throw new IllegalArgumentException("ID must be positive");
        }
        if (brand == null || brand.trim().isEmpty()) {
            throw new IllegalArgumentException("Brand cannot be empty");
        }
        if (model == null || model.trim().isEmpty()) {
            throw new IllegalArgumentException("Model cannot be empty");
        }
        if (cameraCount < 0 || cameraCount > 7) {
            throw new IllegalArgumentException("Camera count must be between 0 and 7");
        }
        if (releaseDate == null) {
            throw new IllegalArgumentException("Release date cannot be null");
        }
        if (price < 0) {
            throw new IllegalArgumentException("Price cannot be negative");
        }
    }
}