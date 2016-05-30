/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef BASE_VECTOR_HPP
#define BASE_VECTOR_HPP

#include <cstdlib>
#include <vector>
#include <ostream>

class BaseVector {
 public:
    BaseVector();
    explicit BaseVector(size_t);
    BaseVector(size_t, double);
    BaseVector(std::initializer_list<double>);

    ~BaseVector();

    size_t dimension() const;
    double norm() const;

    double& operator[](size_t);
    const double& operator[](size_t) const;
    typename std::vector<double>::iterator begin();
    typename std::vector<double>::const_iterator begin() const;
    typename std::vector<double>::iterator end();
    typename std::vector<double>::const_iterator end() const;

    bool operator==(const BaseVector&) const;
    bool operator!=(const BaseVector&) const;

    BaseVector& operator+=(const BaseVector&);
    BaseVector& operator-=(const BaseVector&);
    BaseVector& operator+=(double);
    BaseVector& operator-=(double);
    BaseVector& operator*=(double);
    BaseVector& operator/=(double);

    BaseVector operator+(const BaseVector&) const;
    BaseVector operator-(const BaseVector&) const;
    BaseVector operator+(double) const;
    BaseVector operator-(double) const;
    BaseVector operator*(double) const;
    BaseVector operator/(double) const;
    BaseVector operator-() const;

    double operator*(const BaseVector&) const;

 private:
    std::vector<double> coordinates;
};

std::ostream& operator<<(std::ostream& out, const BaseVector& rhs);

#endif /* VECTOR_HPP */