
#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <vector>
#include "Matrix.hpp"

template<unsigned D>
class Drawable;
template<unsigned D> 
class Point;
template<unsigned D>
class Line;
template<unsigned D>
class Polygon;

template<unsigned D>
class Drawer {
 public:
    Drawer(const int, const int);
    ~Drawer();

    void addShape(Drawable<D>*);
    virtual void draw(const Point<D>&) const = 0;
    virtual void draw(const Line<D>&) const = 0;
    virtual void draw(const Polygon<D>&) const = 0;
    void drawAll();
    virtual void drawAxis() = 0;
    void moveVertical(const int);
    void moveHorizontal(const int);
    void removeShape(const long);
    void translate(const long, const double, const double);
    void scale(const long, const double, const double);
    void rotate(const long, const double, const int, const std::vector<std::string>&);
    Matrix<D+1,D+1> translationMatrix(const double, const double);
    Matrix<D+1,D+1> scalingMatrix(const double, const double);
    Matrix<D+1,D+1> rotationMatrix(const double);

    void setZoom(const double);
    void setViewport(Point<2>*, Point<2>*);
    void zoom(const int);
    void resizeViewport(const double, const double);

 protected:
    std::pair<Point<2>*, Point<2>*> window;
    std::pair<Point<2>*, Point<2>*> viewport;

 private:
    std::vector<Drawable<D>*> displayFile;
    double defaultWidth;
    double defaultHeight;
    double zoomRate = 0.05;
    int moveSpeed = 10;
};

#include "Drawer.ipp"

#endif /* DRAWER_HPP */