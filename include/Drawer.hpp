/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <vector>
#include "BaseDrawer.hpp"

using Point2D = Point<2>;
using Line2D = Line<2>;
using Polygon2D = Polygon<2>;
using Drawable2D = Drawable<2>;
using SimpleCurve2D = SimpleCurve<2>;
using Curve2D = Curve<2>;

using Point3D = Point<3>;
using Line3D = Line<3>;
using Polygon3D = Polygon<3>;
using SimpleCurve3D = SimpleCurve<3>;
using Curve3D = Curve<3>;

using SuperDrawer = BaseDrawer<2>;

class Drawer : public SuperDrawer {
 public:
    Drawer(const unsigned, const unsigned, const unsigned = 0);

    double getWindowAngle();
    double getZoomLevel();
    void setZoom(const double);
    void setViewport(Point2D, Point2D);
    void addShape(Drawable2D*);
    void draw(Point2D&);
    void draw(Line2D&);
    void draw(Polygon2D&);
    void draw(SimpleCurve2D&);
    void draw(Curve2D&);
    void drawAll();
    void moveVertical(const int);
    void moveHorizontal(const int);
    void resizeViewport(const double, const double);
    void rotate(const unsigned long, const double, const int,
                const std::vector<std::string>&);
    void rotateWindow(long);
    void scale(const unsigned long, const std::array<double, 2>&);
    void setClippingAlgorithm(const int);
    void swap(const std::vector<Drawable2D*>&);
    void translate(const unsigned long, const std::array<double, 2>&);
    void update(Drawable2D*);
    void updateAll();
    void zoom(const int);

 private:
    Window window;
    std::pair<Point2D, Point2D> viewport;
    Polygon2D wview;

    double zoomRate = 0.05;
    double rotationSpeed = 15;
    int moveSpeed = 10;
};

#endif /* DRAWER_HPP */