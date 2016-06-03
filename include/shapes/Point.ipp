/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "Point.hpp"
#include "BaseDrawer.hpp"
#include "BaseMatrix.hpp"
#include "Matrix.hpp"
#include "Window.hpp"

template<unsigned D>
Point<D>::Point()
: Drawable(DrawableType::Point), BaseVector(D) { }

template<unsigned D>
Point<D>::Point(double value)
: Drawable(DrawableType::Point), BaseVector(D, value) { }

template<unsigned D>
template<typename... Args>
Point<D>::Point(typename std::enable_if<sizeof...(Args)+1 == D,
      double>::type c,
      const Args... args) 
: Drawable(DrawableType::Point), BaseVector(D) {
    init(0, c, args...);
}

template<unsigned D>
template<typename... Args>
Point<D>::Point(const std::string& name,
      typename std::enable_if<sizeof...(Args)+1 == D,
      double>::type c,
      Args... args)
: Drawable(name, DrawableType::Point), BaseVector(D) {
    init(0, c, args...);
}

template<unsigned D>
size_t Point<D>::dimension() const {
    return D;
}

template<unsigned D>
Point<D>& Point<D>::operator+=(const Point<D>& p) {
    BaseVector::operator+=(p);
    return *this;
}

template<unsigned D>
Point<D>& Point<D>::operator-=(const Point<D>& p) {
    BaseVector::operator-=(p);
    return *this;
}

template<unsigned D>
Point<D>& Point<D>::operator*=(double v) {
    BaseVector::operator*=(v);
    return *this;
}

template<unsigned D>
Point<D>& Point<D>::operator/=(double v) {
    BaseVector::operator/=(v);
    return *this;
}

template<unsigned D>
Point<D> Point<D>::operator+(const Point<D>& p) const {
    Point<D> r = *this;
    return r += p;
}

template<unsigned D>
Point<D> Point<D>::operator-(const Point<D>& p) const {
    Point<D> r = *this;
    return r -= p;
}

template<unsigned D>
Point<D> Point<D>::operator-() const {
    return Point<D>() - *this;
}

template<unsigned D>
Point<D> Point<D>::operator*(double v) const {
    Point<D> r = *this;
    return r *= v;
}

template<unsigned D>
Point<D> Point<D>::operator/(double v) const {
    Point<D> r = *this;
    return r /= v;
}

template<unsigned D>
std::array<double, D> Point<D>::toArray() const {
    //return coordinates;
}

template<unsigned D>
template<typename ...Args>
void Point<D>::init(unsigned pos, double c, const Args... args) {
    (*this)[pos] = c;
    init(++pos, args...);
}

template<unsigned D>
void Point<D>::init(unsigned) {}

//------------------------------- non-member --------------------------------//

template<unsigned R, unsigned C>
Matrix<1,R+1> operator*(const Point<R>& p, const Matrix<R+1,C>& m) {
    Matrix<1,R+1> r;
    for (unsigned i = 0; i < R + 1; i++) {
        double value = (i < R) ? p[i] : 1;
        r[0][i] = value;
    }
    return r * m;
}

template<unsigned R, unsigned C>
Point<R>& operator*=(Point<R>& p, const Matrix<R+1,C>& m) {
    Matrix<1,R+1> r = p * m;
    for (unsigned i = 0; i < R; i++) {
        p[i] = r[0][i];
    }
    return p;
}

template<unsigned D>
std::ostream& operator<<(std::ostream& stream, const Point<D>& p) {
    stream << '(';
    for (unsigned i = 0; i < D; i++) {
        if (i > 0) {
            stream << ',';
        }
        stream << p[i];
    }
    stream << ')';
    return stream;
}
