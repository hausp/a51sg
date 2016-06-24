/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <vector>
#include "BaseDrawer.hpp"
#include "Point.hpp"
#include "Polygon.hpp"
#include "Window.hpp"
#include "Wireframe.hpp"
#include "BicubicSurface.hpp"

using Point2D = Point<2>;
using Line2D = Line<2>;
using Polygon2D = Polygon<2>;
using Drawable2D = Drawable<2>;
using SimpleCurve2D = SimpleCurve<2>;
using Curve2D = Curve<2>;

using Point3D = Point<3>;
using Line3D = Line<3>;
using Polygon3D = Polygon<3>;
using Drawable3D = Drawable<3>;
using SimpleCurve3D = SimpleCurve<3>;
using Curve3D = Curve<3>;
using Wireframe3D = Wireframe<3>;

using SuperDrawer = BaseDrawer<3>;

class Drawer : public SuperDrawer {
 public:
    Drawer(unsigned, unsigned, unsigned = 0);

    double getWindowAngle();
    double getZoomLevel();
    void setZoom(double);
    void setViewport(Point2D, Point2D);
    void addShape(Drawable3D*);

    void draw(Point3D&);
    void draw(Line3D&);
    void draw(Polygon3D&);
    void draw(SimpleCurve3D&);
    void draw(Curve3D&);
    void draw(Wireframe3D&);
    void draw(BicubicSurface&);
    void drawAll();

    void moveVertical(int);
    void moveHorizontal(int);
    void resizeViewport(double, double);
    void rotate(unsigned long, double, int,
                const std::vector<std::string>&);
    void rotateWindow(long);
    void scale(unsigned long, const std::array<double, 3>&);
    void setClippingAlgorithm(int);
    void swap(const std::vector<Drawable3D*>&);
    void translate(unsigned long, const std::array<double, 3>&);
    void update(Drawable3D*);
    void updateAll();
    void zoom(int);

    void updateShape(Point3D*, const Matrix<3,3>&);
    void updateShape(Line3D*, const Matrix<3,3>&);
    void updateShape(Polygon3D*, const Matrix<3,3>&);
    void updateShape(SimpleCurve3D*, const Matrix<3,3>&);
    void updateShape(Curve3D*, const Matrix<3,3>&);
    void updateShape(Wireframe3D*, const Matrix<3,3>&);
    void updateShape(BicubicSurface*, const Matrix<3,3>&);

 private:
    Window window;
    std::pair<Point2D, Point2D> viewport;
    Polygon3D wview;

    double zoomRate = 0.05;
    double rotationSpeed = 15;
    int moveSpeed = 10;
};

#endif /* DRAWER_HPP */