/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "Point.hpp"
#include "BaseDrawer.hpp"
#include "BaseMatrix.hpp"
#include "Matrix.hpp"
#include "Window.hpp"

//-------------------------------- Point<2> ---------------------------------//

Point<2u>::Point()
: Drawable(DrawableType::Point), BaseVector(2) { }

Point<2>::Point(double value)
: Drawable(DrawableType::Point), BaseVector(2, value) { }

Point<2>::Point(double v1, double v2)
: Drawable(DrawableType::Point), BaseVector{{v1, v2}} { }

Point<2>::Point(const std::string& name, double v1, double v2)
: Drawable(name, DrawableType::Point), BaseVector(v1, v2) { }

void Point<2>::draw(BaseDrawer& drawer) {
    drawer.draw(*this);
}

void Point<2>::clip(Window& window) {
    window.clip(*this);
}

void Point<2>::transform(const BaseMatrix& matrix) {
    *this *= matrix;
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

void Point<2>::update(const BaseMatrix& matrix, const Window& window) {
    ndc() = window.parallelProjection(*this);
    ndc() *= matrix;
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
    std::array<double, 2> array;
    array[0] = (*this)[0];
    array[1] = (*this)[1];
    return array;
}
