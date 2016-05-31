/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "Point.hpp"
#include "BaseDrawer.hpp"
#include "Matrix.hpp"
#include "Window.hpp"

template<unsigned D>
Point<D>::Point() : Drawable<D>("", DrawableType::Point) { 
    coordinates.fill(0);
}

template<unsigned D>
Point<D>::Point(double value) : Drawable<D>("", DrawableType::Point) {
    coordinates.fill(value);
}

template<unsigned D>
template<typename... Args>
Point<D>::Point(typename std::enable_if<sizeof...(Args)+1 == D,
      double>::type c,
      const Args... args) 
: Drawable<D>(std::string(""), DrawableType::Point) {
    init(0, c, args...);
}

template<unsigned D>
template<typename... Args>
Point<D>::Point(const std::string& name,
      typename std::enable_if<sizeof...(Args)+1 == D,
      double>::type c,
      Args... args) : Drawable<D>(name, DrawableType::Point) {
    init(0, c, args...);
}

template<unsigned D>
template<unsigned Dn>
Point<D>::Point(const Point<Dn>& p)
: Drawable<D>("", DrawableType::Point) {
    for (unsigned i = 0; i < D; i++) {
        coordinates[i] = (i < Dn) ? p[i] : 0;
    }
}

template<unsigned D>
void Point<D>::draw(BaseDrawer<D>& drawer) {
    drawer.draw(*this);
}

template<unsigned D>
void Point<D>::clip(Window& window) {
    window.clip(*this);
}

template<unsigned D>
void Point<D>::transform(const Matrix<D+1,D+1>& matrix) {
    *this *= matrix;
}

template<unsigned D>
Point<D> Point<D>::center() const {
    return *this;
}

template<unsigned D>
Point<2>& Point<D>::ndc() {
    if (!normalized_point)
        normalized_point = std::make_shared<Point<2>>(*this);
    return *normalized_point;
}

template<unsigned D>
std::vector<Point<D>> Point<D>::points() const {
    return {*this};
}

template<unsigned D>
void Point<D>::update(const Matrix<3,3>& matrix, const Window& window) {
    ndc() = window.perspectiveProjection(*this);
    ndc() *= matrix;
}

template<unsigned D>
size_t Point<D>::dimension() const {
    return coordinates.size();
}

template<unsigned D>
double Point<D>::norm() const {
    double result = 0;
    for (unsigned i = 0; i < D; i++) {
        result += (*this)[i] * (*this)[i];
    }
    return sqrt(result);
}

template<unsigned D>
double& Point<D>::operator[](size_t index) {
    return coordinates[index];
}

template<unsigned D>
double Point<D>::operator[](size_t index) const {
    return coordinates[index];
}

template<unsigned D>
bool Point<D>::operator==(const Point<D>& p) const {
    for (unsigned i = 0; i < D; i++) {
        if ((*this)[i] != p[i]) {
            return false;
        }
    }
    return true;
}

template<unsigned D>
bool Point<D>::operator!=(const Point<D>& p) const {
    return !(*this == p);
}

template<unsigned D>
Point<D>& Point<D>::operator+=(const Point<D>& p) {
    for (unsigned i = 0; i < D; i++) {
        (*this)[i] += p[i];
    }
    return *this;
}

template<unsigned D>
Point<D>& Point<D>::operator-=(const Point<D>& p) {
    for (unsigned i = 0; i < D; i++) {
        (*this)[i] -= p[i];
    }
    return *this;
}

template<unsigned D>
Point<D>& Point<D>::operator*=(double v) {
    for (unsigned i = 0; i < D; i++) {
        (*this)[i] *= v;
    }
    return *this;
}

template<unsigned D>
Point<D>& Point<D>::operator/=(double v) {
    for (unsigned i = 0; i < D; i++) {
        (*this)[i] /= v;
    }
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
double Point<D>::operator*(const Point<D>& p) const {
    double sum = 0;
    for (unsigned i = 0; i < D; i++) {
        sum += (*this)[i] * p[i];
    }
    return sum;
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
typename std::array<double, D>::iterator Point<D>::begin() {
    return coordinates.begin();
}

template<unsigned D>
typename std::array<double, D>::const_iterator Point<D>::begin() const {
    return coordinates.cbegin();
}

template<unsigned D>
typename std::array<double, D>::iterator Point<D>::end() {
    return coordinates.end();
}

template<unsigned D>
typename std::array<double, D>::const_iterator Point<D>::end() const {
    return coordinates.cend();
}

template<unsigned D>
std::array<double, D> Point<D>::toArray() const {
    return coordinates;
}

template<unsigned D>
template<typename ...Args>
void Point<D>::init(unsigned pos, double c, const Args... args) {
    coordinates[pos] = c;
    init(++pos, args...);
}

template<unsigned D>
void Point<D>::init(unsigned) {}

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
