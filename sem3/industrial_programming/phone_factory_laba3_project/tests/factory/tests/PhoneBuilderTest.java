package tests.factory.tests;

import factory.model.Phone;
import factory.model.PhoneBuilder;
import org.junit.jupiter.api.Test;
import java.util.Date;
import static org.junit.jupiter.api.Assertions.*;

class PhoneBuilderTest {

    @Test
    void testPhoneBuilder() {
        Date date = new Date();
        Phone phone = Phone.builder()
                .setId(1)
                .setBrand("Samsung")
                .setModel("Galaxy S21")
                .setCameraCount(3)
                .setReleaseDate(date)
                .setPrice(699.99)
                .build();

        assertEquals(1, phone.getId());
        assertEquals("Samsung", phone.getBrand());
        assertEquals("Galaxy S21", phone.getModel());
        assertEquals(3, phone.getCameraCount());
        assertEquals(date, phone.getReleaseDate());
        assertEquals(699.99, phone.getPrice(), 0.001);
    }

    @Test
    void testPhoneBuilderValidation() {
        assertThrows(IllegalArgumentException.class, () -> {
            Phone.builder()
                    .setId(0)
                    .setBrand("Test")
                    .setModel("Model")
                    .setCameraCount(2)
                    .setReleaseDate(new Date())
                    .setPrice(100.0)
                    .build();
        });
    }

    @Test
    void testPhoneBuilderEdgeCases() {
        Date date = new Date();

        Phone minPhone = Phone.builder()
                .setId(1)
                .setBrand("Ab")
                .setModel("A")
                .setCameraCount(0)
                .setReleaseDate(date)
                .setPrice(0.01)
                .build();
                
        assertEquals(1, minPhone.getId());
        assertEquals("Ab", minPhone.getBrand());
        assertEquals(0, minPhone.getCameraCount());
        assertEquals(0.01, minPhone.getPrice(), 0.001);

        Phone maxPhone = Phone.builder()
                .setId(Integer.MAX_VALUE)
                .setBrand("VeryLongBrandNameThatIsExactlyFiftyCharactersLong")
                .setModel("VeryLongModelNameThatIsExactlyFiftyChars")
                .setCameraCount(7)
                .setReleaseDate(date)
                .setPrice(99999.99)
                .build();
                
        assertEquals(Integer.MAX_VALUE, maxPhone.getId());
        assertEquals(7, maxPhone.getCameraCount());
        assertEquals(99999.99, maxPhone.getPrice(), 0.001);
    }

    @Test
    void testPhoneBuilderMethodChaining() {
        Date date = new Date();
        
        PhoneBuilder builder = Phone.builder()
                .setId(1)
                .setBrand("Test")
                .setModel("Model")
                .setCameraCount(2)
                .setReleaseDate(date)
                .setPrice(100.0);
                
        assertNotNull(builder);
        
        Phone phone = builder.build();
        assertNotNull(phone);
        assertEquals("Test", phone.getBrand());
    }
}