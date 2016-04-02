
#ifndef DRAWER2D_HPP
#define DRAWER2D_HPP

#include <vector>
#include "Drawer.hpp"

using Point2D = Point<2>;
using Line2D = Line<2>;
using Polygon2D = Polygon<2>;

class Drawer2D : public Drawer<2> {
 public:
    Drawer2D(const int, const int);

    void draw(const Point2D&) const;
    void draw(const Line2D&) const;
    void draw(const Polygon2D&) const;
    void drawAxis() override;

 private:
    Point2D toViewport(const Point2D&) const;
};

#endif /* DRAWER2D_HPP */