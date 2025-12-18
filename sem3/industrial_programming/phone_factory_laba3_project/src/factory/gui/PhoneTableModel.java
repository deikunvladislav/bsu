package factory.gui;

import factory.model.Phone;
import javax.swing.table.AbstractTableModel;
import java.text.SimpleDateFormat;
import java.util.List;

public class PhoneTableModel extends AbstractTableModel {
    private final String[] columnNames = {"ID", "Brand", "Model", "Cameras", "Release Date", "Price"};
    private List<Phone> phones;
    private final SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
    
    public PhoneTableModel(List<Phone> phones) {
        this.phones = phones;
    }
    
    @Override
    public int getRowCount() {
        return phones.size();
    }
    
    @Override
    public int getColumnCount() {
        return columnNames.length;
    }
    
    @Override
    public String getColumnName(int column) {
        return columnNames[column];
    }
    
    @Override
    public Object getValueAt(int rowIndex, int columnIndex) {
        Phone phone = phones.get(rowIndex);
        switch (columnIndex) {
            case 0: return phone.getId();
            case 1: return phone.getBrand();
            case 2: return phone.getModel();
            case 3: return phone.getCameraCount();
            case 4: return dateFormat.format(phone.getReleaseDate());
            case 5: return String.format("%.2f", phone.getPrice());
            default: return null;
        }
    }
    
    public void setPhones(List<Phone> phones) {
        this.phones = phones;
    }
    
    public Phone getPhoneAt(int row) {
        return phones.get(row);
    }
    
    @Override
    public Class<?> getColumnClass(int columnIndex) {
        switch (columnIndex) {
            case 0: return Integer.class;
            case 1: return String.class;
            case 2: return String.class;
            case 3: return Integer.class;
            case 4: return String.class;
            case 5: return String.class;
            default: return Object.class;
        }
    }
}