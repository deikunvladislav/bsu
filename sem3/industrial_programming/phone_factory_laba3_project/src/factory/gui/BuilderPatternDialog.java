package factory.gui;

import factory.model.Phone;
import factory.validation.PhoneValidator;
import javax.swing.*;
import java.awt.*;

public class BuilderPatternDialog extends JDialog {
    private Phone phone;
    private boolean confirmed;
    
    private JTextField idField;
    private JTextField brandField;
    private JTextField modelField;
    private JTextField camerasField;
    private JTextField dateField;
    private JTextField priceField;
    
    public BuilderPatternDialog(JFrame parent) {
        super(parent, "Builder Pattern - Create Phone", true);
        this.confirmed = false;
        
        initializeUI();
    }
    
    private void initializeUI() {
        setLayout(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(5, 5, 5, 5);
        gbc.fill = GridBagConstraints.HORIZONTAL;
        
        gbc.gridx = 0;
        gbc.gridy = 0;
        add(new JLabel("ID:"), gbc);
        
        gbc.gridx = 1;
        idField = new JTextField(20);
        add(idField, gbc);
        
        gbc.gridx = 0;
        gbc.gridy = 1;
        add(new JLabel("Brand:"), gbc);
        
        gbc.gridx = 1;
        brandField = new JTextField(20);
        add(brandField, gbc);
        
        gbc.gridx = 0;
        gbc.gridy = 2;
        add(new JLabel("Model:"), gbc);
        
        gbc.gridx = 1;
        modelField = new JTextField(20);
        add(modelField, gbc);
        
        gbc.gridx = 0;
        gbc.gridy = 3;
        add(new JLabel("Camera Count (0-7):"), gbc);
        
        gbc.gridx = 1;
        camerasField = new JTextField(20);
        add(camerasField, gbc);
        
        gbc.gridx = 0;
        gbc.gridy = 4;
        add(new JLabel("Release Date (yyyy-MM-dd):"), gbc);
        
        gbc.gridx = 1;
        dateField = new JTextField(20);
        add(dateField, gbc);
        
        gbc.gridx = 0;
        gbc.gridy = 5;
        add(new JLabel("Price:"), gbc);
        
        gbc.gridx = 1;
        priceField = new JTextField(20);
        add(priceField, gbc);
        
        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        JButton buildButton = new JButton("Build Phone");
        JButton cancelButton = new JButton("Cancel");
        
        buildButton.addActionListener(e -> buildPhone());
        cancelButton.addActionListener(e -> dispose());
        
        buttonPanel.add(buildButton);
        buttonPanel.add(cancelButton);
        
        gbc.gridx = 0;
        gbc.gridy = 6;
        gbc.gridwidth = 2;
        add(buttonPanel, gbc);
        
        pack();
        setLocationRelativeTo(getParent());
    }
    
    private void buildPhone() {
        try {
            String idStr = idField.getText().trim();
            String brand = brandField.getText().trim();
            String model = modelField.getText().trim();
            String camerasStr = camerasField.getText().trim();
            String dateStr = dateField.getText().trim();
            String priceStr = priceField.getText().trim();
            
            if (!PhoneValidator.isValidId(idStr)) {
                JOptionPane.showMessageDialog(this,
                    "Error: ID must be a positive integer!",
                    "Validation Error",
                    JOptionPane.ERROR_MESSAGE);
                return;
            }
            
            if (!PhoneValidator.isValidBrand(brand)) {
                JOptionPane.showMessageDialog(this,
                    "Error: Brand must contain only letters and spaces (2-50 characters)!",
                    "Validation Error",
                    JOptionPane.ERROR_MESSAGE);
                return;
            }
            
            if (!PhoneValidator.isValidModel(model)) {
                JOptionPane.showMessageDialog(this,
                    "Error: Model format is invalid!",
                    "Validation Error",
                    JOptionPane.ERROR_MESSAGE);
                return;
            }
            
            if (!PhoneValidator.isValidCameraCount(camerasStr)) {
                JOptionPane.showMessageDialog(this,
                    "Error: Camera count must be between 0 and 7!",
                    "Validation Error",
                    JOptionPane.ERROR_MESSAGE);
                return;
            }
            
            if (!PhoneValidator.isValidDate(dateStr)) {
                JOptionPane.showMessageDialog(this,
                    "Error: Date must be in format yyyy-MM-dd!",
                    "Validation Error",
                    JOptionPane.ERROR_MESSAGE);
                return;
            }
            
            if (!PhoneValidator.isValidPrice(priceStr)) {
                JOptionPane.showMessageDialog(this,
                    "Error: Price format is invalid!",
                    "Validation Error",
                    JOptionPane.ERROR_MESSAGE);
                return;
            }
            
            int id = Integer.parseInt(idStr);
            int cameras = Integer.parseInt(camerasStr);
            java.util.Date date = PhoneValidator.parseDate(dateStr);
            double price = PhoneValidator.parsePrice(priceStr);
            
            if (price <= 0) {
                JOptionPane.showMessageDialog(this,
                    "Error: Price must be positive!",
                    "Validation Error",
                    JOptionPane.ERROR_MESSAGE);
                return;
            }
            
            phone = Phone.builder()
                    .setId(id)
                    .setBrand(brand)
                    .setModel(model)
                    .setCameraCount(cameras)
                    .setReleaseDate(date)
                    .setPrice(price)
                    .build();
            
            confirmed = true;
            dispose();
            
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this,
                "Error building phone: " + e.getMessage(),
                "Error",
                JOptionPane.ERROR_MESSAGE);
        }
    }
    
    public Phone getPhone() {
        return phone;
    }
    
    public boolean isConfirmed() {
        return confirmed;
    }
}