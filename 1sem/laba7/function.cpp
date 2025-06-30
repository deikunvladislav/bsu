#include "Point.h"
using namespace std;

Point::Point(int dim) : dimensions(dim) {
    coords = new int[dimensions]();
}

Point::Point(const Point& other) : dimensions(other.dimensions) {
    coords = new int[dimensions];
    for (int i = 0; i < dimensions; ++i) {
        coords[i] = other.coords[i];
    }
}

Point::~Point() {
    delete[] coords;
}

Point& Point::operator=(const Point& other) {
    if (this == &other) return *this;
    delete[] coords;
    dimensions = other.dimensions;
    coords = new int[dimensions];
    for (int i = 0; i < dimensions; ++i) {
        coords[i] = other.coords[i];
    }
    return *this;
}

Point Point::operator+(const Point& other) const {
    Point result(dimensions);
    for (int i = 0; i < dimensions; ++i) {
        result.coords[i] = coords[i] + other.coords[i];
    }
    return result;
}

Point Point::operator-(const Point& other) const {
    Point result(dimensions);
    for (int i = 0; i < dimensions; ++i) {
        result.coords[i] = coords[i] - other.coords[i];
    }
    return result;
}

Point Point::operator*(int scalar) const {
    for (int i = 0; i < dimensions; ++i) {
        coords[i] = coords[i] * scalar;
    }
    return *this;
}

Point Point::operator/(int scalar) const {
    if (scalar == 0) throw invalid_argument("деление на 0");
    for (int i = 0; i < dimensions; ++i) {
        coords[i] = coords[i] / scalar;
    }
    return *this;
}

Point& Point::operator++() {
    for (int i = 0; i < dimensions; ++i) {
        ++coords[i];
    }
    return *this;
}

Point Point::operator++(int) {
    Point temp(*this);
    ++(*this);
    return temp;
}

Point& Point::operator--() {
    for (int i = 0; i < dimensions; ++i) {
        --coords[i];
    }
    return *this;
}

Point Point::operator--(int) {
    Point temp(*this);
    --(*this);
    return temp;
}

bool Point::operator==(const Point& other) const {
    for (int i = 0; i < dimensions; ++i) {
        if (coords[i] != other.coords[i]) return false;
    }
    return true;
}

bool Point::operator!=(const Point& other) const {
    return !(*this == other);
}

bool Point::operator>=(const Point& other) const {
    for (int i = 0; i < dimensions; ++i) {
        if (coords[i] < other.coords[i]) return false;
    }
    return true;
}

bool Point::operator<=(const Point& other) const {
    for (int i = 0; i < dimensions; ++i) {
        if (coords[i] > other.coords[i]) return false;
    }
    return true;
}

int& Point::operator[](int index) {
    if (index < 0 || index >= dimensions) throw out_of_range("выход за границу массива");
    return coords[index];
}

const int& Point::operator[](int index) const {
    if (index < 0 || index >= dimensions) throw out_of_range("выход за границу массива");
    return coords[index];
}

Point Point::operator~() const {
    Point result(dimensions);
    for (int i = 0; i < dimensions; ++i) {
        result.coords[i] = (-1)*coords[i];
    }
    return result;
}

Point operator+(const Point& other) const {
    Point result(dimensions);
    for (int i = 0; i < dimensions; ++i) {
        result.coords[i] = coords[i] + other.coords[i];
    }
    return result;
}

Point operator-(const Point& other) const {
    Point result(dimensions);
    for (int i = 0; i < dimensions; ++i) {
        result.coords[i] = coords[i] - other.coords[i];
    }
    return result;
}

Point Poperator*(int scalar) const {
    for (int i = 0; i < dimensions; ++i) {
        coords[i] = coords[i] * scalar ;
    }
    return *this;
}

ostream& operator<<(ostream& out, const Point& p) {
    out << "[";
    for (int i = 0; i < p.dimensions; ++i) {
        out << p.coords[i];
        if (i < p.dimensions - 1) out << ", ";
    }
    out << "]";
    return out;
}

istream& operator>>(istream& in, Point& p) {
    for (int i = 0; i < p.dimensions; ++i) {
        in >> p.coords[i];
    }
    return in;
}
