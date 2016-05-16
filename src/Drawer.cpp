/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include <cmath>
#include "Drawer.hpp"
#include "cairo.hpp"

Drawer::Drawer(const unsigned width, const unsigned height, const unsigned border)
: window(Point2D(0,0), Point2D(width - 2 * border, height - 2 * border)) {
    viewport = std::make_pair(Point2D(border, border),
                              Point2D(width - border, height - border));

    wview = {Point2D(0, 0),
             Point2D(0, height - 2 * border),
             Point2D(width - 2 * border, height - 2 * border),
             Point2D(width - 2 * border, 0)};

    wview.setVisible(true);
    wview.update(window.normalizerMatrix());
    wview.setColor(177, 0, 0);
}

void Drawer::addShape(Drawable2D* d) {
    SuperDrawer::addShape(d);
    update(d);
}

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
    auto& points = p.ndc();
    for (auto point : points) {
        point = window.toViewport(viewport, point);
        cairo::line_to(point[0], point[1]);
    }
    cairo::close_path();
    if (p.isFilled()) {
        cairo::fill();
    } else {
        cairo::stroke();
    }
}

void Drawer::draw(SimpleCurve2D& c) {
    if (!c.isVisible()) return;
    cairo::set_color(c.getColor());
    Line2D* lastLine = nullptr;
    for (auto& line : c) {
        Point2D newPoint;
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

void Drawer::drawAll() {
    cairo::clear();
    SuperDrawer::drawAll();
    draw(wview);
}

// void Drawer::drawAxis() {
//     Line2D x(Point2D(0, viewport.second[1]/2), 
//              Point2D(viewport.second[0], viewport.second[1]/2));
//     Line2D y(Point2D(viewport.second[0]/2, 0), 
//              Point2D(viewport.second[0]/2, viewport.second[1]));
//     draw(x);
//     draw(y);
// }

double Drawer::getWindowAngle() {
    return window.getAngle();
}

double Drawer::getZoomLevel() {
    return window.getZoomLevel();
}

void Drawer::moveHorizontal(const int direction) {
    window.moveHorizontal(direction * moveSpeed);
    updateAll();
}

void Drawer::moveVertical(const int direction) {
    window.moveVertical(direction * moveSpeed);
    updateAll();
}

void Drawer::rotateWindow(long direction) {
    window.rotate(direction * rotationSpeed);
    updateAll();
}

void Drawer::translate(const unsigned long index, const std::array<double, 2>& ds) {
    if (index < displayFile.size()) {
        SuperDrawer::translate(index, ds);
        update(displayFile[index]);
        drawAll();
    }
}

void Drawer::scale(const unsigned long index, const std::array<double, 2>& ss) {
    if (index < displayFile.size()) {
        SuperDrawer::scale(index, ss);
        update(displayFile[index]);
        drawAll();
    }
}

void Drawer::rotate(const unsigned long index, const double angle,
                    const int type, const std::vector<std::string>& entries) {
    if (index < displayFile.size()) {
        Point2D axis;
        auto& shape = displayFile[index];
        switch (type) {
            case 0:
                axis = shape->center();
                break;
            case 1:
                axis = {0, 0};
                break;
            case 2:
                axis = {stod(entries[0]), stod(entries[1])};
                break;
        }
        SuperDrawer::rotate(index, angle, axis);
        update(displayFile[index]);
        drawAll();
    }
}

void Drawer::resizeViewport(const double length, const double height) {
    //viewport.second[0] = length;
    //viewport.second[1] = height;
    draw(wview);
}

void Drawer::setZoom(const double rate) {
    zoomRate = rate;
}

void Drawer::setViewport(Point2D v1, Point2D v2) {
    viewport = std::make_pair(v1, v2);
}

void Drawer::setClippingAlgorithm(const int algorithm) {
    window.setClippingAlgorithm(algorithm);
}

void Drawer::swap(const std::vector<Drawable2D*>& newDisplayFile) {
    SuperDrawer::swap(newDisplayFile);
    updateAll();
}

void Drawer::update(Drawable2D* shape) {
    shape->update(window.normalizerMatrix());
    shape->clip(window);
}

void Drawer::updateAll() {
    auto normalizer = window.normalizerMatrix();
    for (auto shape : displayFile) {
        shape->update(normalizer);
        shape->clip(window);
    }
}

void Drawer::zoom(const int d) {
    window.zoom(d * zoomRate);
    updateAll();
}
