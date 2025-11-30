package factory.io;

import factory.model.Phone;
import java.util.List;

public abstract class AbstractDataWriterDecorator implements DataWriter {
    protected DataWriter wrappedWriter;

    public AbstractDataWriterDecorator(DataWriter wrappedWriter) {
        this.wrappedWriter = wrappedWriter;
    }

    @Override
    public void write(String filename, List<Phone> data) {
        wrappedWriter.write(filename, data);
    }

    @Override
    public List<Phone> read(String filename) {
        return wrappedWriter.read(filename);
    }
}