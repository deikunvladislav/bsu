package factory.io;

import factory.model.Phone;
import java.util.List;

public class FileHandlerAdapter implements DataWriter {
    private AbstractFileHandler<Phone> fileHandler;

    public FileHandlerAdapter(AbstractFileHandler<Phone> fileHandler) {
        this.fileHandler = fileHandler;
    }

    @Override
    public void write(String filename, List<Phone> data) {
        fileHandler.writeToFile(filename, data);
    }

    @Override
    public List<Phone> read(String filename) {
        return fileHandler.readFromFile(filename);
    }
}