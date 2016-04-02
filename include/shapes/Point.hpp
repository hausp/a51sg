/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef POINT_HPP
#define POINT_HPP

#include <vector>

#include "Drawable.hpp"
#include "Drawer.hpp"
#include "Matrix.hpp"

template<unsigned D>
class Point : public Drawable<D> {
 public:

    Point() : Drawable<D>("", DrawableType::Point) { 
        for (unsigned i = 0; i < D; i++) {
            coordinates.push_back(0);
        }
    }

    Point(const Point& point) : Drawable<D>("", DrawableType::Point) {
        for (auto c : point) {
            coordinates.push_back(c);
        }
    }

    template<typename ...Args>
    Point(typename std::enable_if<sizeof...(Args)+1 == D,
          const double>::type c,
          const Args... args) 
    : Drawable<D>(std::string(""), DrawableType::Point) {
        init(c, args...);
    }

    template<typename ...Args>
    Point(const std::string& name,
          typename std::enable_if<sizeof...(Args)+1 == D,
          const double>::type c,
          Args... args) : Drawable<D>(name, DrawableType::Point) {
        init(c, args...);
    }

    ~Point() {}

    void draw(Drawer<D>& drawer) override {
        drawer.draw(*this);
    }

    void transform(const Matrix<D+1,D+1>& matrix) override {
        *this *= matrix;
    }

    Point<D> center() const override {
        return *this;
    }

    const size_t dimension() const {
        return coordinates.size();
    }

    double& operator[](size_t index) {
        return coordinates[index];
    }
    
    const double& operator[](size_t index) const {
        return coordinates[index];
    }

    Point<D>& operator+=(const Point<D>& p) {
        for (unsigned i = 0; i < D; i++) {
            (*this)[i] += p[i];
        }
        return *this;
    }

    Point<D>& operator-=(const Point<D>& p) {
        for (unsigned i = 0; i < D; i++) {
            (*this)[i] -= p[i];
        }
        return *this;
    }

    Point<D>& operator*=(const double v) {
        for (unsigned i = 0; i < D; i++) {
            (*this)[i] *= v;
        }
        return *this;
    }

    Point<D>& operator/=(const double v) {
        for (unsigned i = 0; i < D; i++) {
            (*this)[i] /= v;
        }
        return *this;
    }

    Point<D> operator+(const Point<D>& p) const {
        Point<D> r = *this;
        return r += p;
    }

    Point<D> operator-(const Point<D>& p) const {
        Point<D> r = *this;
        return r -= p;
    }

    Point<D> operator*(const double v) const {
        Point<D> r = *this;
        return r *= v;
    }

    Point<D> operator/(const double v) const {
        Point<D> r = *this;
        return r /= v;
    }

    typename std::vector<double>::iterator begin() {
        return coordinates.begin();
    }

    typename std::vector<double>::const_iterator begin() const {
        return coordinates.cbegin();
    }

    typename std::vector<double>::iterator end() {
        return coordinates.end();
    }
    
    typename std::vector<double>::const_iterator end() const {
        return coordinates.cend();
    }

 private:
    std::vector<double> coordinates;

    template<typename ...Args>
    void init(const double c, const Args... args) {
        coordinates.push_back(c);
        init(args...);
    }
    void init() {}
};

template<unsigned R, unsigned C>
inline Matrix<1,R+1> operator*(const Point<R>& p, const Matrix<R+1,C>& m) {
    Matrix<1,R+1> r;
    for (unsigned i = 0; i < R + 1; i++) {
        double value = (i < R) ? p[i] : 1;
        r[0][i] = value;
    }
    return r * m;
}

template<unsigned R, unsigned C>
inline Point<R>& operator*=(Point<R>& p, const Matrix<R+1,C>& m) {
    Matrix<1,R+1> r = p * m;
    for (unsigned i = 0; i < R; i++) {
        p[i] = r[0][i];
    }
    return p;
}

#endif /* POINT_HPP */