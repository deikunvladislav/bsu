#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>

using namespace std;

struct Node {
    string name;
    string address;
    double percent;
    Node* prev;
    Node* next;

    Node(string name, string address, double percent)
        : name(name), address(address), percent(percent), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
private:
    Node* head;
    Node* tail;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    ~DoublyLinkedList() {
        clear();
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Не удалось открыть файл.");
        }

        string name, address;
        double percent;
        while (file >> name >> address >> percent) {
            addToEnd(name, address, percent);
        }
        file.close();
    }

    void addToBeginning(const string& name, const string& address, double percent) {
        Node* newNode = new Node(name, address, percent);
        if (!head) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }

    void addToEnd(const string& name, const string& address, double percent) {
        Node* newNode = new Node(name, address, percent);
        if (!tail) {
            head = tail = newNode;
        }
        else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
    }

    void addAtIndex(int index, const string& name, const string& address, double percent) {
        if (index < 0) throw out_of_range("Некорректный индекс.");

        Node* newNode = new Node(name, address, percent);
        if (index == 0) {
            addToBeginning(name, address, percent);
            return;
        }

        Node* current = head;
        for (int i = 0; i < index - 1; ++i) {
            if (!current) throw out_of_range("Индекс вне диапазона.");
            current = current->next;
        }

        if (!current->next) {
            addToEnd(name, address, percent);
        }
        else {
            newNode->next = current->next;
            newNode->prev = current;
            current->next->prev = newNode;
            current->next = newNode;
        }
    }

    void removeFromBeginning() {
        if (!head) throw runtime_error("Список пуст.");
        Node* temp = head;
        head = head->next;
        if (head) {
            head->prev = nullptr;
        }
        else {
            tail = nullptr;
        }
        delete temp;
    }

    void removeFromEnd() {
        if (!tail) throw runtime_error("Список пуст.");
        Node* temp = tail;
        tail = tail->prev;
        if (tail) {
            tail->next = nullptr;
        }
        else {
            head = nullptr;
        }
        delete temp;
    }

    void removeAtIndex(int index) {
        if (index < 0 || !head) throw out_of_range("Некорректный индекс или список пуст.");
        if (index == 0) {
            removeFromBeginning();
            return;
        }

        Node* current = head;
        for (int i = 0; i < index; ++i) {
            if (!current) throw out_of_range("Индекс вне диапазона.");
            current = current->next;
        }

        if (!current->next) {
            removeFromEnd();
        }
        else {
            current->prev->next = current->next;
            current->next->prev = current->prev;
            delete current;
        }
    }

    void clear() {
        while (head) {
            removeFromBeginning();
        }
    }

    void bubbleSort() {
        if (!head || !head->next) {
            return;
        }

        bool swapped;
        do {
            swapped = false;
            Node* current = head;

            while (current->next) {
                if (current->percent > current->next->percent) {
                    swap(current, current->next);
                    swapped = true;
                }
                current = current->next;
            }
        } while (swapped);
    }

    void swap(Node* a, Node* b) {
        string tempName = a->name;
        string tempAddress = a->address;
        double tempPercent = a->percent;

        a->name = b->name;
        a->address = b->address;
        a->percent = b->percent;

        b->name = tempName;
        b->address = tempAddress;
        b->percent = tempPercent;
    }

    void printToConsole() const {
        Node* current = head;
        while (current) {
            cout << current->name << " " << current->address << " " << current->percent << endl;
            current = current->next;
        }
    }

    void printToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Не удалось открыть файл.");
        }

        Node* current = head;
        while (current) {
            file << current->name << " " << current->address << " " << current->percent << endl;
            current = current->next;
        }
        file.close();
    }
};

int main() {
    DoublyLinkedList list;
    int choice;
    setlocale(LC_ALL, "RUS");
    do {
        cout << "1. Создать список из файла\n"
            << "2. Добавить элемент\n"
            << "3. Удалить элемент\n"
            << "4. Удалить все элементы\n"
            << "5. Сортировать список\n"
            << "6. Вывести список на консоль\n"
            << "7. Вывести список в файл\n"
            << "0. Выход\n"
            << "Выберите опцию: ";
        cin >> choice;

        try {
            switch (choice) {
            case 1: {
                string filename;
                cout << "Введите имя файла: ";
                cin >> filename;
                list.loadFromFile(filename);
                break;
            }
            case 2: {
                int index;
                string name, address;
                double percent;
                cout << "Введите данные (имя, адрес, процент): ";
                cin >> name >> address >> percent;
                cout << "Введите индекс (0 - начало, -1 - конец): ";
                cin >> index;
                if (index == 0) {
                    list.addToBeginning(name, address, percent);
                }
                else if (index == -1) {
                    list.addToEnd(name, address, percent);
                }
                else {
                    list.addAtIndex(index, name, address, percent);
                }
                break;
            }
            case 3: {
                int index;
                cout << "Введите индекс элемента для удаления (0 - начало, -1 - конец): ";
                cin >> index;
                if (index == 0) {
                    list.removeFromBeginning();
                }
                else if (index == -1) {
                    list.removeFromEnd();
                }
                else {
                    list.removeAtIndex(index);
                }
                break;
            }
            case 4:
                list.clear();
                break;
            case 5:
                list.bubbleSort();
                break;
            case 6:
                list.printToConsole();
                break;
            case 7: {
                string filename;
                cout << "Введите имя файла: ";
                cin >> filename;
                list.printToFile(filename);
                break;
            }
            case 0:
                cout << "Выход..." << endl;
                break;
            default:
                cout << "Некорректный выбор. Попробуйте снова." << endl;
            }
        }
        catch (const exception& e) {
            cout << "Ошибка: " << e.what() << endl;
        }
    } while (choice != 0);

    return 0;
}
