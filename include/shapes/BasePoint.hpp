/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef BASE_VECTOR_HPP
#define BASE_VECTOR_HPP

#include <cstdlib>
#include <vector>


template<unsigned R, unsigned C>
class Matrix;

class BaseVector {
 public:
    BaseVector();
    explicit BaseVector(size_t);
    BaseVector(const BaseVector&);
    template<typename... Args>
    BaseVector(Args...);

    ~BaseVector();

    const size_t dimension() const;

    double& operator[](size_t);
    const double& operator[](size_t) const;

    bool operator==(const BaseVector&) const;
    bool operator!=(const BaseVector&) const;

    BaseVector& operator+=(const BaseVector&);
    BaseVector& operator-=(const BaseVector&);
    BaseVector& operator*=(const double);
    BaseVector& operator/=(const double);

    BaseVector operator+(const BaseVector&) const;
    BaseVector operator-(const BaseVector&) const;
    BaseVector operator*(const double) const;
    BaseVector operator/(const double) const;

    typename std::vector<double>::iterator begin();
    typename std::vector<double>::const_iterator begin() const;
    typename std::vector<double>::iterator end();
    typename std::vector<double>::const_iterator end() const;

 private:
    std::vector<double> coordinates;

    template<typename... Args>
    void _init_coordinates(double, Args...);
    void _init_coordinates();
};

template<typename... Args>
BaseVector::BaseVector(Args... args) {
    _init_coordinates(args...);
}

template<typename... Args>
void BaseVector::_init_coordinates(double c1, Args... args) {
    coordinates.push_back(c1);
    _init_coordinates(args...);
}

void BaseVector::_init_coordinates() { }

#endif /* VECTOR_HPP */