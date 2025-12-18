package factory.gui;

import factory.gui.components.ButtonPanel;
import factory.gui.components.StatusBar;
import factory.io.*;
import factory.model.Phone;
import factory.storage.AbstractStorage;
import factory.validation.PhoneValidator;
import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.io.File;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class MainFrame extends JFrame {
    private AbstractStorage<Phone> storage;
    private AbstractFileHandler<Phone> currentFileHandler;
    private JTable phoneTable;
    private DefaultTableModel tableModel;
    private ExecutorService executorService;
    private ButtonPanel buttonPanel;
    private StatusBar statusBar;
    
    public MainFrame(AbstractStorage<Phone> storage, AbstractFileHandler<Phone> fileHandler) {
        this.storage = storage;
        this.currentFileHandler = fileHandler;
        this.executorService = Executors.newFixedThreadPool(3);
        
        initializeUI();
        loadInitialData();
    }
    
    private void initializeUI() {
        setTitle("Phone Factory Manager");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());
        
        JMenuBar menuBar = createMenuBar();
        setJMenuBar(menuBar);
        
        buttonPanel = new ButtonPanel(this);
        add(buttonPanel, BorderLayout.NORTH);
        
        JScrollPane tableScrollPane = createTable();
        add(tableScrollPane, BorderLayout.CENTER);
        
        statusBar = new StatusBar();
        add(statusBar, BorderLayout.SOUTH);
        
        setSize(1000, 700);
        setLocationRelativeTo(null);
    }
    
    private JMenuBar createMenuBar() {
        JMenuBar menuBar = new JMenuBar();
        
        JMenu fileMenu = new JMenu("File");
        JMenuItem loadTxtItem = new JMenuItem("Load from TXT");
        JMenuItem saveTxtItem = new JMenuItem("Save to TXT");
        JMenuItem loadXmlItem = new JMenuItem("Load from XML");
        JMenuItem saveXmlItem = new JMenuItem("Save to XML");
        JMenuItem loadJsonItem = new JMenuItem("Load from JSON");
        JMenuItem saveJsonItem = new JMenuItem("Save to JSON");
        JMenuItem exitItem = new JMenuItem("Exit");
        
        loadTxtItem.addActionListener(e -> loadFromFileAsync("txt"));
        saveTxtItem.addActionListener(e -> saveToFileAsync("txt"));
        loadXmlItem.addActionListener(e -> loadFromFileAsync("xml"));
        saveXmlItem.addActionListener(e -> saveToFileAsync("xml"));
        loadJsonItem.addActionListener(e -> loadFromFileAsync("json"));
        saveJsonItem.addActionListener(e -> saveToFileAsync("json"));
        exitItem.addActionListener(e -> System.exit(0));
        
        fileMenu.add(loadTxtItem);
        fileMenu.add(saveTxtItem);
        fileMenu.addSeparator();
        fileMenu.add(loadXmlItem);
        fileMenu.add(saveXmlItem);
        fileMenu.addSeparator();
        fileMenu.add(loadJsonItem);
        fileMenu.add(saveJsonItem);
        fileMenu.addSeparator();
        fileMenu.add(exitItem);
        
        JMenu editMenu = new JMenu("Edit");
        JMenuItem addItem = new JMenuItem("Add Phone");
        JMenuItem editItem = new JMenuItem("Edit Selected");
        JMenuItem deleteItem = new JMenuItem("Delete Selected");
        
        addItem.addActionListener(e -> showAddPhoneDialog());
        editItem.addActionListener(e -> editSelectedPhone());
        deleteItem.addActionListener(e -> deleteSelectedPhone());
        
        editMenu.add(addItem);
        editMenu.add(editItem);
        editMenu.add(deleteItem);
        
        JMenu toolsMenu = new JMenu("Tools");
        JMenuItem sortItem = new JMenuItem("Sort Phones");
        JMenuItem encryptItem = new JMenuItem("Encrypt Data");
        JMenuItem archiveItem = new JMenuItem("Create Archive");
        JMenuItem decoratorItem = new JMenuItem("Demonstrate Decorator");
        
        sortItem.addActionListener(e -> showSortDialog());
        encryptItem.addActionListener(e -> showEncryptDialog());
        archiveItem.addActionListener(e -> showArchiveDialog());
        decoratorItem.addActionListener(e -> demonstrateDecoratorPattern());
        
        toolsMenu.add(sortItem);
        toolsMenu.add(encryptItem);
        toolsMenu.add(archiveItem);
        toolsMenu.add(decoratorItem);
        
        menuBar.add(fileMenu);
        menuBar.add(editMenu);
        menuBar.add(toolsMenu);
        
        return menuBar;
    }
    
    private JScrollPane createTable() {
        String[] columnNames = {"ID", "Brand", "Model", "Cameras", "Release Date", "Price"};
        tableModel = new DefaultTableModel(columnNames, 0) {
            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        };
        phoneTable = new JTable(tableModel);
        
        phoneTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        phoneTable.setRowHeight(25);
        phoneTable.getColumnModel().getColumn(0).setPreferredWidth(50);
        phoneTable.getColumnModel().getColumn(1).setPreferredWidth(100);
        phoneTable.getColumnModel().getColumn(2).setPreferredWidth(150);
        phoneTable.getColumnModel().getColumn(3).setPreferredWidth(70);
        phoneTable.getColumnModel().getColumn(4).setPreferredWidth(100);
        phoneTable.getColumnModel().getColumn(5).setPreferredWidth(80);
        
        return new JScrollPane(phoneTable);
    }
    
    public void showAddPhoneDialog() {
        AddEditPhoneDialog dialog = new AddEditPhoneDialog(this, null);
        dialog.setVisible(true);
        
        if (dialog.isConfirmed()) {
            Phone newPhone = dialog.getPhone();
            storage.add(newPhone);
            refreshTable();
            
            JOptionPane.showMessageDialog(this,
                "Phone added successfully!",
                "Success",
                JOptionPane.INFORMATION_MESSAGE);
        }
    }
    
    public void editSelectedPhone() {
        int selectedRow = phoneTable.getSelectedRow();
        if (selectedRow >= 0) {
            int id = (int) tableModel.getValueAt(selectedRow, 0);
            Phone phone = storage.findById(id);
            if (phone != null) {
                AddEditPhoneDialog dialog = new AddEditPhoneDialog(this, phone);
                dialog.setVisible(true);
                
                if (dialog.isConfirmed()) {
                    Phone updatedPhone = dialog.getPhone();
                    storage.update(id, updatedPhone);
                    refreshTable();
                    
                    JOptionPane.showMessageDialog(this,
                        "Phone updated successfully!",
                        "Success",
                        JOptionPane.INFORMATION_MESSAGE);
                }
            }
        } else {
            JOptionPane.showMessageDialog(this,
                "Please select a phone to edit.",
                "No Selection",
                JOptionPane.WARNING_MESSAGE);
        }
    }
    
    public void deleteSelectedPhone() {
        int selectedRow = phoneTable.getSelectedRow();
        if (selectedRow >= 0) {
            int id = (int) tableModel.getValueAt(selectedRow, 0);
            int confirm = JOptionPane.showConfirmDialog(this,
                "Are you sure you want to delete this phone?",
                "Confirm Delete",
                JOptionPane.YES_NO_OPTION);
            
            if (confirm == JOptionPane.YES_OPTION) {
                storage.delete(id);
                refreshTable();
                
                JOptionPane.showMessageDialog(this,
                    "Phone deleted successfully!",
                    "Success",
                    JOptionPane.INFORMATION_MESSAGE);
            }
        } else {
            JOptionPane.showMessageDialog(this,
                "Please select a phone to delete.",
                "No Selection",
                JOptionPane.WARNING_MESSAGE);
        }
    }
    
    private void loadFromFileAsync(String format) {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setDialogTitle("Select file to load");
        
        if (fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            statusBar.setStatus("Loading file...");
            statusBar.showProgress(true);
            statusBar.setIndeterminate(true);
            
            executorService.submit(() -> {
                try {
                    AbstractFileHandler<Phone> handler = null;
                    if (format.equals("txt")) {
                        handler = new PhoneFileHandler();
                    } else if (format.equals("xml")) {
                        handler = new PhoneXmlFileHandler();
                    } else if (format.equals("json")) {
                        handler = new PhoneJsonFileHandler();
                    }
                    
                    List<Phone> phones = handler.readFromFile(
                        fileChooser.getSelectedFile().getAbsolutePath());
                    
                    SwingUtilities.invokeLater(() -> {
                        clearStorage();
                        phones.forEach(storage::add);
                        refreshTable();
                        statusBar.setStatus("File loaded successfully");
                        statusBar.showProgress(false);
                        
                        JOptionPane.showMessageDialog(this,
                            String.format("Loaded %d phones from file", phones.size()),
                            "Success",
                            JOptionPane.INFORMATION_MESSAGE);
                    });
                    
                } catch (Exception e) {
                    SwingUtilities.invokeLater(() -> {
                        statusBar.setStatus("Error loading file");
                        statusBar.showProgress(false);
                        JOptionPane.showMessageDialog(this,
                            "Error loading file: " + e.getMessage(),
                            "Error",
                            JOptionPane.ERROR_MESSAGE);
                    });
                }
            });
        }
    }
    
    private void saveToFileAsync(String format) {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setDialogTitle("Save file");
        fileChooser.setSelectedFile(new File("phones." + format));
        
        if (fileChooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
            statusBar.setStatus("Saving file...");
            statusBar.showProgress(true);
            statusBar.setIndeterminate(true);
            
            executorService.submit(() -> {
                try {
                    AbstractFileHandler<Phone> handler = null;
                    if (format.equals("txt")) {
                        handler = new PhoneFileHandler();
                    } else if (format.equals("xml")) {
                        handler = new PhoneXmlFileHandler();
                    } else if (format.equals("json")) {
                        handler = new PhoneJsonFileHandler();
                    }
                    
                    handler.writeToFile(
                        fileChooser.getSelectedFile().getAbsolutePath(),
                        storage.getAll());
                    
                    SwingUtilities.invokeLater(() -> {
                        statusBar.setStatus("File saved successfully");
                        statusBar.showProgress(false);
                        JOptionPane.showMessageDialog(this,
                            "File saved successfully!",
                            "Success",
                            JOptionPane.INFORMATION_MESSAGE);
                    });
                    
                } catch (Exception e) {
                    SwingUtilities.invokeLater(() -> {
                        statusBar.setStatus("Error saving file");
                        statusBar.showProgress(false);
                        JOptionPane.showMessageDialog(this,
                            "Error saving file: " + e.getMessage(),
                            "Error",
                            JOptionPane.ERROR_MESSAGE);
                    });
                }
            });
        }
    }
    
    public void refreshTable() {
        tableModel.setRowCount(0);
        List<Phone> phones = storage.getAll();
        for (Phone phone : phones) {
            Object[] row = {
                phone.getId(),
                phone.getBrand(),
                phone.getModel(),
                phone.getCameraCount(),
                PhoneValidator.formatDate(phone.getReleaseDate()),
                String.format("%.2f", phone.getPrice())
            };
            tableModel.addRow(row);
        }
        statusBar.setRecordCount(phones.size());
        statusBar.setStatus("Table refreshed");
    }
    
    public void sortTable(String criterion) {
        List<Phone> sorted = storage.getAll();
        switch (criterion) {
            case "ID":
                sorted.sort((a, b) -> Integer.compare(a.getId(), b.getId()));
                break;
            case "Brand":
                sorted.sort((a, b) -> a.getBrand().compareToIgnoreCase(b.getBrand()));
                break;
            case "Model":
                sorted.sort((a, b) -> a.getModel().compareToIgnoreCase(b.getModel()));
                break;
            case "Price":
                sorted.sort((a, b) -> Double.compare(a.getPrice(), b.getPrice()));
                break;
            case "Release Date":
                sorted.sort((a, b) -> a.getReleaseDate().compareTo(b.getReleaseDate()));
                break;
            case "Camera Count":
                sorted.sort((a, b) -> Integer.compare(a.getCameraCount(), b.getCameraCount()));
                break;
        }
        
        tableModel.setRowCount(0);
        for (Phone phone : sorted) {
            Object[] row = {
                phone.getId(),
                phone.getBrand(),
                phone.getModel(),
                phone.getCameraCount(),
                PhoneValidator.formatDate(phone.getReleaseDate()),
                String.format("%.2f", phone.getPrice())
            };
            tableModel.addRow(row);
        }
        statusBar.setStatus("Sorted by " + criterion);
    }
    
    public void searchPhones(String query) {
        if (query == null || query.trim().isEmpty()) {
            refreshTable();
            return;
        }
        
        List<Phone> allPhones = storage.getAll();
        tableModel.setRowCount(0);
        
        int foundCount = 0;
        for (Phone phone : allPhones) {
            if (String.valueOf(phone.getId()).contains(query) ||
                phone.getBrand().toLowerCase().contains(query.toLowerCase()) ||
                phone.getModel().toLowerCase().contains(query.toLowerCase()) ||
                String.valueOf(phone.getCameraCount()).contains(query) ||
                PhoneValidator.formatDate(phone.getReleaseDate()).contains(query) ||
                String.format("%.2f", phone.getPrice()).contains(query)) {
                
                Object[] row = {
                    phone.getId(),
                    phone.getBrand(),
                    phone.getModel(),
                    phone.getCameraCount(),
                    PhoneValidator.formatDate(phone.getReleaseDate()),
                    String.format("%.2f", phone.getPrice())
                };
                tableModel.addRow(row);
                foundCount++;
            }
        }
        statusBar.setStatus("Found " + foundCount + " phones");
    }
    
    private void showSortDialog() {
        String[] options = {"ID", "Brand", "Model", "Price", "Release Date", "Camera Count"};
        String choice = (String) JOptionPane.showInputDialog(this,
            "Select sort criteria:",
            "Sort Phones",
            JOptionPane.QUESTION_MESSAGE,
            null,
            options,
            options[0]);
        
        if (choice != null) {
            sortTable(choice);
        }
    }
    
    private void showEncryptDialog() {
        String[] options = {"Encrypt current data", "Decrypt from file"};
        int choice = JOptionPane.showOptionDialog(this,
            "Select encryption option:",
            "Encryption",
            JOptionPane.DEFAULT_OPTION,
            JOptionPane.QUESTION_MESSAGE,
            null,
            options,
            options[0]);
        
        if (choice == 0) {
            encryptCurrentData();
        } else if (choice == 1) {
            decryptFromFile();
        }
    }
    
    private void showArchiveDialog() {
        String[] options = {"Create ZIP archive", "Create JAR archive"};
        int choice = JOptionPane.showOptionDialog(this,
            "Select archive type:",
            "Create Archive",
            JOptionPane.DEFAULT_OPTION,
            JOptionPane.QUESTION_MESSAGE,
            null,
            options,
            options[0]);
        
        if (choice == 0) {
            createZipArchive();
        } else if (choice == 1) {
            createJarArchive();
        }
    }
    
    private void encryptCurrentData() {
        statusBar.setStatus("Encrypting data...");
        statusBar.showProgress(true);
        statusBar.setIndeterminate(true);
        
        executorService.submit(() -> {
            try {
                List<Phone> phones = storage.getAll();
                org.json.JSONArray jsonArray = new org.json.JSONArray();
                
                for (Phone phone : phones) {
                    org.json.JSONObject jsonObject = new org.json.JSONObject();
                    jsonObject.put("id", phone.getId());
                    jsonObject.put("brand", phone.getBrand());
                    jsonObject.put("model", phone.getModel());
                    jsonObject.put("cameraCount", phone.getCameraCount());
                    jsonObject.put("releaseDate", PhoneValidator.formatDate(phone.getReleaseDate()));
                    jsonObject.put("price", phone.getPrice());
                    jsonArray.put(jsonObject);
                }
                
                String dataToEncrypt = jsonArray.toString();
                String encryptedData = factory.io.EncryptionUtil.encrypt(dataToEncrypt);
                
                try (java.io.PrintWriter writer = new java.io.PrintWriter("encrypted_data.txt")) {
                    writer.print(encryptedData);
                }
                
                SwingUtilities.invokeLater(() -> {
                    statusBar.setStatus("Data encrypted successfully");
                    statusBar.showProgress(false);
                    JOptionPane.showMessageDialog(this,
                        String.format("Data encrypted and saved to encrypted_data.txt\nEncrypted %d phones", phones.size()),
                        "Success",
                        JOptionPane.INFORMATION_MESSAGE);
                });
                
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    statusBar.setStatus("Error encrypting data");
                    statusBar.showProgress(false);
                    JOptionPane.showMessageDialog(this,
                        "Error encrypting data: " + e.getMessage(),
                        "Error",
                        JOptionPane.ERROR_MESSAGE);
                });
            }
        });
    }
    
    private void decryptFromFile() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setDialogTitle("Select encrypted file");
        
        if (fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            statusBar.setStatus("Decrypting data...");
            statusBar.showProgress(true);
            statusBar.setIndeterminate(true);
            
            executorService.submit(() -> {
                try {
                    StringBuilder encryptedData = new StringBuilder();
                    try (java.io.BufferedReader reader = new java.io.BufferedReader(
                            new java.io.FileReader(fileChooser.getSelectedFile()))) {
                        String line;
                        while ((line = reader.readLine()) != null) {
                            encryptedData.append(line);
                        }
                    }
                    
                    String decryptedData = factory.io.EncryptionUtil.decrypt(encryptedData.toString());
                    org.json.JSONArray jsonArray = new org.json.JSONArray(decryptedData);
                    
                    List<Phone> decryptedPhones = new java.util.ArrayList<>();
                    for (int i = 0; i < jsonArray.length(); i++) {
                        try {
                            org.json.JSONObject jsonObject = jsonArray.getJSONObject(i);
                            int id = jsonObject.getInt("id");
                            String brand = jsonObject.getString("brand");
                            String model = jsonObject.getString("model");
                            int cameraCount = jsonObject.getInt("cameraCount");
                            String dateStr = jsonObject.getString("releaseDate");
                            double price = jsonObject.getDouble("price");
                            
                            java.util.Date releaseDate = PhoneValidator.parseDate(dateStr);
                            Phone phone = new Phone(id, brand, model, cameraCount, releaseDate, price);
                            decryptedPhones.add(phone);
                        } catch (Exception e) {
                            System.err.println("Error parsing phone data: " + e.getMessage());
                        }
                    }
                    
                    List<Phone> finalPhones = decryptedPhones;
                    SwingUtilities.invokeLater(() -> {
                        statusBar.setStatus("Data decrypted successfully");
                        statusBar.showProgress(false);
                        
                        String[] options = {"Replace current data", "Add to current data"};
                        int choice = JOptionPane.showOptionDialog(this,
                            String.format("Loaded %d phones. Choose loading option:", finalPhones.size()),
                            "Decrypt Data",
                            JOptionPane.DEFAULT_OPTION,
                            JOptionPane.QUESTION_MESSAGE,
                            null,
                            options,
                            options[0]);
                        
                        if (choice == 0) {
                            clearStorage();
                            finalPhones.forEach(storage::add);
                        } else if (choice == 1) {
                            int added = 0;
                            int skipped = 0;
                            for (Phone phone : finalPhones) {
                                if (storage.findById(phone.getId()) == null) {
                                    storage.add(phone);
                                    added++;
                                } else {
                                    skipped++;
                                }
                            }
                            JOptionPane.showMessageDialog(this,
                                String.format("Added %d phones, skipped %d duplicates", added, skipped),
                                "Result",
                                JOptionPane.INFORMATION_MESSAGE);
                        }
                        refreshTable();
                    });
                    
                } catch (Exception e) {
                    SwingUtilities.invokeLater(() -> {
                        statusBar.setStatus("Error decrypting data");
                        statusBar.showProgress(false);
                        JOptionPane.showMessageDialog(this,
                            "Error decrypting data: " + e.getMessage(),
                            "Error",
                            JOptionPane.ERROR_MESSAGE);
                    });
                }
            });
        }
    }
    
    private void createZipArchive() {
        String archiveName = JOptionPane.showInputDialog(this,
            "Enter archive name (without extension):",
            "Create ZIP Archive",
            JOptionPane.QUESTION_MESSAGE);
        
        if (archiveName != null && !archiveName.trim().isEmpty()) {
            statusBar.setStatus("Creating ZIP archive...");
            statusBar.showProgress(true);
            statusBar.setIndeterminate(true);
            
            executorService.submit(() -> {
                String[] filesToArchive = {
                    "phones.txt", "phones.xml", "phones.json",
                    "report.txt", "report.xml", "report.json",
                    "errors.log", "encrypted_data.txt"
                };
                
                java.util.List<String> existingFiles = new java.util.ArrayList<>();
                for (String file : filesToArchive) {
                    java.io.File fileObj = new java.io.File(file);
                    if (fileObj.exists()) {
                        existingFiles.add(file);
                    }
                }
                
                if (existingFiles.isEmpty()) {
                    SwingUtilities.invokeLater(() -> {
                        statusBar.setStatus("No files to archive");
                        statusBar.showProgress(false);
                        JOptionPane.showMessageDialog(this,
                            "No files found to archive!",
                            "Warning",
                            JOptionPane.WARNING_MESSAGE);
                    });
                    return;
                }
                
                factory.io.ArchiveUtil.createZipArchiveFromFiles(
                    existingFiles.toArray(new String[0]),
                    archiveName + ".zip"
                );
                
                SwingUtilities.invokeLater(() -> {
                    statusBar.setStatus("ZIP archive created");
                    statusBar.showProgress(false);
                    JOptionPane.showMessageDialog(this,
                        String.format("ZIP archive created with %d files", existingFiles.size()),
                        "Success",
                        JOptionPane.INFORMATION_MESSAGE);
                });
            });
        }
    }
    
    private void createJarArchive() {
        String archiveName = JOptionPane.showInputDialog(this,
            "Enter JAR name (without extension):",
            "Create JAR Archive",
            JOptionPane.QUESTION_MESSAGE);
        
        if (archiveName != null && !archiveName.trim().isEmpty()) {
            statusBar.setStatus("Creating JAR archive...");
            statusBar.showProgress(true);
            statusBar.setIndeterminate(true);
            
            executorService.submit(() -> {
                String[] filesToInclude = {
                    "phones.txt", "phones.xml", "phones.json",
                    "report.txt", "report.xml", "report.json"
                };
                
                java.util.List<String> existingFiles = new java.util.ArrayList<>();
                for (String file : filesToInclude) {
                    java.io.File fileObj = new java.io.File(file);
                    if (fileObj.exists()) {
                        existingFiles.add(file);
                    }
                }
                
                if (existingFiles.isEmpty()) {
                    SwingUtilities.invokeLater(() -> {
                        statusBar.setStatus("No files to archive");
                        statusBar.showProgress(false);
                        JOptionPane.showMessageDialog(this,
                            "No files found to archive!",
                            "Warning",
                            JOptionPane.WARNING_MESSAGE);
                    });
                    return;
                }
                
                factory.io.ArchiveUtil.createJarArchive(
                    existingFiles.toArray(new String[0]),
                    archiveName + ".jar"
                );
                
                SwingUtilities.invokeLater(() -> {
                    statusBar.setStatus("JAR archive created");
                    statusBar.showProgress(false);
                    JOptionPane.showMessageDialog(this,
                        String.format("JAR archive created with %d files", existingFiles.size()),
                        "Success",
                        JOptionPane.INFORMATION_MESSAGE);
                });
            });
        }
    }
    
    public void showBuilderDialog() {
        BuilderPatternDialog dialog = new BuilderPatternDialog(this);
        dialog.setVisible(true);
        
        if (dialog.isConfirmed()) {
            Phone newPhone = dialog.getPhone();
            storage.add(newPhone);
            refreshTable();
            
            JOptionPane.showMessageDialog(this,
                "Phone created using Builder Pattern!",
                "Success",
                JOptionPane.INFORMATION_MESSAGE);
        }
    }
    
    private void demonstrateDecoratorPattern() {
        String[] options = {"Basic writer", "Writer with encryption"};
        int choice = JOptionPane.showOptionDialog(this,
            "Select writer type:",
            "Demonstrate Decorator Pattern",
            JOptionPane.DEFAULT_OPTION,
            JOptionPane.QUESTION_MESSAGE,
            null,
            options,
            options[0]);
        
        if (choice == 0) {
            factory.io.SimpleDataWriter writer = new factory.io.SimpleDataWriter();
            writer.write("decorator_demo.txt", storage.getAll());
            JOptionPane.showMessageDialog(this,
                "Data written with SimpleDataWriter",
                "Success",
                JOptionPane.INFORMATION_MESSAGE);
        } else if (choice == 1) {
            factory.io.SimpleDataWriter simpleWriter = new factory.io.SimpleDataWriter();
            factory.io.EncryptionDataWriterDecorator encryptedWriter = 
                new factory.io.EncryptionDataWriterDecorator(simpleWriter);
            encryptedWriter.write("decorator_demo.txt", storage.getAll());
            JOptionPane.showMessageDialog(this,
                "Data written with EncryptionDataWriterDecorator",
                "Success",
                JOptionPane.INFORMATION_MESSAGE);
        }
    }
    
    private void clearStorage() {
        List<Phone> currentPhones = storage.getAll();
        for (Phone phone : currentPhones) {
            storage.delete(phone.getId());
        }
    }
    
    private void loadInitialData() {
        refreshTable();
    }
    
    @Override
    public void dispose() {
        executorService.shutdown();
        super.dispose();
    }
}