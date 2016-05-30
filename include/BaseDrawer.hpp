/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef BASE_DRAWER_HPP
#define BASE_DRAWER_HPP

#include <array>
#include <cstdlib>
#include <vector>

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
template<unsigned R, unsigned C>
class Matrix;
class BaseVector;

class BaseDrawer {
 public:
    BaseDrawer();
    ~BaseDrawer();

    void addShape(Drawable*);
    const std::vector<Drawable*>& getDisplayFile();
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

    void drawAll();
    void removeShape(size_t);
    template<size_t D>
    void translate(size_t, const std::array<double, D>&);
    template<size_t D>
    void scale(size_t, const std::array<double, D>&);
    void rotate(size_t, double, const BaseVector&);
    void highlightObject(long);
    void swap(const std::vector<Drawable*>&);

 protected:
    std::vector<Drawable*> displayFile;
    std::vector<Drawable*> normalizedDisplayFile;
 private:
    long highlighted = -1;
};

#include "BaseDrawer.ipp"

#endif /* BASE_DRAWER_HPP */