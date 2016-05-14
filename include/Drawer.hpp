/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <vector>
#include "BaseDrawer.hpp"

using Point2D = Point<2>;
using Line2D = Line<2>;
using Polygon2D = Polygon<2>;
using SimpleCurve2D = SimpleCurve<2>;
using Curve2D = Curve<2>;

class Drawer : public BaseDrawer<2> {
 public:
    Drawer(const unsigned, const unsigned, const unsigned = 0);

    void draw(Point2D&);
    void draw(Line2D&);
    void draw(Polygon2D&);
    void draw(SimpleCurve2D&);
    void draw(Curve2D&);
    void drawAxis();
};

#endif /* DRAWER_HPP */