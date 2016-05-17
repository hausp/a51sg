/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <list>
#include <memory>
#include "Point.hpp"

// template<unsigned D>
// class Line;
template<unsigned D>
class Polygon;
template<unsigned D>
class Curve;

using Viewport = std::pair<Point<2>, Point<2>>;

class Window {
 public:
    Window(const Point<2>&, const Point<2>&);
    Matrix<3, 3> normalizerMatrix();
    void moveHorizontal(const double);
    void moveVertical(const double);
    void rotate(const double);
    template<unsigned D>
    Point<D> toViewport(const Viewport&, Point<D>&);
    void zoom(const double);
    double getAngle();
    double getZoomLevel();
    void clip(Point<2>&);
    void clip(Line<2>&);
    void clip(Polygon<2>&);
    void clip(SimpleCurve<2>&);
    void clip(Curve<2>&);
    template<unsigned D>
    void clip(Point<D>&) {}
    template<unsigned D>
    void clip(Line<D>&) {}
    template<unsigned D>
    void clip(Polygon<D>&) {}
    template<unsigned D>
    void clip(Curve<D>&) {}
    void setClippingAlgorithm(const int);

    Point<2> parallelProjection(Point<2> p) const;
    Point<2> parallelProjection(Point<3> p) const;

 private:
    Point<2> min;
    Point<2> max;
    double angle;
    double currentZoom;
    double defaultWidth;
    double defaultHeight;
    unsigned lcAlgorithm;
    std::unique_ptr<Line<3>> vpn;

    void clipCS(Line<2>&);
    void clipLB(Line<2>&);
    void clipNLN(Line<2>&);
    bool leftcolumn(Point<2>&, Point<2>&);
    bool topleftcorner(Point<2>&, Point<2>&);
    bool leftbottomregion(Point<2>&, Point<2>&, double);
    bool leftedge(Point<2>&, Point<2>&);
    bool p2bottom(Point<2>&, Point<2>&);
    bool centercolumn(Point<2>&, Point<2>&);
    bool inside(Point<2>&, Point<2>&);
    void p2left(Point<2>&, Point<2>&);
    void p2lefttop(Point<2>&, Point<2>&);
    void clockwiseSort(Polygon<2>&);
    void listInsert(std::list<Point<2>>&, unsigned, const Point<2>&);
    void buildLists(Polygon<2>&, std::list<Point<2>>&,
        std::vector<Point<2>>&, std::vector<Point<2>>&, std::vector<Point<2>>&);
    void athertonStep(const std::vector<Point<2>>&, const std::vector<Point<2>>&,
        const std::vector<Point<2>>&, const Point<2>&, std::vector<Point<2>>&);
};

template<unsigned D>
Point<D> Window::toViewport(const Viewport& viewport, Point<D>& p) {
    double width  = viewport.second[0] - viewport.first[0];
    double height = viewport.second[1] - viewport.first[1];
    auto& pn = p.ndc();
    double x = (pn[0] + 1) / 2 * width + viewport.first[0];
    double y = (1 - (pn[1] + 1)/ 2) * height + viewport.first[1];
    return Point<D>(x, y);
}

#endif /* WINDOW_HPP */
