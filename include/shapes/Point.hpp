/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef POINT_HPP
#define POINT_HPP

#include <array>
#include <memory>
#include <ostream>
#include <vector>
#include "Drawable.hpp"

template<unsigned R, unsigned C>
class Matrix;

template<unsigned Dn, unsigned D>
struct is_valid {
    bool value = Dn >= D;
};

template<unsigned D>
class Point : public Drawable<D> {
 public:
    Point();
    Point(double);

    template<typename ...Args>
    Point(typename std::enable_if<sizeof...(Args)+1 == D,
          double>::type,
          const Args...);

    template<typename ...Args>
    Point(const std::string& name,
          typename std::enable_if<sizeof...(Args)+1 == D,
          double>::type,
          Args...);

    template<unsigned Dn>
    Point(const Point<Dn>&);

    void draw(BaseDrawer<D>&) override;
    void clip(Window&) override;
    void transform(const Matrix<D+1,D+1>&) override;
    Point<D> center() const override;
    Point<2>& ndc();
    std::vector<Point<D>> points() const override;
    void update(const Matrix<3,3>&, const Window& window) override;
    size_t dimension() const;
    double norm() const;

    double& operator[](size_t);
    double operator[](size_t) const;
    bool operator==(const Point<D>&) const;
    bool operator!=(const Point<D>&) const;
    Point<D>& operator+=(const Point<D>&);
    Point<D>& operator-=(const Point<D>&);
    Point<D>& operator*=(double);
    Point<D>& operator/=(double);
    Point<D> operator+(const Point<D>&) const;
    Point<D> operator-(const Point<D>&) const;
    Point<D> operator-() const;
    double operator*(const Point<D>&) const;
    Point<D> operator*(double) const;
    Point<D> operator/(double) const;
    typename std::array<double, D>::iterator begin();
    typename std::array<double, D>::const_iterator begin() const;
    typename std::array<double, D>::iterator end();
    typename std::array<double, D>::const_iterator end() const;
    std::array<double, D> toArray() const;

 private:
    std::array<double, D> coordinates;
    std::shared_ptr<Point<2>> normalized_point;

    template<typename ...Args>
    void init(unsigned pos, double, const Args...);
    void init(unsigned);
};

template<unsigned R, unsigned C>
Matrix<1,R+1> operator*(const Point<R>&, const Matrix<R+1,C>&);

template<unsigned R, unsigned C>
Point<R>& operator*=(Point<R>&, const Matrix<R+1,C>&);

template<unsigned D>
std::ostream& operator<<(std::ostream& stream, const Point<D>& p);

#include "Point.ipp"

#endif /* POINT_HPP */