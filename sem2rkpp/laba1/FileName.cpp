#include <iostream>
#include <string>
using namespace std;
class Abstract
{
protected:
    static int counter;
    const int id;
public:
    virtual void Printer(void) {};
    Abstract() : id(counter) {
        counter++;
    }
    virtual ~Abstract() {
        counter--;
    }
    static int coutcounter() {
        return counter;
    }
    const int Getid() {
        return id;
    }
};
int Abstract::counter = 0;

class car : public Abstract
{
public:
    car() : Abstract() {};
    ~car() {};
    car(int a, int b, string str) : Abstract()
    {
        power = a;
        cylinder_number = b;
        marka = str;
    }
    int power;
    int cylinder_number;
    string marka;
    void Printer() override
    {
        cout << "identificational number : " << Getid() << endl;
        cout << "characteristics : " << endl;
        cout << "power : " << power << endl;
        cout << "cylinder_number : " << cylinder_number << endl;
        cout << "marka : " << marka << endl;
    }
};

class truck : public car
{
public:
    int weight_lifting;
    truck() : car() {};
    ~truck() {};
    truck(int a, int b, int c, string str) : car() {
        cylinder_number = a;
        power = b;
        weight_lifting = c;
        marka = str;
    }
    void Printer() override
    {
        cout << "identificational number : " << Getid() << endl;
        cout << "characteristics : " << endl;
        cout << "power : " << power << endl;
        cout << "cylinder_number : " << cylinder_number << endl;
        cout << "marka : " << marka << endl;
        cout << "weight_lifting : " << weight_lifting << endl;
    }
    void remaker(int weight_lifting_new, string marka_new) {
        weight_lifting = weight_lifting_new;
        marka = marka_new;
    }
};

int main()
{
    car a;
    a.power = 300;
    a.cylinder_number = 8;
    a.marka = "bmw";
    a.Printer();
    cout << "current amount of objects : " << Abstract::coutcounter() << endl;
    cout << endl;
    truck b(6, 200, 150, "volvo");
    b.Printer();
    cout << "current amount of objects : " << Abstract::coutcounter() << endl;
    cout << endl << "after remaking" << endl << endl;
    b.remaker(400, "BELAZ");
    b.Printer();
    cout << "current amount of objects : " << Abstract::coutcounter() << endl;
    return 0;
}