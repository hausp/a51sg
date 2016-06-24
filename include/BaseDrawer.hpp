/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef BASE_DRAWER_HPP
#define BASE_DRAWER_HPP

#include <array>
#include <cstdlib>
#include <unordered_map>
#include <vector>

template<unsigned D>
class Drawable;
template<unsigned D>
class Point;
template<unsigned D>
class Line;
template<unsigned D>
class Polygon;
template<unsigned D>
class SimpleCurve;
template<unsigned D>
class Curve;
template<unsigned D>
class Wireframe;

class BicubicSurface;

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

    virtual void draw(Point<2>&) {}
    virtual void draw(Line<2>&) {}
    virtual void draw(Polygon<2>&) {}
    virtual void draw(SimpleCurve<2>&) {}
    virtual void draw(Curve<2>&) {}

    virtual void draw(Point<3>&) = 0;
    virtual void draw(Line<3>&) = 0;
    virtual void draw(Polygon<3>&) = 0;
    virtual void draw(SimpleCurve<3>&) = 0;
    virtual void draw(Curve<3>&) = 0;
    virtual void draw(Wireframe<3>&) = 0;
    virtual void draw(BicubicSurface&) = 0;

    void drawAll();
    void removeShape(size_t);
    void translate(size_t, const std::array<double, D>&);
    void scale(size_t, const std::array<double, D>&);
    void rotate(size_t, double, const Point<D>&);
    void highlightObject(long);
    void swap(const std::vector<Drawable<D>*>&);

    Point<2>& getNDC(Point<2>&);
    Point<2>& getNDC(Point<3>&);
    std::vector<Point<2>>& getNDC(Polygon<2>&);
    std::vector<Point<3>>& getNDC(Polygon<3>&);

 protected:
    std::vector<Drawable<D>*> displayFile;
    std::unordered_map<Point<2>*, Point<2>*> ndc2D;
    std::unordered_map<Point<3>*, Point<2>*> ndc3D;
    std::unordered_map<Polygon<2>*, std::vector<Point<2>>> ndc2DPolygons;
    std::unordered_map<Polygon<3>*, std::vector<Point<3>>> ndc3DPolygons;

 private:
    long highlighted = -1;
};

#include "BaseDrawer.ipp"

#endif /* BASE_DRAWER_HPP */