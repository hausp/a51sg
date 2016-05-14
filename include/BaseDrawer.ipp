/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include <cmath>
#include <iostream>
#include "BaseDrawer.hpp"
#include "cairo.hpp"
#include "Drawable.hpp"
#include "gtk.hpp"
#include "Line.hpp"
#include "Matrix.hpp"
#include "Polygon.hpp"
#include "Point.hpp"
#include "Window.hpp"

template<unsigned D>
BaseDrawer<D>::BaseDrawer(const unsigned width, const unsigned height, const unsigned border)
: window(Point<2>(0,0), Point<2>(width - 2 * border, height - 2 * border)) {
    viewport = std::make_pair(Point<2>(border, border),
                              Point<2>(width - border, height - border));

    wview = {Point<2>(0, 0),
             Point<2>(0, height - 2 * border),
             Point<2>(width - 2 * border, height - 2 * border),
             Point<2>(width - 2 * border, 0)};

    wview.setVisible(true);
    wview.update(window.normalizerMatrix());
    wview.setColor(177, 0, 0);
}


template<unsigned D>
BaseDrawer<D>::~BaseDrawer<D>() {
    for (auto shape : displayFile) {
        delete shape;
    }
}

template<unsigned D>
void BaseDrawer<D>::addShape(Drawable<D>* d) {
    displayFile.push_back(d);
    update(d);
}

template<unsigned D>
void BaseDrawer<D>::swap(const std::vector<Drawable<D>*>& newDisplayFile) {
    for (auto shape : displayFile) {
        delete shape;
    }
    displayFile = newDisplayFile;
    updateAll();
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
double BaseDrawer<D>::getWindowAngle() {
    return window.getAngle();
}

template<unsigned D>
double BaseDrawer<D>::getZoomLevel() {
    return window.getZoomLevel();
}

template<unsigned D>
void BaseDrawer<D>::drawAll() {
    cairo::clear();
    for (auto shape : displayFile) {
        shape->draw(*this);
    }
    draw(wview);
}

template<unsigned D>
void BaseDrawer<D>::moveHorizontal(const int direction) {
    window.moveHorizontal(direction * moveSpeed);
    updateAll();
}

template<unsigned D>
void BaseDrawer<D>::moveVertical(const int direction) {
    window.moveVertical(direction * moveSpeed);
    updateAll();
}

template<unsigned D>
void BaseDrawer<D>::removeShape(const unsigned long index) {
    if (index < displayFile.size()) {
        displayFile.erase(displayFile.begin() + index);
    }
}

template<unsigned D>
void BaseDrawer<D>::translate(const unsigned long index, const std::array<double, D>& ds) {
    if (index < displayFile.size()) {
        displayFile[index]->transform(utils::translationMatrix(ds));
        update(displayFile[index]);
        drawAll();
    }
}

template<unsigned D>
void BaseDrawer<D>::scale(const unsigned long index, const std::array<double, D>& ss) {
    if (index < displayFile.size()) {
        auto& shape = displayFile[index];
        auto center = shape->center();
        auto m = utils::translationMatrix((center * -1).toArray());
        m *= utils::scalingMatrix(ss);
        m *= utils::translationMatrix(center.toArray());
        shape->transform(m);
        update(displayFile[index]);
        drawAll();
    }
}

template<unsigned D>
void BaseDrawer<D>::rotate(const unsigned long index, const double angle,
                       const int type, const std::vector<std::string>& entries) {
    if (index < displayFile.size()) {
        auto& shape = displayFile[index];
        Point<D> axis;
        switch (type) {
            case 0:
                axis = shape->center();
                break;
            case 1:
                axis = Point<D>(0, 0);
                break;
            case 2:
                axis = Point<D>(stod(entries[0]), stod(entries[1]));
                break;
        }

        auto m = utils::translationMatrix((axis * -1).toArray());
        m *= utils::rotationMatrix<2>(angle);
        m *= utils::translationMatrix(axis.toArray());
        shape->transform(m);
        update(displayFile[index]);
        drawAll();
    }
}

template<unsigned D>
void BaseDrawer<D>::rotateWindow(long direction) {
    window.rotate(direction * rotationSpeed);
    updateAll();
}

template<unsigned D>
void BaseDrawer<D>::update(Drawable<D>* shape) {
    shape->update(window.normalizerMatrix());
    shape->clip(window);
}

template<unsigned D>
void BaseDrawer<D>::updateAll() {
    auto normalizer = window.normalizerMatrix();
    for (auto shape : displayFile) {
        shape->update(normalizer);
        shape->clip(window);
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

template<unsigned D>
void BaseDrawer<D>::setZoom(const double rate) {
    zoomRate = rate;
}

template<unsigned D>
void BaseDrawer<D>::setViewport(Point<2> v1, Point<2> v2) {
    viewport = std::make_pair(v1, v2);
}

template<unsigned D>
void BaseDrawer<D>::zoom(const int d) {
    window.zoom(d * zoomRate);
    updateAll();
}

template<unsigned D>
void BaseDrawer<D>::resizeViewport(const double length, const double height) {
    //viewport.second[0] = length;
    //viewport.second[1] = height;
    draw(wview);
}

template<unsigned D>
void BaseDrawer<D>::setClippingAlgorithm(const int algorithm) {
    window.setClippingAlgorithm(algorithm);
}