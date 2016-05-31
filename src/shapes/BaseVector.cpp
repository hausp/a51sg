/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "BaseVector.hpp"
#include <cassert>
#include <cmath>

BaseVector::BaseVector() {
    coordinates.resize(2, 0);
}

BaseVector::BaseVector(size_t count) {
    coordinates.resize(count, 0);
}

BaseVector::BaseVector(size_t count, double value) {
    coordinates.resize(count, value);
}

BaseVector::BaseVector(std::initializer_list<double> list) {
    coordinates = list;
}

BaseVector::~BaseVector() {
    coordinates.clear();
}

size_t BaseVector::dimension() const {
    return coordinates.size();
}

double BaseVector::norm() const {
    double result = 0;
    for (unsigned i = 0; i < coordinates.size(); i++) {
        result += (*this)[i] * (*this)[i];
    }
    return sqrt(result);
}

double& BaseVector::operator[](size_t index) {
    return coordinates[index];
}

double BaseVector::operator[](size_t index) const {
    return coordinates[index];
}

typename std::vector<double>::iterator BaseVector::begin() {
    return coordinates.begin();
}

typename std::vector<double>::const_iterator BaseVector::begin() const {
    return coordinates.cbegin();
}

typename std::vector<double>::iterator BaseVector::end() {
    return coordinates.end();
}

typename std::vector<double>::const_iterator BaseVector::end() const {
    return coordinates.cend();
}

bool BaseVector::operator==(const BaseVector& rhs) const {
    return this->coordinates == rhs.coordinates;
}

bool BaseVector::operator!=(const BaseVector& rhs) const {
    return !(*this == rhs);
}


BaseVector& BaseVector::operator+=(const BaseVector& rhs) {
    assert(this->dimension() == rhs.dimension());
    for (unsigned i = 0; i < this->coordinates.size(); i++) {
        this->coordinates[i] += rhs.coordinates[i];
    }
    return *this;
}

BaseVector& BaseVector::operator-=(const BaseVector& rhs) {
    assert(this->dimension() == rhs.dimension());
    for (unsigned i = 0; i < this->coordinates.size(); i++) {
        this->coordinates[i] -= rhs.coordinates[i];
    }
    return *this;
}

BaseVector& BaseVector::operator+=(double rhs) {
    for (auto& c : coordinates) {
        c += rhs;
    }
    return *this;
}

BaseVector& BaseVector::operator-=(double rhs) {
    for (auto& c : coordinates) {
        c -= rhs;
    }
    return *this;
}

BaseVector& BaseVector::operator*=(double rhs) {
    for (auto& c : coordinates) {
        c *= rhs;
    }
    return *this;
}

BaseVector& BaseVector::operator/=(double rhs) {
    for (auto& c : coordinates) {
        c /= rhs;
    }
    return *this;
}

BaseVector BaseVector::operator+(const BaseVector& rhs) const {
    assert(this->dimension() == rhs.dimension());
    BaseVector re {*this};
    return re += rhs;
}

BaseVector BaseVector::operator-(const BaseVector& rhs) const {
    assert(this->dimension() == rhs.dimension());
    BaseVector re {*this};
    return re -= rhs;
}

BaseVector BaseVector::operator+(double rhs) const {
    BaseVector re {*this};
    return re += rhs;
}

BaseVector BaseVector::operator-(double rhs) const {
    BaseVector re {*this};
    return re -= rhs;
}

BaseVector BaseVector::operator*(double rhs) const {
    BaseVector re {*this};
    return re *= rhs;
}

BaseVector BaseVector::operator/(double rhs) const {
    BaseVector re {*this};
    return re /= rhs;
}

double BaseVector::operator*(const BaseVector& rhs) const {
    assert(this->dimension() == rhs.dimension());
    double sum = 0;
    for (unsigned i = 0; i < coordinates.size(); i++) {
        sum += this->coordinates[i] * rhs.coordinates[i];
    }
    return sum;
}

BaseVector BaseVector::operator-() const {
    return BaseVector(dimension()) - *this;
}

std::ostream& operator<<(std::ostream& out, const BaseVector& rhs) {
    out << '(';
    for (unsigned i = 0; i < rhs.dimension(); i++) {
        if (i > 0) {
            out << ',';
        }
        out << rhs[i];
    }
    out << ')';
    return out;
}