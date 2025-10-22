package factory.storage;

import factory.model.Phone;
import java.util.ArrayList;
import java.util.List;
import java.util.SortedMap;
import java.util.TreeMap;

public class PhoneMapStorage extends AbstractStorage<Phone> {
    private SortedMap<Integer, Phone> phoneMap = new TreeMap<>();

    @Override
    public void add(Phone phone) { 
        phoneMap.put(phone.getId(), phone); 
    }

    @Override
    public void update(int id, Phone phone) { 
        phoneMap.put(id, phone); 
    }

    @Override
    public void delete(int id) { 
        phoneMap.remove(id); 
    }

    @Override
    public void printAll() {
        var iterator = phoneMap.values().iterator();
        while (iterator.hasNext()) {
            System.out.println(iterator.next());
        }
    }

    @Override
    public Phone findById(int id) {
        return phoneMap.get(id);
    }
    
    @Override
    public List<Phone> getAll() {
        return new ArrayList<>(phoneMap.values());
    }
    
    public SortedMap<Integer, Phone> getPhoneMap() { 
        return new TreeMap<>(phoneMap); 
    }
}