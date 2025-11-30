package factory.io;

import factory.model.Phone;
import java.util.List;

public interface DataWriter {
    void write(String filename, List<Phone> data);
    List<Phone> read(String filename);
}