/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Point.hpp"

template<unsigned D>
class Line;
template<unsigned D>
class Polygon;

using Viewport = std::pair<Point<2>, Point<2>>;

class Window {
 public:
    Window(const Point<2>&, const Point<2>&);
    Matrix<3, 3> normalizerMatrix();
    void moveHorizontal(const double);
    void moveVertical(const double);
    void rotate(const double);
    Point<2> toViewport(const Viewport&, Point<2>&);
    void zoom(const double);
    double getAngle();
    double getZoomLevel();
    void clip(Point<2>&);
    void clip(Line<2>&);
    void clip(Polygon<2>&);
    template<unsigned D>
    void clip(Point<D>&) {}
    template<unsigned D>
    void clip(Line<D>&) {}
    template<unsigned D>
    void clip(Polygon<D>&) {}

 private:
    Point<2> min;
    Point<2> max;
    double angle;
    double currentZoom;
    double defaultWidth;
    double defaultHeight;

    void clipCS(Line<2>&);
    void clipLB(Line<2>&);
};

#endif /* WINDOW_HPP */
