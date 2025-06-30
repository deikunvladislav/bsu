#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

struct Element1 {
    char Name[50];
    int SredniyBal;

    Element1() {
        Name[0] = '0';
        SredniyBal = 0;
    }
    Element1(int a) {
        Name[0] = '0';
        SredniyBal = a;
    }
    friend ostream& operator<<(ostream& out, const Element1& a) {
        out << a.Name << " " << a.SredniyBal;
        return out;
    }

    friend istream& operator>>(istream& in, Element1& a) {
        in.getline(a.Name, sizeof(a.Name));
        in >> a.SredniyBal;
        in.ignore();
        return in;
    }

    bool operator==(const Element1& other) const {
        return strcmp(Name, other.Name) == 0;
    }
    bool operator<(const Element1& other) const {
        return SredniyBal < other.SredniyBal; 
    }
};

struct Element2 {
    char Name[50];
    char Space[50];

    Element2() {
        Name[0] = '0';
        Space[0] = '0';
    }
    friend ostream& operator<<(ostream& out, const Element2& a) {
        out << a.Name << " " << a.Space;
        return out;
    }

    friend istream& operator>>(istream& in, Element2& a) {
        in.getline(a.Name, sizeof(a.Name));
        in.getline(a.Space, sizeof(a.Space));
        return in;
    }
    bool operator==(const Element2& other) const {
        return strcmp(Name, other.Name) == 0;
    }
};

struct Element3 {
    char Name[50];
    int SredniyBal;

    Element3() {
        Name[0] = '0';
        SredniyBal = 0;
    }

    friend ostream& operator<<(ostream& out, const Element3& a) {
        out << a.Name << " " << a.SredniyBal;
        return out;
    }
    Element3& operator=(const Element1& other) {
            strcpy(Name, other.Name);
            SredniyBal = other.SredniyBal;
        return *this;
    }
};

template <typename T>
class MASSIV {
private:
    T* data;
    size_t size;

public:
    MASSIV() : data(nullptr), size(0) {}

    MASSIV(size_t n) : size(n) {
        data = new T[n];
        for (size_t i = 0; i < size; i++) {
            data[i] = T();
        }
    }

    MASSIV(const MASSIV<T>& other) : size(other.size) {
        data = new T[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    ~MASSIV() {
        delete[] data;
    }

    T& operator[](size_t index) {
        if (index >= size) {
            cout << "Index out of range";
            exit(1);
        }
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size) {
            cout << "Index out of range";
            exit(1); 
        }
        return data[index];
    }

    size_t getSize() const {
        return size;
    }

    MASSIV<T>& operator=(const MASSIV<T>& other) {
        if (this == &other) return *this;
        delete[] data;
        size = other.size;
        data = new T[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        return *this;
    }

    friend ostream& operator<<(ostream& out, const MASSIV<T>& Arr) {
        for (size_t i = 0; i < Arr.size; i++) {
            out << Arr.data[i] << endl;
        }
        return out;
    }

    friend istream& operator>>(istream& in, MASSIV<T>& Arr) {
        for (size_t i = 0; i < Arr.size; i++) {
            in >> ws; 
            in >> Arr.data[i];
        }
        return in;
    }
};
bool operator==(const Element1& a, const Element2& b) {
    return strcmp(a.Name, b.Name) == 0;
}
void printSportPeople(const MASSIV<Element2>& people) {
    for (size_t i = 0; i < people.getSize(); i++) {
        if (strcmp(people[i].Space, "sport") == 0) {
            cout << people[i].Name << " " << people[i].Space << endl;
        }
    }
}
template <typename T1, typename T2>
MASSIV<Element3> connecter(const MASSIV<T1>& Arr1, const MASSIV<T2>& Arr2, T1 t) {
    vector<Element3> result;
    for (size_t i = 0; i < Arr1.getSize(); i++) {
        if (Arr1[i] < t) continue;
        for (size_t j = 0; j < Arr2.getSize(); j++) {
            if (Arr1[i]==Arr2[j] ) {
                Element3 a;
                a = Arr1[i];
                result.push_back(a);
            }
        }
    }

    MASSIV<Element3> connection(result.size());

    for (size_t i = 0; i < result.size(); i++) {
        connection[i] = result[i];
    }

    return connection;
}

void read(const string& str, MASSIV<Element1>& students1, MASSIV<Element2>& students2) {
    ifstream fstruct1(str + "1.txt");
    ifstream fstruct2(str + "2.txt");

    if (fstruct1.is_open()) {
        fstruct1 >> students1;
        fstruct1.close();
    }
    else {
        cout << "Unable to open file: " << str + "1.txt" << endl;
    }

    if (fstruct2.is_open()) {
        fstruct2 >> students2;
        fstruct2.close();
    }
    else {
        cout << "Unable to open file: " << str + "2.txt" << endl;
    }
}

void write(const string& str, const MASSIV<Element3>& results) {
    ofstream out(str);

    if (out.is_open()) {
        out << "Active and well studying students: " << endl;
        out << results;
        out.close();
    }
    else {
        cout << "Unable to open file: " << str << endl;
    }
}
template <typename T>
void sortMassiv(MASSIV<T>& arr, bool (*compare)(const T&, const T&)) {
    size_t size = arr.getSize();
    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = 0; j < size - 1 - i; j++) {
            if (compare(arr[j + 1], arr[j])) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}
bool compareSpace(const Element2& a, const Element2& b) {
    return strcmp(a.Space, b.Space) < 0;
}

int main() {
    MASSIV<Element1> students1(4);
    MASSIV<Element2> students2(5);

    Element1 t(8);
    read("struct", students1, students2);
    cout << "students1 : " << endl << students1 << endl;
    cout << "students2 : " << endl << students2 << endl;
    sortMassiv(students2, compareSpace);

    cout << "Sorted students2 : " << endl << students2 << endl;
    cout << "Sportsmens : " << endl;
    printSportPeople(students2);
    cout << endl;
    MASSIV<Element3> students3 = connecter(students1, students2, t);

    cout << "Active and well studying students : " << endl;
    cout << students3;

    write("active&wellstudying_students.txt", students3);

    return 0;
}