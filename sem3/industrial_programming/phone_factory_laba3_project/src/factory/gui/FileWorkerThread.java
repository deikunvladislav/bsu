package factory.gui;

import factory.io.*;
import factory.model.Phone;
import javax.swing.*;
import java.util.List;

public class FileWorkerThread extends Thread {
    private final AbstractFileHandler<Phone> fileHandler;
    private final String filePath;
    private final List<Phone> data;
    private final boolean isReadOperation;
    private final JFrame parentFrame;
    private final Runnable onComplete;
    
    public FileWorkerThread(AbstractFileHandler<Phone> fileHandler,
                          String filePath,
                          List<Phone> data,
                          boolean isReadOperation,
                          JFrame parentFrame,
                          Runnable onComplete) {
        this.fileHandler = fileHandler;
        this.filePath = filePath;
        this.data = data;
        this.isReadOperation = isReadOperation;
        this.parentFrame = parentFrame;
        this.onComplete = onComplete;
    }
    
    @Override
    public void run() {
        try {
            if (isReadOperation) {
                List<Phone> result = fileHandler.readFromFile(filePath);
                SwingUtilities.invokeLater(() -> {
                    if (onComplete != null) {
                        onComplete.run();
                    }
                    showSuccessDialog("Successfully loaded " + result.size() + " phones");
                });
            } else {
                fileHandler.writeToFile(filePath, data);
                SwingUtilities.invokeLater(() -> {
                    if (onComplete != null) {
                        onComplete.run();
                    }
                    showSuccessDialog("File saved successfully");
                });
            }
        } catch (Exception e) {
            SwingUtilities.invokeLater(() -> 
                showErrorDialog("Error: " + e.getMessage()));
        }
    }
    
    private void showSuccessDialog(String message) {
        JOptionPane.showMessageDialog(parentFrame,
            message,
            "Success",
            JOptionPane.INFORMATION_MESSAGE);
    }
    
    private void showErrorDialog(String message) {
        JOptionPane.showMessageDialog(parentFrame,
            message,
            "Error",
            JOptionPane.ERROR_MESSAGE);
    }
}