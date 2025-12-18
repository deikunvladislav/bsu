package factory.gui.components;

import factory.gui.MainFrame;
import javax.swing.*;
import java.awt.*;

public class ButtonPanel extends JPanel {
    private final MainFrame mainFrame;
    private JComboBox<String> sortComboBox;
    private JTextField searchField;
    
    public ButtonPanel(MainFrame mainFrame) {
        this.mainFrame = mainFrame;
        initializeUI();
    }
    
    private void initializeUI() {
        setLayout(new FlowLayout(FlowLayout.LEFT));
        
        JButton addButton = new JButton("Add Phone");
        JButton editButton = new JButton("Edit");
        JButton deleteButton = new JButton("Delete");
        JButton refreshButton = new JButton("Refresh");
        JButton builderButton = new JButton("Builder Pattern");
        
        String[] sortOptions = {"ID", "Brand", "Model", "Price", "Release Date", "Camera Count"};
        sortComboBox = new JComboBox<>(sortOptions);
        
        searchField = new JTextField(20);
        JButton searchButton = new JButton("Search");
        
        addButton.addActionListener(e -> mainFrame.showAddPhoneDialog());
        editButton.addActionListener(e -> mainFrame.editSelectedPhone());
        deleteButton.addActionListener(e -> mainFrame.deleteSelectedPhone());
        refreshButton.addActionListener(e -> mainFrame.refreshTable());
        builderButton.addActionListener(e -> mainFrame.showBuilderDialog());
        sortComboBox.addActionListener(e -> mainFrame.sortTable((String) sortComboBox.getSelectedItem()));
        searchButton.addActionListener(e -> mainFrame.searchPhones(searchField.getText()));
        
        add(addButton);
        add(editButton);
        add(deleteButton);
        add(refreshButton);
        add(builderButton);
        add(new JLabel("Sort by:"));
        add(sortComboBox);
        add(new JLabel("Search:"));
        add(searchField);
        add(searchButton);
    }
    
    public String getSearchText() {
        return searchField.getText();
    }
    
    public void clearSearch() {
        searchField.setText("");
    }
}