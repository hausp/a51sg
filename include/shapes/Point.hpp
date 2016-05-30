/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef POINT_HPP
#define POINT_HPP

#include <array>
#include <memory>
#include <ostream>
#include <vector>
#include "BaseVector.hpp"
#include "Drawable.hpp"

class Window;
class BaseDrawer;
class BaseTransformation;

#define double_if typename std::enable_if<sizeof...(Args)+1 == D, double>::type

template<unsigned R, unsigned C>
class Matrix;

template<unsigned D>
class Point : public Point<D-1> {
 public:
    Point();
    explicit Point(double);

    template<typename ...Args>
    Point(double_if, const Args...);

    template<typename ...Args>
    Point(const std::string& name, double_if, Args...);

    template<unsigned Dn>
    Point(const Point<Dn>&);

    Point<D>& operator+=(const Point<D>&);
    Point<D>& operator-=(const Point<D>&);
    Point<D>& operator*=(double);
    Point<D>& operator/=(double);
    Point<D> operator+(const Point<D>&) const;
    Point<D> operator-(const Point<D>&) const;
    Point<D> operator-() const;
    Point<D> operator*(double) const;
    Point<D> operator/(double) const;
    double operator*(const Point<D>&) const;
    std::array<double, D> toArray() const;

    using BaseVector::operator*;

 private:
    template<typename ...Args>
    void init(unsigned pos, double, const Args...);
    void init(unsigned);
};

template<>
class Point<2> : virtual public Drawable, virtual public BaseVector {
 public:
    Point();
    Point(double, double);
    Point(const std::string&, double, double);
    explicit Point(double);

    void draw(BaseDrawer&) override;
    void clip(Window&) override;
    void transform(const BaseTransformation&) override;
    BaseVector center() const override;
    Point<2>& ndc();
    std::vector<BaseVector> points() const override;
    void update(const BaseTransformation&, const Window& window) override;

    Point<2>& operator+=(const Point<2>&);
    Point<2>& operator-=(const Point<2>&);
    Point<2>& operator*=(double);
    Point<2>& operator/=(double);
    Point<2> operator+(const Point<2>&) const;
    Point<2> operator-(const Point<2>&) const;
    Point<2> operator-() const;
    Point<2> operator*(double) const;
    Point<2> operator/(double) const;
    double operator*(const Point<2>&) const;
    std::array<double, 2> toArray() const;

    using BaseVector::operator*;

 private:
    std::shared_ptr<Point<2>> normalized_point;
};

template<>
class Point<1> { };

template<>
class Point<0> { };

template<unsigned R, unsigned C>
Matrix<1,R+1> operator*(const Point<R>&, const Matrix<R+1,C>&);

template<unsigned R, unsigned C>
Point<R>& operator*=(Point<R>&, const Matrix<R+1,C>&);

#include "Point.ipp"

#endif /* POINT_HPP */