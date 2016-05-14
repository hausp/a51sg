/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include <cmath>
#include "Drawer.hpp"

Drawer::Drawer(const unsigned width, const unsigned height, const unsigned border) 
: BaseDrawer<2>(width, height, border) { }

void Drawer::draw(Point2D& p) {
    if (!p.isVisible()) return;
    cairo::set_color(p.getColor());
    Point2D pv = window.toViewport(viewport, p);
    cairo::point(pv[0], pv[1]);
}

void Drawer::draw(Line2D& ln) {
    if (!ln.isVisible()) return;
    cairo::set_color(ln.getColor());
    auto p1 = window.toViewport(viewport, ln[0]);
    auto p2 = window.toViewport(viewport, ln[1]);
    cairo::move_to(p1[0], p1[1]);
    cairo::line_to(p2[0], p2[1]);
    cairo::stroke();
}

void Drawer::draw(Polygon2D& p) {
    if (!p.isVisible()) return;
    cairo::set_color(p.getColor());
    /*Point2D pv;
    for (unsigned i = 0; i < p.numberOfPoints(); i++ ) {
        pv = window.toViewport(viewport, p[i]);
        cairo::line_to(pv[0], pv[1]);
    }*/
    auto& points = p.ndc();
    for (auto point : points) {
        point = window.toViewport(viewport, point);
        cairo::line_to(point[0], point[1]);
    }
    cairo::close_path();
    //cairo::stroke_preserve();
    if (p.isFilled()) {
        cairo::fill();
    } else {
        cairo::stroke();
    }
}

void Drawer::draw(SimpleCurve2D& c) {
    if (!c.isVisible()) return;
    cairo::set_color(c.getColor());
    Line<2>* lastLine = nullptr;
    for (auto& line : c) {
        Point<2> newPoint;
        if (line.isVisible()) {
            newPoint = window.toViewport(viewport, line[0]);
            lastLine = &line;
            cairo::line_to(newPoint[0], newPoint[1]);                
        } else if (lastLine != nullptr) {
            newPoint = window.toViewport(viewport, (*lastLine)[1]);
            lastLine = nullptr;
            cairo::line_to(newPoint[0], newPoint[1]);                
        }
    }
    cairo::stroke();        
}

void Drawer::draw(Curve2D& curve) {
    for (auto& c : curve) {
        c.setColor(curve.getColor());
        draw(c);
    }
}

void Drawer::drawAxis() {
    Line2D x(Point2D(0, viewport.second[1]/2), 
             Point2D(viewport.second[0], viewport.second[1]/2));
    Line2D y(Point2D(viewport.second[0]/2, 0), 
             Point2D(viewport.second[0]/2, viewport.second[1]));
    draw(x);
    draw(y);
}
