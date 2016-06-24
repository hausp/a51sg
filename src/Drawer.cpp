/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include <cmath>
#include "Drawer.hpp"
#include "cairo.hpp"

Drawer::Drawer(unsigned width, unsigned height, unsigned border)
: window(Point2D(0,0), Point2D(width - 2 * border, height - 2 * border)) {
    viewport = std::make_pair(Point2D(border, border),
                              Point2D(width - border, height - border));

    window.setDrawer(*this);
    wview = {Point3D(0, 0, 1),
             Point3D(0, height - 2 * border, 1),
             Point3D(width - 2 * border, height - 2 * border, 1),
             Point3D(width - 2 * border, 0, 1)};

    wview.setVisible(true);
    // wview.update(window.normalizerMatrix(), window);
    updateShape(wview, window.normalizerMatrix());
    wview.setColor(177, 0, 0);
}

void Drawer::addShape(Drawable3D* d) {
    SuperDrawer::addShape(d);
    update(*d);
}

void Drawer::draw(Point3D& p) {
    if (!p.isVisible()) return;
    cairo::set_color(p.getColor());
    auto pv = window.toViewport(viewport, getNDC(p));
    cairo::point(pv[0], pv[1]);
}

void Drawer::draw(Line3D& ln) {
    if (!ln.isVisible()) return;
    cairo::set_color(ln.getColor());
    auto p1 = window.toViewport(viewport, getNDC(ln[0]));
    auto p2 = window.toViewport(viewport, getNDC(ln[1]));
    cairo::move_to(p1[0], p1[1]);
    cairo::line_to(p2[0], p2[1]);
    cairo::stroke();
}

void Drawer::draw(Polygon3D& polygon) {
    if (!polygon.isVisible()) return;
    cairo::set_color(polygon.getColor());
    auto& points = getNDC(polygon);
    for (auto& point : points) {
        // auto newPoint = window.toViewport(viewport, getNDC(point));
        auto newPoint = window.toViewport(viewport, point);
        cairo::line_to(newPoint[0], newPoint[1]);
    }
    cairo::close_path();
    if (polygon.isFilled()) {
        cairo::fill();
    } else {
        cairo::stroke();
    }
}

void Drawer::draw(SimpleCurve3D& c) {
    if (!c.isVisible()) return;
    cairo::set_color(c.getColor());
    Line3D* lastLine = nullptr;
    for (auto& line : c) {
        Point2D newPoint;
        if (line.isVisible()) {
            newPoint = window.toViewport(viewport, getNDC(line[0]));
            lastLine = &line;
            cairo::line_to(newPoint[0], newPoint[1]);                
        } else if (lastLine != nullptr) {
            newPoint = window.toViewport(viewport, getNDC((*lastLine)[1]));
            lastLine = nullptr;
            cairo::line_to(newPoint[0], newPoint[1]);
        }
    }
    cairo::stroke();
}

void Drawer::draw(Curve3D& curve) {
    for (auto& c : curve) {
        c.setColor(curve.getColor());
        draw(c);
    }
}

void Drawer::draw(Wireframe3D& wireframe) {
    if (!wireframe.isVisible()) return;
    for (auto& edge : wireframe) {
        edge.setColor(wireframe.getColor());
        draw(edge);
    }
}

void Drawer::draw(BicubicSurface& surface) {
    for (auto& c : surface) {
        c.setColor(surface.getColor());
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

void Drawer::moveHorizontal(int direction) {
    window.moveHorizontal(direction * moveSpeed);
    updateAll();
}

void Drawer::moveVertical(int direction) {
    window.moveVertical(direction * moveSpeed);
    updateAll();
}

void Drawer::rotateWindow(long direction) {
    window.rotate(direction * rotationSpeed);
    updateAll();
}

void Drawer::translate(unsigned long index, const std::array<double, 3>& ds) {
    if (index < displayFile.size()) {
        SuperDrawer::translate(index, ds);
        update(*displayFile[index]);
        drawAll();
    }
}

void Drawer::scale(unsigned long index, const std::array<double, 3>& ss) {
    if (index < displayFile.size()) {
        SuperDrawer::scale(index, ss);
        update(*displayFile[index]);
        drawAll();
    }
}

void Drawer::rotate(unsigned long index, double angle,
                    int type, const std::vector<std::string>& entries) {
    if (index < displayFile.size()) {
        Point3D axis;
        auto& shape = displayFile[index];
        switch (type) {
            case 0:
                axis = shape->center();
                break;
            case 1:
                break;
            case 2:
                Point3D p1 = {stod(entries[0]), stod(entries[1]), stod(entries[2])};
                Point3D p2 = {stod(entries[3]), stod(entries[4]), stod(entries[5])};
                axis = p2 - p1;
                break;
        }
        SuperDrawer::rotate(index, angle, axis);
        update(*displayFile[index]);
        drawAll();
    }
}

void Drawer::resizeViewport(double length, double height) {
    //viewport.second[0] = length;
    //viewport.second[1] = height;
    draw(wview);
}

void Drawer::setZoom(double rate) {
    zoomRate = rate;
}

void Drawer::setViewport(Point2D v1, Point2D v2) {
    viewport = std::make_pair(v1, v2);
}

void Drawer::setClippingAlgorithm(int algorithm) {
    window.setClippingAlgorithm(algorithm);
}

void Drawer::swap(const std::vector<Drawable3D*>& newDisplayFile) {
    SuperDrawer::swap(newDisplayFile);
    updateAll();
}

void Drawer::update(Drawable3D& shape, const Matrix<3,3>& matrix) {
    updateShape(shape, matrix);
    shape.clip(window);
}

void Drawer::update(Drawable3D& shape) {
    update(shape, window.normalizerMatrix());
}

void Drawer::updateAll() {
    auto normalizer = window.normalizerMatrix();
    for (auto shape : displayFile) {
        // shape->update(normalizer, window);
        update(*shape, normalizer);
        shape->clip(window);
    }
}

void Drawer::zoom(int d) {
    window.zoom(d * zoomRate);
    updateAll();
}

void Drawer::updateShape(Drawable3D& shape, const Matrix<3,3>& matrix) {
    auto type = shape.getType();
    switch (type) {
        case DrawableType::Point:
            updateShape(static_cast<Point3D&>(shape), matrix);
            break;
        case DrawableType::Line:
            updateShape(static_cast<Line3D&>(shape), matrix);
            break;
        case DrawableType::Polygon:
            updateShape(static_cast<Polygon3D&>(shape), matrix);
            break;
        case DrawableType::Curve:
            updateShape(static_cast<Curve3D&>(shape), matrix);
            break;
        case DrawableType::Wireframe:
            updateShape(static_cast<Wireframe3D&>(shape), matrix);
            break;
        case DrawableType::BicubicSurface:
            updateShape(static_cast<BicubicSurface&>(shape), matrix);
            break;
        default:
            ECHO("dafuq is this?");
            throw 666;
    }
}

void Drawer::updateShape(Point3D& point, const Matrix<3,3>& matrix) {
    getNDC(point) = window.perspectiveProjection(point);
    getNDC(point) *= matrix;
}

void Drawer::updateShape(Line3D& line, const Matrix<3,3>& matrix) {
    updateShape(line[0], matrix);
    updateShape(line[1], matrix);
}

void Drawer::updateShape(Polygon3D& polygon, const Matrix<3,3>& matrix) {
    getNDC(polygon).clear();
    for (auto& vertex : polygon) {
        updateShape(vertex, matrix);
        auto ndc = getNDC(vertex);
        getNDC(polygon).push_back(Point<3>(ndc[0], ndc[1], 0));
        // getNDC(polygon).push_back(vertex);
    }
}

void Drawer::updateShape(SimpleCurve3D& curve, const Matrix<3,3>& matrix) {
    for (auto& line : curve) {
        updateShape(line, matrix);
    }
}

void Drawer::updateShape(Curve3D& curve, const Matrix<3,3>& matrix) {
    for (auto& c : curve) {
        updateShape(c, matrix);
    }
}

void Drawer::updateShape(Wireframe3D& wireframe, const Matrix<3,3>& matrix) {
    for (auto& edge : wireframe) {
        updateShape(edge, matrix);
    }
}

void Drawer::updateShape(BicubicSurface& surface, const Matrix<3,3>& matrix) {
    for (auto& c : surface) {
        updateShape(c, matrix);
    }
}
