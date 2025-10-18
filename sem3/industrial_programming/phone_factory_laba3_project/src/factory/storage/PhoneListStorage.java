package factory.storage;

import factory.model.Phone;
import java.util.ArrayList;
import java.util.List;

public class PhoneListStorage extends AbstractStorage<Phone> {
    private final List<Phone> phones = new ArrayList<>();

    @Override
    public void add(Phone phone) {
        phones.add(phone);
    }

    @Override
    public void update(int id, Phone phone) {
        for (int i = 0; i < phones.size(); i++) {
            if (phones.get(i).getId() == id) {
                phones.set(i, phone);
                return;
            }
        }
    }

    @Override
    public void delete(int id) {
        phones.removeIf(p -> p.getId() == id);
    }

    @Override
    public void printAll() {
        for (Phone p : phones) {
            System.out.println(p);
        }
    }

    @Override
    public Phone findById(int id) {
        for (Phone p : phones) {
            if (p.getId() == id) {
                return p;
            }
        }
        return null;
    }
    
    @Override
    public List<Phone> getAll() {
        return new ArrayList<>(phones);
    }
}