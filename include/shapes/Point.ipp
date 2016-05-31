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
    // for (unsigned i = 0; i < D; i++) {
    //     (*this)[i] += p[i];
    // }
    BaseVector::operator+=(p);
    return *this;
}

template<unsigned D>
Point<D>& Point<D>::operator-=(const Point<D>& p) {
    // for (unsigned i = 0; i < D; i++) {
    //     (*this)[i] -= p[i];
    // }
    BaseVector::operator-=(p);
    return *this;
}

template<unsigned D>
Point<D>& Point<D>::operator*=(double v) {
    // for (unsigned i = 0; i < D; i++) {
    //     (*this)[i] *= v;
    // }
    BaseVector::operator*=(v);
    return *this;
}

template<unsigned D>
Point<D>& Point<D>::operator/=(double v) {
    // for (unsigned i = 0; i < D; i++) {
    //     (*this)[i] /= v;
    // }
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


//-------------------------------- Point<2> ---------------------------------//

Point<2u>::Point()
: Drawable(DrawableType::Point), BaseVector(2) { }

Point<2>::Point(double value)
: Drawable(DrawableType::Point), BaseVector(2, value) { }

Point<2>::Point(double v1, double v2)
: Drawable(DrawableType::Point), BaseVector(v1, v2) { }

Point<2>::Point(const std::string& name, double v1, double v2)
: Drawable(name, DrawableType::Point), BaseVector(v1, v2) { }

void Point<2>::draw(BaseDrawer& drawer) {
    drawer.draw(*this);
}

void Point<2>::clip(Window& window) {
    window.clip(*this);
}

void Point<2>::transform(const BaseMatrix& tr) {
    ECHO("TODO");
    //*this *= matrix;
}

BaseVector Point<2>::center() const {
    return *this;
}

Point<2>& Point<2>::ndc() {
    if (!normalized_point)
        normalized_point = std::make_shared<Point<2>>(*this);
    return *normalized_point;
}

std::vector<BaseVector> Point<2>::points() const {
    return {*this};
}

void Point<2>::update(const BaseMatrix& tr, const Window& window) {
    ECHO("TODO");
    // ndc() = window.parallelProjection(*this);
    // ndc() *= matrix;
}

size_t Point<2>::dimension() const {
    return 2;
}

Point<2>& Point<2>::operator+=(const Point<2>& p) {
    // for (unsigned i = 0; i < D; i++) {
    //     (*this)[i] += p[i];
    // }
    BaseVector::operator+=(p);
    return *this;
}

Point<2>& Point<2>::operator-=(const Point<2>& p) {
    // for (unsigned i = 0; i < D; i++) {
    //     (*this)[i] -= p[i];
    // }
    BaseVector::operator-=(p);
    return *this;
}

Point<2>& Point<2>::operator*=(double v) {
    // for (unsigned i = 0; i < D; i++) {
    //     (*this)[i] *= v;
    // }
    BaseVector::operator*=(v);
    return *this;
}

Point<2>& Point<2>::operator/=(double v) {
    // for (unsigned i = 0; i < D; i++) {
    //     (*this)[i] /= v;
    // }
    BaseVector::operator/=(v);
    return *this;
}

Point<2> Point<2>::operator+(const Point<2>& p) const {
    Point<2> r = *this;
    return r += p;
}

Point<2> Point<2>::operator-(const Point<2>& p) const {
    Point<2> r = *this;
    return r -= p;
}

Point<2> Point<2>::operator-() const {
    return Point<2>() - *this;
}

Point<2> Point<2>::operator*(double v) const {
    Point<2> r = *this;
    return r *= v;
}

Point<2> Point<2>::operator/(double v) const {
    Point<2> r = *this;
    return r /= v;
}

double Point<2>::operator*(const Point<2>& p) const {
    return BaseVector::operator*(p);
}

std::array<double, 2> Point<2>::toArray() const {
    ECHO("TODO");
    //return coordinates;
}

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
