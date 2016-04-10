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

    Point(const Point& point);

    template<typename ...Args>
    Point(typename std::enable_if<sizeof...(Args)+1 == D,
          const double>::type c,
          const Args... args);

    template<typename ...Args>
    Point(const std::string& name,
          typename std::enable_if<sizeof...(Args)+1 == D,
          const double>::type c,
          Args... args);

    ~Point();

    void draw(Drawer<D>& drawer) override;

    void transform(const Matrix<D+1,D+1>& matrix) override;

    Point<D> center() const override;

    Point<D>& ndc();

    std::vector<Point<D>> points() const override;

    const size_t dimension() const;

    double& operator[](size_t index);
    
    const double& operator[](size_t index) const;

    bool operator==(const Point<D>& p);

    Point<D>& operator+=(const Point<D>& p);

    Point<D>& operator-=(const Point<D>& p);

    Point<D>& operator*=(const double v);

    Point<D>& operator/=(const double v);

    Point<D> operator+(const Point<D>& p) const;

    Point<D> operator-(const Point<D>& p) const;

    Point<D> operator*(const double v) const;

    Point<D> operator/(const double v) const;

    typename std::vector<double>::iterator begin();

    typename std::vector<double>::const_iterator begin() const;

    typename std::vector<double>::iterator end();
    
    typename std::vector<double>::const_iterator end() const;

    std::array<double, D> toArray();

 private:
    std::array<double, D> coordinates;
    Point<D>* normalized_point = NULL;

    template<typename ...Args>
    void init(unsigned pos, const double c, const Args... args);
    void init(unsigned);
};



template<unsigned R, unsigned C>
Matrix<1,R+1> operator*(const Point<R>& p, const Matrix<R+1,C>& m);

template<unsigned R, unsigned C>
Point<R>& operator*=(Point<R>& p, const Matrix<R+1,C>& m);

#include "Point.ipp"

#endif /* POINT_HPP */