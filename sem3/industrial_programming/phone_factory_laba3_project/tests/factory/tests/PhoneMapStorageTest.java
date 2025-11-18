package tests.factory.tests;

import factory.model.Phone;
import factory.storage.PhoneMapStorage;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import java.util.Date;
import java.util.SortedMap;
import static org.junit.jupiter.api.Assertions.*;

class PhoneMapStorageTest {
    private PhoneMapStorage storage;
    private Phone testPhone;

    @BeforeEach
    void setUp() {
        storage = new PhoneMapStorage();
        testPhone = new Phone(1, "Samsung", "Galaxy S21", 3, new Date(), 699.99);
    }

    @Test
    void testAddPhone() {
        storage.add(testPhone);
        assertEquals(1, storage.getAll().size());
        assertEquals(testPhone, storage.findById(1));
    }

    @Test
    void testAddDuplicateId() {
        storage.add(testPhone);
        Phone duplicatePhone = new Phone(1, "Apple", "iPhone", 2, new Date(), 899.99);
        storage.add(duplicatePhone);
        assertEquals(1, storage.getAll().size());
        assertEquals("Apple", storage.findById(1).getBrand());
    }

    @Test
    void testGetPhoneMap() {
        storage.add(testPhone);
        SortedMap<Integer, Phone> phoneMap = storage.getPhoneMap();
        assertNotNull(phoneMap);
        assertEquals(1, phoneMap.size());
        assertTrue(phoneMap.containsKey(1));
        assertEquals(testPhone, phoneMap.get(1));
    }

    @Test
    void testPhoneMapIsSorted() {
        Phone phone1 = new Phone(3, "Brand1", "Model1", 1, new Date(), 100.0);
        Phone phone2 = new Phone(1, "Brand2", "Model2", 2, new Date(), 200.0);
        Phone phone3 = new Phone(2, "Brand3", "Model3", 3, new Date(), 300.0);
        storage.add(phone1);
        storage.add(phone2);
        storage.add(phone3);
        SortedMap<Integer, Phone> phoneMap = storage.getPhoneMap();
        assertEquals(1, phoneMap.firstKey());
        assertEquals(3, phoneMap.lastKey());
        Integer[] keys = phoneMap.keySet().toArray(new Integer[0]);
        assertArrayEquals(new Integer[]{1, 2, 3}, keys);
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
    }

    @Test
    void testDeletePhone() {
        storage.add(testPhone);
        storage.delete(1);
        assertNull(storage.findById(1));
    }

    @Test
    void testGetAll() {
        storage.add(testPhone);
        assertEquals(1, storage.getAll().size());
    }

    @Test
    void testPrintAll() {
        storage.add(testPhone);
        assertDoesNotThrow(() -> storage.printAll());
    }
}