/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include <cmath>
#include <iostream>
#include "BaseDrawer.hpp"
#include "Drawable.hpp"
#include "Matrix.hpp"
#include "Point.hpp"

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
void BaseDrawer<D>::removeShape(const size_t index) {
    if (index < displayFile.size()) {
        displayFile.erase(displayFile.begin() + index);
    }
}

template<unsigned D>
void BaseDrawer<D>::translate(const size_t index, const std::array<double, D>& ds) {
    if (index < displayFile.size()) {
        displayFile[index]->transform(utils::translationMatrix(ds));
    }
}

template<unsigned D>
void BaseDrawer<D>::scale(const size_t index, const std::array<double, D>& ss) {
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
void BaseDrawer<D>::rotate(const size_t index, const double angle, const Point<D>& axis) {
    if (index < displayFile.size()) {
        auto& shape = displayFile[index];
        auto m = utils::translationMatrix((axis * -1).toArray());
        m *= utils::rotationMatrix<2>(angle);
        m *= utils::translationMatrix(axis.toArray());
        shape->transform(m);
    }
}

template<unsigned D>
void BaseDrawer<D>::highlightObject(const long index) {
    if (highlighted != -1) {
        displayFile[highlighted]->setColor(0, 0, 0);
    }
    displayFile[index]->setColor(48, 160, 255);
    highlighted = index;
}