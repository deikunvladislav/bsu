package factory;

import factory.gui.MainFrame;
import factory.io.*;
import factory.storage.AbstractStorage;
import factory.model.Phone;
import javax.swing.*;

public class Main {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            try {
                UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
                
                Object[] options = {"List based storage", "Map based storage"};
                int storageChoice = JOptionPane.showOptionDialog(null,
                    "Choose storage type",
                    "Storage Selection",
                    JOptionPane.DEFAULT_OPTION,
                    JOptionPane.QUESTION_MESSAGE,
                    null,
                    options,
                    options[0]);
                
                AbstractStorage<Phone> storage;
                if (storageChoice == 1) {
                    storage = new factory.storage.PhoneMapStorage();
                } else {
                    storage = new factory.storage.PhoneListStorage();
                }
                
                Object[] fileOptions = {"TXT (phones.txt)", "XML (phones.xml)", "JSON (phones.json)"};
                int fileFormatChoice = JOptionPane.showOptionDialog(null,
                    "Choose initial file format",
                    "File Format Selection",
                    JOptionPane.DEFAULT_OPTION,
                    JOptionPane.QUESTION_MESSAGE,
                    null,
                    fileOptions,
                    fileOptions[0]);
                
                AbstractFileHandler<Phone> fileHandler = null;
                String filename = "";
                switch (fileFormatChoice) {
                    case 0:
                        fileHandler = new PhoneFileHandler();
                        filename = "phones.txt";
                        break;
                    case 1:
                        fileHandler = new PhoneXmlFileHandler();
                        filename = "phones.xml";
                        break;
                    case 2:
                        fileHandler = new PhoneJsonFileHandler();
                        filename = "phones.json";
                        break;
                    default:
                        fileHandler = new PhoneFileHandler();
                        filename = "phones.txt";
                }
                
                java.util.List<Phone> phones = fileHandler.readFromFile(filename);
                for (Phone phone : phones) {
                    storage.add(phone);
                }
                
                MainFrame mainFrame = new MainFrame(storage, fileHandler);
                mainFrame.setVisible(true);
                
            } catch (Exception e) {
                e.printStackTrace();
                JOptionPane.showMessageDialog(null,
                    "Error starting application: " + e.getMessage(),
                    "Fatal Error",
                    JOptionPane.ERROR_MESSAGE);
            }
        });
    }
}