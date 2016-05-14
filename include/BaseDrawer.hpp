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
    BaseDrawer(const unsigned, const unsigned, const unsigned = 0);
    ~BaseDrawer();

    void addShape(Drawable<D>*);
    void swap(const std::vector<Drawable<D>*>&);
    const std::vector<Drawable<D>*>& getDisplayFile();
    void clearDisplayFile();

    double getWindowAngle();
    double getZoomLevel();

    virtual void draw(Point<D>&) = 0;
    virtual void draw(Line<D>&) = 0;
    virtual void draw(Polygon<D>&) = 0;
    virtual void draw(SimpleCurve<D>&) = 0;
    virtual void draw(Curve<D>&) = 0;
    void drawAll();
    void moveVertical(const int);
    void moveHorizontal(const int);
    void removeShape(const unsigned long);
    void translate(const unsigned long, const std::array<double, D>&);
    void scale(const unsigned long, const std::array<double, D>&);
    void rotate(const unsigned long, const double, const int, const std::vector<std::string>&);
    void rotateWindow(long);
    void update(Drawable<D>*);
    void updateAll();
    void highlightObject(const long);

    void setZoom(const double);
    void setViewport(Point<2>, Point<2>);
    void zoom(const int);
    void resizeViewport(const double, const double);
    void setClippingAlgorithm(const int);

    void drawViewportBorders();

 protected:
    Window window;
    std::pair<Point<2>, Point<2>> viewport;
    Polygon<2> wview;

 private:
    std::vector<Drawable<D>*> displayFile;
    long highlighted = -1;
    double zoomRate = 0.05;
    double rotationSpeed = 15;
    int moveSpeed = 10;
};

#include "BaseDrawer.ipp"

#endif /* BASE_DRAWER_HPP */