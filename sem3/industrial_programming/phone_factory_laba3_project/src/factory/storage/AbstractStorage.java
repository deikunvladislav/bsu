package factory.storage;

import factory.model.AbstractEntity;
import java.util.List;

public abstract class AbstractStorage<T extends AbstractEntity> {
    public abstract void add(T obj);
    public abstract void update(int id, T obj);
    public abstract void delete(int id);
    public abstract void printAll();
    public abstract T findById(int id);
    public abstract List<T> getAll();
}