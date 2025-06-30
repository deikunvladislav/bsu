#include <iostream>
#include <stdexcept>
using namespace std;

class Point {
private:
    int* coords;
    int dimensions;

public:
    Point(int dim);
    Point(const Point& other);
    ~Point();
    Point& operator=(const Point& other);
    Point operator+(const Point& other) const;
    Point operator-(const Point& other) const;
    Point operator*(int scalar) const;
    Point operator/(int scalar) const;
    Point& operator++();
    Point operator++(int);
    Point& operator--();
    Point operator--(int);
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
    bool operator>=(const Point& other) const;
    bool operator<=(const Point& other) const;
    int& operator[](int index);
    const int& operator[](int index) const;
    Point operator~() const;
    friend Point operator+(const Point& other) const;
    friend Point operator-(const Point& other) const;
    friend Point operator*(int scalar) const;
    friend ostream& operator<<(ostream& out, const Point& p);
    friend istream& operator>>(istream& in, Point& p);
};