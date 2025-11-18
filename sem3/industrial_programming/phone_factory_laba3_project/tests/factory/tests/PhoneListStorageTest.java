package tests.factory.tests;

import factory.model.Phone;
import factory.storage.PhoneListStorage;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import java.util.Date;
import java.util.List;
import static org.junit.jupiter.api.Assertions.*;

class PhoneListStorageTest {
    private PhoneListStorage storage;
    private Phone testPhone;

    @BeforeEach
    void setUp() {
        storage = new PhoneListStorage();
        testPhone = new Phone(1, "Samsung", "Galaxy S21", 3, new Date(), 699.99);
    }

    @Test
    void testAddPhone() {
        storage.add(testPhone);
        assertEquals(1, storage.getAll().size());
        assertTrue(storage.getAll().contains(testPhone));
    }

    @Test
    void testAddMultiplePhones() {
        Phone phone2 = new Phone(2, "Apple", "iPhone 14", 2, new Date(), 999.99);
        storage.add(testPhone);
        storage.add(phone2);
        assertEquals(2, storage.getAll().size());
    }

    @Test
    void testFindById() {
        storage.add(testPhone);
        Phone found = storage.findById(1);
        assertNotNull(found);
        assertEquals(testPhone, found);
    }

    @Test
    void testFindByIdNotFound() {
        assertNull(storage.findById(999));
    }

    @Test
    void testUpdatePhone() {
        storage.add(testPhone);
        Phone updatedPhone = new Phone(1, "Samsung", "Galaxy S22", 4, new Date(), 799.99);
        storage.update(1, updatedPhone);
        Phone found = storage.findById(1);
        assertEquals("Galaxy S22", found.getModel());
        assertEquals(4, found.getCameraCount());
        assertEquals(799.99, found.getPrice());
    }

    @Test
    void testUpdateNonExistentPhone() {
        Phone updatedPhone = new Phone(999, "Brand", "Model", 1, new Date(), 100.0);
        assertDoesNotThrow(() -> storage.update(999, updatedPhone));
    }

    @Test
    void testDeletePhone() {
        storage.add(testPhone);
        assertNotNull(storage.findById(1));
        storage.delete(1);
        assertNull(storage.findById(1));
        assertEquals(0, storage.getAll().size());
    }

    @Test
    void testDeleteNonExistentPhone() {
        assertDoesNotThrow(() -> storage.delete(999));
        assertEquals(0, storage.getAll().size());
    }

    @Test
    void testGetAllEmpty() {
        List<Phone> phones = storage.getAll();
        assertNotNull(phones);
        assertTrue(phones.isEmpty());
    }

    @Test
    void testGetAllWithData() {
        storage.add(testPhone);
        Phone phone2 = new Phone(2, "Apple", "iPhone 14", 2, new Date(), 999.99);
        storage.add(phone2);
        List<Phone> phones = storage.getAll();
        assertEquals(2, phones.size());
        assertTrue(phones.contains(testPhone));
        assertTrue(phones.contains(phone2));
    }

    @Test
    void testPrintAll() {
        storage.add(testPhone);
        assertDoesNotThrow(() -> storage.printAll());
    }

    @Test
    void testPrintAllEmpty() {
        assertDoesNotThrow(() -> storage.printAll());
    }
}