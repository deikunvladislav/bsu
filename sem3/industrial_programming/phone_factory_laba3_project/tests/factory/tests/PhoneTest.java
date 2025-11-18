package tests.factory.tests;

import factory.model.Phone;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;
import java.util.Date;
import static org.junit.jupiter.api.Assertions.*;

class PhoneTest {

    @Test
    void testPhoneCreation() {
        Date date = new Date();
        Phone phone = new Phone(1, "TestBrand", "TestModel", 2, date, 299.99);
        assertEquals(1, phone.getId());
        assertEquals("TestBrand", phone.getBrand());
        assertEquals("TestModel", phone.getModel());
        assertEquals(2, phone.getCameraCount());
        assertEquals(date, phone.getReleaseDate());
        assertEquals(299.99, phone.getPrice(), 0.001);
    }

    @ParameterizedTest
    @CsvSource({"1, Samsung, Galaxy S21, 3, 699.99", "2, Apple, iPhone 14, 2, 999.99", "3, Xiaomi, Redmi Note, 4, 299.99", "100, Test, Model, 0, 0.00"})
    void testPhoneParameterizedCreation(int id, String brand, String model, int cameras, double price) {
        Date date = new Date();
        Phone phone = new Phone(id, brand, model, cameras, date, price);
        assertEquals(id, phone.getId());
        assertEquals(brand, phone.getBrand());
        assertEquals(model, phone.getModel());
        assertEquals(cameras, phone.getCameraCount());
        assertEquals(date, phone.getReleaseDate());
        assertEquals(price, phone.getPrice(), 0.001);
    }

    @Test
    void testSetters() {
        Phone phone = new Phone(1, "OldBrand", "OldModel", 1, new Date(), 199.99);
        phone.setBrand("NewBrand");
        phone.setModel("NewModel");
        phone.setCameraCount(3);
        Date newDate = new Date();
        phone.setReleaseDate(newDate);
        phone.setPrice(399.99);
        phone.setId(2);
        assertEquals("NewBrand", phone.getBrand());
        assertEquals("NewModel", phone.getModel());
        assertEquals(3, phone.getCameraCount());
        assertEquals(newDate, phone.getReleaseDate());
        assertEquals(399.99, phone.getPrice(), 0.001);
        assertEquals(2, phone.getId());
    }

    @Test
    void testToStringFormat() {
        Date date = new Date();
        Phone phone = new Phone(1, "Samsung", "Galaxy", 3, date, 599.99);
        String stringRep = phone.toString();
        assertNotNull(stringRep);
        assertTrue(stringRep.contains("Samsung"));
        assertTrue(stringRep.contains("Galaxy"));
        assertTrue(stringRep.contains("3"));
        assertTrue(stringRep.contains("599,99"));
        assertTrue(stringRep.startsWith("ID: 1"));
    }

    @Test
    void testEdgeCases() {
        Phone minPhone = new Phone(1, "A", "B", 0, new Date(0), 0.0);
        assertEquals(1, minPhone.getId());
        assertEquals("A", minPhone.getBrand());
        assertEquals("B", minPhone.getModel());
        assertEquals(0, minPhone.getCameraCount());
        assertEquals(0.0, minPhone.getPrice(), 0.001);
        Phone maxPhone = new Phone(Integer.MAX_VALUE, "MaxBrand", "MaxModel", 7, new Date(), 99999.99);
        assertEquals(Integer.MAX_VALUE, maxPhone.getId());
        assertEquals(7, maxPhone.getCameraCount());
    }

    @Test
    void testNullSafetyInSetters() {
        Phone phone = new Phone(1, "Brand", "Model", 1, new Date(), 100.0);
        assertDoesNotThrow(() -> phone.setBrand(null));
        assertDoesNotThrow(() -> phone.setModel(null));
        assertDoesNotThrow(() -> phone.setReleaseDate(null));
    }
}