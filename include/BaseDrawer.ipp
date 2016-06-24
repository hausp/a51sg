/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include <cmath>
#include "BaseDrawer.hpp"
#include "Curve.hpp"
#include "Drawable.hpp"
#include "Matrix.hpp"
#include "Point.hpp"
#include "Polygon.hpp"
#include "SimpleCurve.hpp"

template<unsigned D>
BaseDrawer<D>::BaseDrawer() { }

template<unsigned D>
BaseDrawer<D>::~BaseDrawer<D>() {
    for (auto shape : displayFile) {
        delete shape;
    }
}

template<unsigned D>
void BaseDrawer<D>::addShape(Drawable<D>* d) {
    displayFile.push_back(d);
}

template<unsigned D>
void BaseDrawer<D>::swap(const std::vector<Drawable<D>*>& newDisplayFile) {
    for (auto shape : displayFile) {
        delete shape;
    }
    displayFile = newDisplayFile;
}

template<unsigned D>
const std::vector<Drawable<D>*>& BaseDrawer<D>::getDisplayFile() {
    return displayFile;
}

template<unsigned D>
void BaseDrawer<D>::clearDisplayFile() {
    displayFile.clear();
}

template<unsigned D>
void BaseDrawer<D>::drawAll() {
    for (auto shape : displayFile) {
        shape->draw(*this);
    }
}

template<unsigned D>
void BaseDrawer<D>::removeShape(size_t index) {
    if (index < displayFile.size()) {
        displayFile.erase(displayFile.begin() + index);
    }
}

template<unsigned D>
void BaseDrawer<D>::translate(size_t index, const std::array<double, D>& ds) {
    if (index < displayFile.size()) {
        displayFile[index]->transform(utils::translationMatrix(ds));
    }
}

template<unsigned D>
void BaseDrawer<D>::scale(size_t index, const std::array<double, D>& ss) {
    if (index < displayFile.size()) {
        auto& shape = displayFile[index];
        auto center = shape->center();
        auto m = utils::translationMatrix((center * -1).toArray());
        m *= utils::scalingMatrix(ss);
        m *= utils::translationMatrix(center.toArray());
        shape->transform(m);
    }
}

template<unsigned D>
void BaseDrawer<D>::rotate(size_t index, double angle, const Point<D>& axis) {
    if (index < displayFile.size()) {
        auto& shape = displayFile[index];
        // auto m = utils::translationMatrix((axis * -1).toArray());
        // m *= utils::rotationMatrix<D>(angle);
        // m *= utils::translationMatrix(axis.toArray());
        Point<3> xAxis(1, 0, 0);
        Point<3> yAxis(0, 1, 0);
        double tx = acos((xAxis * axis) / axis.norm()) * 180 / M_PI;
        double ty = acos((yAxis * axis) / axis.norm()) * 180 / M_PI;
        auto transformation = utils::rotationMatrix<3>(tx, utils::RotationPlane::X);
        transformation *= utils::rotationMatrix<3>(ty, utils::RotationPlane::Y);

        transformation *= utils::rotationMatrix<3>(angle, utils::RotationPlane::Z);
        transformation *= utils::rotationMatrix<3>(-ty, utils::RotationPlane::Y);        
        transformation *= utils::rotationMatrix<3>(-tx, utils::RotationPlane::X);
        shape->transform(transformation);
    }
}

template<unsigned D>
void BaseDrawer<D>::highlightObject(long index) {
    if (highlighted != -1) {
        displayFile[highlighted]->setColor(0, 0, 0);
    }
    displayFile[index]->setColor(48, 160, 255);
    highlighted = index;
}

template<unsigned D>
Point<2>& BaseDrawer<D>::getNDC(Point<2>& shape) {
    if (ndc2D.count(&shape) == 0) {
        ndc2D[&shape] = new Point<2>(shape);
    }
    return *ndc2D[&shape];
}

template<unsigned D>
Point<2>& BaseDrawer<D>::getNDC(Point<3>& shape) {
    if (ndc3D.count(&shape) == 0) {
        ndc3D[&shape] = new Point<2>(shape);
    }
    return *ndc3D[&shape];
}

template<unsigned D>
std::vector<Point<2>>& BaseDrawer<D>::getNDC(Polygon<2>& polygon) {
    if (ndc2DPolygons.count(&polygon) == 0) {
        std::cout << "[2D] &polygon = " << &polygon << std::endl;
        // TRACE(&polygon);
        ndc2DPolygons[&polygon] = polygon.points();
    }
    return ndc2DPolygons[&polygon];
}

template<unsigned D>
std::vector<Point<3>>& BaseDrawer<D>::getNDC(Polygon<3>& polygon) {
    if (ndc3DPolygons.count(&polygon) == 0) {
        std::cout << "[3D] &polygon = " << &polygon << std::endl;
        // TRACE(&polygon);
        ndc3DPolygons[&polygon] = polygon.points();
    }
    return ndc3DPolygons[&polygon];
}
