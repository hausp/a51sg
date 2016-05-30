/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include <cmath>
#include "BaseDrawer.hpp"
#include "Curve.hpp"
#include "Drawable.hpp"
#include "Matrix.hpp"
#include "Point.hpp"
#include "SimpleCurve.hpp"

BaseDrawer::BaseDrawer() { }

BaseDrawer::~BaseDrawer() {
    for (auto shape : displayFile) {
        delete shape;
    }
}

void BaseDrawer::addShape(Drawable* d) {
    displayFile.push_back(d);
}

void BaseDrawer::swap(const std::vector<Drawable*>& newDisplayFile) {
    for (auto shape : displayFile) {
        delete shape;
    }
    displayFile = newDisplayFile;
}

const std::vector<Drawable*>& BaseDrawer::getDisplayFile() {
    return displayFile;
}

void BaseDrawer::clearDisplayFile() {
    displayFile.clear();
}

void BaseDrawer::drawAll() {
    for (auto shape : displayFile) {
        shape->draw(*this);
    }
}

void BaseDrawer::removeShape(size_t index) {
    if (index < displayFile.size()) {
        displayFile.erase(displayFile.begin() + index);
    }
}

void BaseDrawer::rotate(size_t index, double angle, const BaseVector& axis) {
    if (index < displayFile.size()) {
        auto& shape = displayFile[index];
        // auto m = utils::translationMatrix((axis * -1).toArray());
        // m *= utils::rotationMatrix<D>(angle);
        // m *= utils::translationMatrix(axis.toArray());
        Point<3> xAxis(1, 0, 0);
        Point<3> yAxis(0, 1, 0);
        //double tx = acos((xAxis * axis) / axis.norm()) * 180 / M_PI;
        //double ty = acos((yAxis * axis) / axis.norm()) * 180 / M_PI;
        // auto transformation = utils::rotationMatrix<3>(tx, utils::RotationPlane::X);
        // transformation *= utils::rotationMatrix<3>(ty, utils::RotationPlane::Y);

        // transformation *= utils::rotationMatrix<3>(angle, utils::RotationPlane::Z);
        // transformation *= utils::rotationMatrix<3>(-ty, utils::RotationPlane::Y);        
        // transformation *= utils::rotationMatrix<3>(-tx, utils::RotationPlane::X);
        // shape->transform(transformation);
    }
}

void BaseDrawer::highlightObject(long index) {
    if (highlighted != -1) {
        displayFile[highlighted]->setColor(0, 0, 0);
    }
    displayFile[index]->setColor(48, 160, 255);
    highlighted = index;
}