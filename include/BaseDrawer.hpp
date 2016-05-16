/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef BASE_DRAWER_HPP
#define BASE_DRAWER_HPP

#include <vector>
#include "Curve.hpp"
#include "Line.hpp"
#include "Point.hpp"
#include "Polygon.hpp"
#include "SimpleCurve.hpp"
#include "Window.hpp"

template<unsigned D>
class Drawable;
template<unsigned R, unsigned C>
class Matrix;

template<unsigned D>
class BaseDrawer {
 public:
    BaseDrawer();
    ~BaseDrawer();

    void addShape(Drawable<D>*);
    const std::vector<Drawable<D>*>& getDisplayFile();
    void clearDisplayFile();

    virtual void draw(Point<D>&) = 0;
    virtual void draw(Line<D>&) = 0;
    virtual void draw(Polygon<D>&) = 0;
    virtual void draw(SimpleCurve<D>&) = 0;
    virtual void draw(Curve<D>&) = 0;

    void drawAll();
    void removeShape(const size_t);
    void translate(const size_t, const std::array<double, D>&);
    void scale(const size_t, const std::array<double, D>&);
    void rotate(const size_t, const double, const Point<D>&);
    void highlightObject(const long);
    void swap(const std::vector<Drawable<D>*>&);

 protected:
    std::vector<Drawable<D>*> displayFile;
 private:
    long highlighted = -1;
};

#include "BaseDrawer.ipp"

#endif /* BASE_DRAWER_HPP */