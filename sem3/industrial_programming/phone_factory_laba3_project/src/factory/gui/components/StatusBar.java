package factory.gui.components;

import javax.swing.*;
import java.awt.*;

public class StatusBar extends JPanel {
    private JLabel statusLabel;
    private JLabel recordCountLabel;
    private JProgressBar progressBar;
    
    public StatusBar() {
        initializeUI();
    }
    
    private void initializeUI() {
        setLayout(new BorderLayout());
        setBorder(BorderFactory.createEtchedBorder());
        
        statusLabel = new JLabel("Ready");
        recordCountLabel = new JLabel("Records: 0");
        progressBar = new JProgressBar();
        progressBar.setVisible(false);
        
        JPanel leftPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        leftPanel.add(statusLabel);
        
        JPanel rightPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        rightPanel.add(recordCountLabel);
        rightPanel.add(progressBar);
        
        add(leftPanel, BorderLayout.WEST);
        add(rightPanel, BorderLayout.EAST);
    }
    
    public void setStatus(String status) {
        statusLabel.setText(status);
    }
    
    public void setRecordCount(int count) {
        recordCountLabel.setText("Records: " + count);
    }
    
    public void showProgress(boolean show) {
        progressBar.setVisible(show);
    }
    
    public void setProgress(int value) {
        progressBar.setValue(value);
    }
    
    public void setIndeterminate(boolean indeterminate) {
        progressBar.setIndeterminate(indeterminate);
    }
}