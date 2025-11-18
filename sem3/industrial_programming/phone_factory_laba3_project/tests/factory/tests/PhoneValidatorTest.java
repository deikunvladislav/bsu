package tests.factory.tests;

import factory.validation.PhoneValidator;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.ValueSource;
import org.junit.jupiter.params.provider.CsvSource;
import java.text.ParseException;
import java.util.Date;
import static org.junit.jupiter.api.Assertions.*;

class PhoneValidatorTest {

    @ParameterizedTest
    @ValueSource(strings = {"1", "123", "999", "1000000"})
    void testValidId(String id) {
        assertTrue(PhoneValidator.isValidId(id));
    }

    @ParameterizedTest
    @ValueSource(strings = {"0", "-1", "01", "abc", "12a", "", "  "})
    void testInvalidId(String id) {
        assertFalse(PhoneValidator.isValidId(id));
    }

    @ParameterizedTest
    @ValueSource(strings = {"Sa", "Samsung", "Apple", "Самсунг", "Xiaomi", "Huawei", "Nokia"})
    void testValidBrand(String brand) {
        assertTrue(PhoneValidator.isValidBrand(brand));
    }

    @ParameterizedTest
    @ValueSource(strings = {"A", "", "   ", "S", "Brand@123", "Samsung123"})
    void testInvalidBrand(String brand) {
        assertFalse(PhoneValidator.isValidBrand(brand));
    }

    @Test
    void testBrandBoundaryCases() {
        assertTrue(PhoneValidator.isValidBrand("Ab"));
        StringBuilder maxBrand = new StringBuilder();
        for (int i = 0; i < 50; i++) maxBrand.append("A");
        assertTrue(PhoneValidator.isValidBrand(maxBrand.toString()));
        StringBuilder tooLongBrand = new StringBuilder();
        for (int i = 0; i < 51; i++) tooLongBrand.append("A");
        assertFalse(PhoneValidator.isValidBrand(tooLongBrand.toString()));
        assertFalse(PhoneValidator.isValidBrand("A"));
        assertTrue(PhoneValidator.isValidBrand("Ab"));
        assertTrue(PhoneValidator.isValidBrand("Abc"));
    }

    @ParameterizedTest
    @ValueSource(strings = {"S21", "iPhone 13", "Redmi Note 10", "Galaxy S22 Ultra", "P40 Pro", "A"})
    void testValidModel(String model) {
        assertTrue(PhoneValidator.isValidModel(model));
    }

    @ParameterizedTest
    @ValueSource(strings = {"", "   ", "Model@123", "Mod#el"})
    void testInvalidModel(String model) {
        assertFalse(PhoneValidator.isValidModel(model));
    }

    @Test
    void testModelBoundaryCases() {
        assertTrue(PhoneValidator.isValidModel("A"));
        StringBuilder maxModel = new StringBuilder();
        for (int i = 0; i < 50; i++) maxModel.append("M");
        assertTrue(PhoneValidator.isValidModel(maxModel.toString()));
        StringBuilder tooLongModel = new StringBuilder();
        for (int i = 0; i < 51; i++) tooLongModel.append("M");
        assertFalse(PhoneValidator.isValidModel(tooLongModel.toString()));
    }

    @ParameterizedTest
    @ValueSource(strings = {"0", "1", "2", "3", "4", "5", "6", "7"})
    void testValidCameraCount(String cameras) {
        assertTrue(PhoneValidator.isValidCameraCount(cameras));
    }

    @ParameterizedTest
    @ValueSource(strings = {"-1", "8", "9", "10", "a", "1.5", ""})
    void testInvalidCameraCount(String cameras) {
        assertFalse(PhoneValidator.isValidCameraCount(cameras));
    }

    @Test
    void testCameraBoundaryCases() {
        assertTrue(PhoneValidator.isValidCameraCount("0"));
        assertTrue(PhoneValidator.isValidCameraCount("7"));
        assertFalse(PhoneValidator.isValidCameraCount("-1"));
        assertFalse(PhoneValidator.isValidCameraCount("8"));
    }

    @ParameterizedTest
    @ValueSource(strings = {"1980-01-01", "2000-06-15", "2023-12-31", "2024-02-29"})
    void testValidDate(String date) {
        assertTrue(PhoneValidator.isValidDate(date));
    }

    @ParameterizedTest
    @ValueSource(strings = {"1979-12-31", "2027-01-01", "2023-02-29", "2023-13-01", "2023-00-01", "2023-01-32", "invalid-date", "2023/01/01", "", "   "})
    void testInvalidDate(String date) {
        assertFalse(PhoneValidator.isValidDate(date));
    }

    @Test
    void testDateBoundaryCases() {
        assertTrue(PhoneValidator.isValidDate("1980-01-01"));
        assertFalse(PhoneValidator.isValidDate("1979-12-31"));
        assertFalse(PhoneValidator.isValidDate("2027-01-01"));
        assertFalse(PhoneValidator.isValidDate("2023-02-29"));
        assertFalse(PhoneValidator.isValidDate("2023-04-31"));
        Date currentDate = new Date();
        String currentDateStr = PhoneValidator.formatDate(currentDate);
        assertTrue(PhoneValidator.isValidDate(currentDateStr));
    }

    @ParameterizedTest
    @ValueSource(strings = {"0", "0.00", "0,00", "100", "999.99", "999,99", "12345", "123.45", "123,45"})
    void testValidPrice(String price) {
        assertTrue(PhoneValidator.isValidPrice(price));
    }

    @ParameterizedTest
    @ValueSource(strings = {"-1", "-0.01", "100000", "999.999", "abc", "12.3.4", "", "   "})
    void testInvalidPrice(String price) {
        assertFalse(PhoneValidator.isValidPrice(price));
    }

    @Test
    void testPriceBoundaryCases() {
        assertTrue(PhoneValidator.isValidPrice("0"));
        assertTrue(PhoneValidator.isValidPrice("0.00"));
        assertTrue(PhoneValidator.isValidPrice("99999.99"));
        assertFalse(PhoneValidator.isValidPrice("100000"));
        assertFalse(PhoneValidator.isValidPrice("-0.01"));
        assertFalse(PhoneValidator.isValidPrice("999.999"));
    }

    @Test
    void testParseDate() throws ParseException {
        Date date = PhoneValidator.parseDate("2023-01-15");
        assertNotNull(date);
        String formatted = PhoneValidator.formatDate(date);
        assertEquals("2023-01-15", formatted);
    }

    @ParameterizedTest
    @CsvSource({"99.99, 99.99", "99,99, 99.99", "0, 0.0", "123.45, 123.45"})
    void testParsePrice(String input, double expected) {
        assertEquals(expected, PhoneValidator.parsePrice(input), 0.001);
    }

    @Test
    void testFormatDate() throws ParseException {
        Date date = PhoneValidator.parseDate("2023-01-15");
        String formatted = PhoneValidator.formatDate(date);
        assertEquals("2023-01-15", formatted);
    }

    @Test
    void testParseInvalidDate() {
        assertThrows(ParseException.class, () -> PhoneValidator.parseDate("invalid-date"));
    }

    @Test
    void testEdgeCaseBrands() {
        assertFalse(PhoneValidator.isValidBrand("  "));
        assertFalse(PhoneValidator.isValidBrand(""));
        
        assertFalse(PhoneValidator.isValidBrand("A"));
        assertFalse(PhoneValidator.isValidBrand(" A"));
        assertFalse(PhoneValidator.isValidBrand("A "));
        
        assertTrue(PhoneValidator.isValidBrand("Ab"));
        assertTrue(PhoneValidator.isValidBrand("A b"));
        assertTrue(PhoneValidator.isValidBrand("Valid Brand"));
        
        StringBuilder maxBrand = new StringBuilder();
        for (int i = 0; i < 50; i++) maxBrand.append("A");
        assertTrue(PhoneValidator.isValidBrand(maxBrand.toString()));
        
        StringBuilder tooLongBrand = new StringBuilder();
        for (int i = 0; i < 51; i++) tooLongBrand.append("A");
        assertFalse(PhoneValidator.isValidBrand(tooLongBrand.toString()));
    }

    @Test
    void testEdgeCaseModels() {
        assertTrue(PhoneValidator.isValidModel("Galaxy S21 Ultra 5G"));
        assertTrue(PhoneValidator.isValidModel("iPhone 13 Pro Max"));
        assertFalse(PhoneValidator.isValidModel("  "));
        assertTrue(PhoneValidator.isValidModel(" 123"));
    }

    @Test
    void testEdgeCasePrices() {
        assertTrue(PhoneValidator.isValidPrice("0.01"));
        assertTrue(PhoneValidator.isValidPrice("99999.99"));
        assertFalse(PhoneValidator.isValidPrice("100000.00"));
        assertFalse(PhoneValidator.isValidPrice("99999.999"));
        assertTrue(PhoneValidator.isValidPrice("1"));
        assertTrue(PhoneValidator.isValidPrice("1.0"));
        assertTrue(PhoneValidator.isValidPrice("1.00"));
        assertTrue(PhoneValidator.isValidPrice("1,0"));
        assertTrue(PhoneValidator.isValidPrice("1,00"));
    }
}