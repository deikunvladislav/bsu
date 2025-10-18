package factory.io;

import factory.model.AbstractEntity;
import java.util.List;

public abstract class AbstractFileHandler<T extends AbstractEntity> {
    public abstract List<T> readFromFile(String filename);
    public abstract void writeToFile(String filename, List<T> data);
}