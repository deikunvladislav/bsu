#include <QCoreApplication>
#include <QList>
#include <QVector>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <bitset>
#include <cmath>
using namespace std;

int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);
    QList<int> listik;
    srand(time(0));
    while (true) {
        cout << "Menu:" << endl;
        cout << "1. Check the QList" << endl;
        cout << "2. Add element by index" << endl;
        cout << "3. Delete element by index" << endl;
        cout << "4. Change the size of QList" << endl;
        cout << "5. Increment all elements by X (using transform algorithm)" << endl;
        cout << "6. Delete on condition (removeIf)" << endl;
        cout << "7. Counting leading bits of all elements of QList" << endl;
        cout << "8. qCopyBackword, half of list to its back"<<endl;
        cout << "9. Make list of 10 random elemnets"<<endl;
        cout << "0. Close the menu" << endl;
        cout << "Choose what to do: ";

        int k;
        cin>>k;

        switch (k) {
        case 1:{
            if (listik.isEmpty())
                cout << "List is empty." << endl;
            else {
                cout << "List at the moment:" << endl;
                for (int i = 0; i < listik.size(); i++)
                    cout << listik[i] << endl;
            }
            break;
        }
        case 2: {
            int index;
            double value;
            cout << "Enter index: ";
            cin>>index;
            if (index < 0 || index > listik.size())
                cout << "Index is incorrect." << endl;
            else {
                cout << "Enter the value of element: ";
                cin>>value;
                listik.insert(index, value);
                cout << "Element " << value << " successfully added at position " << index << endl;
            }
            break;
        }
        case 3: {
            int index;
            cout << "Enter index: ";
            cin>>index;
            if (index < 0 || index >= listik.size())
                cout << "Index is incorrect." << endl;
            else {
                listik.removeAt(index);
                cout << "Element at index " << index << " removed." << endl;
            }
            break;
        }
        case 4: {
            int size;
            cout << "Enter new size: ";
            cin>>size;
            if (size < 0)
                cout << "Size of list can't be negative." << endl;
            if (size == listik.size())
                cout << "Size of list wasn't changed." << endl;
            if (size > listik.size()) {
                int h = size - listik.size();
                for (int i = 0; i < h; i++)
                    listik.push_back(0);
                cout << "Size is incremented to " << size << " (added elements are 0)." << endl;
                if(size<listik.size()) {
                int h = listik.size() - size;
                for (int i = 0; i < h; i++)
                    listik.pop_back();
                cout << "Size is decremented to " << size << "." << endl;
            }
            break;
        }
        }
        case 5: {
            double x;
            cout << "Enter x: ";
            cin>>x;
            transform(listik.begin(), listik.end(), listik.begin(), [x](double a) { return a + x; });
            cout << "All elements have been incremented by " << x << "." << endl;
            break;
        }
        case 6: {
            double mark;
            cout << "Enter mark; all elements below mark will be deleted: ";
            cin>>mark;
            listik.removeIf([mark](double a) { return a < mark; });
            cout << "Elements below " << mark << " have been removed." << endl;
            break;
        }
        case 7:{
            QList<quint16> list16;
            if (listik.isEmpty()){
                cout << "List is empty." << endl;
            break;
            }
            else {
                cout << "List at the moment:" << endl;
                for (int i = 0; i < listik.size(); i++){
                    cout << listik[i] << endl;
                    list16.push_back((quint16)abs(listik[i]));
                }
                cout<<"amount of leading bits in each element"<<endl;
                for (int i=0;i<list16.size();i++){
                    if(listik[i]<65536 and listik[i]>-65536){
                    cout<<"element "<<listik[i]<<" number of leading bits "
                             <<qCountLeadingZeroBits(list16[i])<< " check -" <<bitset<16>(list16[i])<<endl;
                    cout<<list16[i]<<endl;}
                }
                break;
            }
        }
        case 8:{
            QVector <int> arr(listik.size()/2);
            cout << "List before:" << endl;
            for (int i = 0; i < listik.size(); i++){
                cout << listik[i] << endl;
            }
            copy_backward(listik.begin(), listik.begin()+listik.size()/2, arr.end());
            for (int i = 0; i < arr.size(); i++){
                listik.push_back(arr[i]);
            }
            cout << "List after:" << endl;
            for (int i = 0; i < listik.size(); i++){
                cout << listik[i] << endl;
            }
            break;
        }
        case 9:{
            for (int i=0; i<10; i++)
                listik.push_back(rand()%10000 - 5000 + 1);
            break;
        }
        case 0:{
            cout << "THE END OF PROGRAMME" << endl<<endl<<endl;
            return 0;
        }
        default:
            cout << "Wrong selection, try again." << endl;
            cout << endl;
        }
    }
    return 0;
}
