package factory.validation;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Calendar;
import java.util.regex.Pattern;

public class PhoneValidator {
    
    private static final Pattern ID_PATTERN = Pattern.compile("^[1-9]\\d*$");
    private static final Pattern BRAND_PATTERN = Pattern.compile("^(?=(.*[A-Za-zА-Яа-я]){2})[A-Za-zА-Яа-я ]{2,50}$");
    private static final Pattern MODEL_PATTERN = Pattern.compile("^(?!\\s*$)[A-Za-zА-Яа-я0-9 ]{1,50}$");
    private static final Pattern CAMERA_PATTERN = Pattern.compile("^[0-7]$");
    private static final Pattern PRICE_PATTERN = Pattern.compile("^\\d{1,5}([.,]\\d{1,2})?$");
    
    private static final SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
    
    static {
        sdf.setLenient(false);
    }
    
    public static boolean isValidId(String idStr) {
        if (idStr == null || idStr.trim().isEmpty()) {
            return false;
        }
        return ID_PATTERN.matcher(idStr).matches();
    }
    
    public static boolean isValidBrand(String brand) {
        if (brand == null || brand.trim().isEmpty()) {
            return false;
        }
        return BRAND_PATTERN.matcher(brand).matches();
    }
    
    public static boolean isValidModel(String model) {
        if (model == null || model.trim().isEmpty()) {
            return false;
        }
        return MODEL_PATTERN.matcher(model).matches();
    }
    
    public static boolean isValidCameraCount(String camerasStr) {
        if (camerasStr == null || camerasStr.trim().isEmpty()) {
            return false;
        }
        return CAMERA_PATTERN.matcher(camerasStr).matches();
    }
    
    public static boolean isValidDate(String dateStr) {
        if (dateStr == null || dateStr.trim().isEmpty()) {
            return false;
        }
        
        try {
            Date date = sdf.parse(dateStr.trim());
            
            String normalized = sdf.format(date);
            if (!normalized.equals(dateStr.trim())) {
                return false;
            }
            
            Date currentDate = new Date();
            if (date.after(currentDate)) {
                return false;
            }
            
            Calendar cal = Calendar.getInstance();
            cal.setTime(date);
            int year = cal.get(Calendar.YEAR);
            if (year < 1980 || year > 2026) {
                return false;
            }
            return true;
            
        } catch (ParseException e) {
            return false;
        }
    }
    
    public static boolean isValidPrice(String priceStr) {
        if (priceStr == null || priceStr.trim().isEmpty()) {
            return false;
        }
        return PRICE_PATTERN.matcher(priceStr.replace(",", ".")).matches();
    }
    
    public static Date parseDate(String dateStr) throws ParseException {
        return sdf.parse(dateStr);
    }
    
    public static double parsePrice(String priceStr) {
        return Double.parseDouble(priceStr.replace(",", "."));
    }
    
    public static String formatDate(Date date) {
        return sdf.format(date);
    }
}