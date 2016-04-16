/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef POINT_HPP
#define POINT_HPP

#include <array>
#include <vector>
#include "Drawable.hpp"

template<unsigned R, unsigned C>
class Matrix;

template<unsigned D>
class Point : public Drawable<D> {
 public:
    Point();
    Point(const double);
    Point(const Point&);

    template<typename ...Args>
    Point(typename std::enable_if<sizeof...(Args)+1 == D,
          const double>::type,
          const Args...);

    template<typename ...Args>
    Point(const std::string& name,
          typename std::enable_if<sizeof...(Args)+1 == D,
          const double>::type,
          Args...);

    ~Point();

    void draw(Drawer<D>&) override;
    void clip(Window&) override;
    void transform(const Matrix<D+1,D+1>&) override;
    Point<D> center() const override;
    Point<D>& ndc();
    std::vector<Point<D>> points() const override;
    void update(const Matrix<D+1,D+1>&) override;
    const size_t dimension() const;
    double& operator[](size_t);
    const double& operator[](size_t) const;
    bool operator==(const Point<D>&);
    Point<D>& operator+=(const Point<D>&);
    Point<D>& operator-=(const Point<D>&);
    Point<D>& operator*=(const double);
    Point<D>& operator/=(const double);
    Point<D> operator+(const Point<D>&) const;
    Point<D> operator-(const Point<D>&) const;
    Point<D> operator*(const double) const;
    Point<D> operator/(const double) const;
    typename std::vector<double>::iterator begin();
    typename std::vector<double>::const_iterator begin() const;
    typename std::vector<double>::iterator end();
    typename std::vector<double>::const_iterator end() const;
    std::array<double, D> toArray();

 private:
    std::array<double, D> coordinates;
    Point<D>* normalized_point = NULL;

    template<typename ...Args>
    void init(unsigned pos, const double, const Args...);
    void init(unsigned);
};

template<unsigned R, unsigned C>
Matrix<1,R+1> operator*(const Point<R>&, const Matrix<R+1,C>&);

template<unsigned R, unsigned C>
Point<R>& operator*=(Point<R>&, const Matrix<R+1,C>&);

#include "Point.ipp"

#endif /* POINT_HPP */