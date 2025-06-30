#include <iostream>
#include "Point.h"
using namespace std;
int main() {
    setlocale(LC_ALL, "RUS");
    cout << "введите размерность точки" << endl;
    int n;
    bool t;
    cin >> n;
    Point p1(n);
    Point p2(n);
    cout << "введите координаты точки 1: "<<endl;
    cin >> p1;
    cout << "введите координаты точки 2: "<<endl;
    cin >> p2;
    cout << "_________________________________" << endl;
    cout << "точка 1: " <<p1<< endl;
    cout << "точка 2: " <<p2<< endl;
    cout << "p1+p2= " << p1 + p2<<endl;
    cout << "p1++ : " <<p1++<< endl;
    cout << "p2++ : " << p2++ << endl;
    cout << "++p1 : " << ++p1 << endl;
    cout << "++p2 : " << ++p2 << endl;
    cout << "p1-p2= " << p1 - p2<<endl;
    cout << "p1-- : " << p1-- << endl;
    cout << "p2-- : " << p2-- << endl;
    cout << "--p1 : " << --p1 << endl;
    cout << "--p2 : " << --p2 << endl;
    cout << "p1*2 : " << p1*2 << endl;
    cout << "p1/2 : " << p1/2<<endl;
    t = p1 != p2;
    cout <<"(p1!=p2)?? : "<< t << endl;
    t = p1 == p2;
    cout << "(p1==p2)?? : " << t << endl;
    t = p1 >= p2;
    cout << "(p1>=p2)?? : " << t << endl;
    t = p1 <= p2;
    cout << "(p1<=p2)?? : " << t << endl;
    p1 = p2;
    cout << "p1=p2 : " << p1 << endl;
    cout << "p1[n-1] : " << p1[n - 1]<<endl;
    cout << "~p1 : " << ~p1 << endl;
    return 0;
}
