/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "Drawer2D.hpp"

Drawer2D::Drawer2D(const int width, const int height) : Drawer<2>(width, height) {}

void Drawer2D::draw(const Point2D& p) const {
    Point2D pv = toViewport(p);
    cairo::point(pv[0], pv[1]);
}

void Drawer2D::draw(const Line2D& ln) const {
    draw(Polygon2D(ln[0], ln[1]));
}

void Drawer2D::draw(const Polygon2D& p) const {
    Point2D pv;
    for (int i = 0; i < p.numberOfPoints(); i++ ) {
        pv = toViewport(p[i]);
        cairo::line_to(pv[0], pv[1]);
    }
    cairo::close_path();
    cairo::stroke();
}

void Drawer2D::drawAxis() {
    Line2D x(Point2D(0, (*viewport.second)[1]/2), 
             Point2D((*viewport.second)[0], (*viewport.second)[1]/2));
    Line2D y(Point2D((*viewport.second)[0]/2, 0), 
             Point2D((*viewport.second)[0]/2, (*viewport.second)[1]));
    draw(x);
    draw(y);
}

Point2D Drawer2D::toViewport(const Point2D& p) const {
    double x = (p[0] - (*window.first)[0])
               / ((*window.second)[0] - (*window.first)[0])
               * ((*viewport.second)[0] - (*viewport.first)[0])
               + (*viewport.first)[0];
    double y = (1 - (p[1] - (*window.first)[1])
               / ((*window.second)[1] - (*window.first)[1]))
               * ((*viewport.second)[1] - (*viewport.first)[1])
               + (*viewport.first)[1];
    return Point2D(x, y);
}
