
#ifndef DRAWER2D_HPP
#define DRAWER2D_HPP

#include <vector>
#include "Drawer.hpp"

using Point2D = Point<2>;
using Line2D = Line<2>;
using Polygon2D = Polygon<2>;

class Drawer2D : public Drawer<2> {
 public:
    Drawer2D(const unsigned, const unsigned, const unsigned = 0);

    void draw(Point2D&);
    void draw(Line2D&);
    void draw(Polygon2D&);
    void drawAxis();
};

#endif /* DRAWER2D_HPP */