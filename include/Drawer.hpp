
#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <vector>
#include "Point.hpp"
#include "Window.hpp"

template<unsigned D>
class Drawable;
template<unsigned D> 
class Line;
template<unsigned D>
class Polygon;
template<unsigned R, unsigned C>
class Matrix;

template<unsigned D>
class Drawer {
 public:
    Drawer(const int, const int);
    ~Drawer();

    void addShape(Drawable<D>*);
    void swap(const std::vector<Drawable<D>*>&);
    const std::vector<Drawable<D>*>& getDisplayFile();
    void clearDisplayFile();

    virtual void draw(Point<D>&) = 0;
    virtual void draw(Line<D>&) = 0;
    virtual void draw(Polygon<D>&) = 0;
    void drawAll();
    void moveVertical(const int);
    void moveHorizontal(const int);
    void removeShape(const unsigned long);
    void translate(const unsigned long, const std::array<double, D>&);
    void scale(const unsigned long, const std::array<double, D>&);
    void rotate(const unsigned long, const double, const int, const std::vector<std::string>&);
    void update(Drawable<D>*);
    void updateAll();
    void highlightObject(const long);

    void setZoom(const double);
    void setViewport(Point<2>, Point<2>);
    void zoom(const int);
    void resizeViewport(const double, const double);

 protected:
    Window window;
    std::pair<Point<2>, Point<2>> viewport;

 private:
    std::vector<Drawable<D>*> displayFile;
    long highlighted = -1;
    double zoomRate = 0.05;
    int moveSpeed = 10;
};

#include "Drawer.ipp"

#endif /* DRAWER_HPP */