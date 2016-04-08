/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include <cmath>
#include <iostream>
#include "gtk.hpp"
#include "cairo.hpp"
#include "Drawable.hpp"
#include "Line.hpp"
#include "Point.hpp"
#include "Polygon.hpp"

template<unsigned D>
Drawer<D>::Drawer(const int width, const int height) 
: defaultWidth(width), defaultHeight(height) {
    window = std::make_pair(new Point<D>(0,0),
                new Point<D>(defaultWidth, defaultHeight));
    viewport = std::make_pair(new Point<D>(0,0),
                new Point<D>(defaultWidth, defaultHeight));
}

template<unsigned D>
Drawer<D>::~Drawer<D>() {
    for (auto shape : displayFile) {
        delete shape;
    }
    delete window.first;
    delete window.second;
    delete viewport.first;
    delete viewport.second;
}

template<unsigned D>
void Drawer<D>::addShape(Drawable<D>* d) {
    displayFile.push_back(d);
}

template<unsigned D>
void Drawer<D>::swap(const std::vector<Drawable<D>*>& newDisplayFile) {
    displayFile = newDisplayFile;
}

template<unsigned D>
const std::vector<Drawable<D>*>& Drawer<D>::getDisplayFile() {
    return displayFile;
}

template<unsigned D>
void Drawer<D>::drawAll() {
    cairo::clear();
    for (auto shape : displayFile) {
        shape->draw(*this);
    }
}

template<unsigned D>
void Drawer<D>::moveVertical(const int direction) {
    (*window.first)[1] += direction * moveSpeed;
    (*window.second)[1] += direction * moveSpeed;
}

template<unsigned D>
void Drawer<D>::moveHorizontal(const int direction) {
    (*window.first)[0] += direction * moveSpeed;
    (*window.second)[0] += direction * moveSpeed;
}

template<unsigned D>
void Drawer<D>::removeShape(const unsigned long index) {
    if (index < displayFile.size()) {
        displayFile.erase(displayFile.begin() + index);
    }
}

template<unsigned D>
void Drawer<D>::translate(const unsigned long index, const double dx, const double dy) {
    if (index < displayFile.size()) {
        displayFile[index]->transform(translationMatrix(dx, dy));
        drawAll();
    }
}

template<unsigned D>
void Drawer<D>::scale(const unsigned long index, const double sx, const double sy) {
    if (index < displayFile.size()) {
        auto& shape = displayFile[index];
        Point<D> center = shape->center();
        auto m = translationMatrix(-center[0], -center[1]);
        m = m * scalingMatrix(sx, sy);
        m = m * translationMatrix(center[0], center[1]);
        shape->transform(m);
        drawAll();
    }
}

template<unsigned D>
void Drawer<D>::rotate(const unsigned long index, const double angle, const int type, const std::vector<std::string>& entries) {
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

        auto m = translationMatrix(-axis[0], -axis[1]);
        m = m * rotationMatrix(angle);
        m = m * translationMatrix(axis[0], axis[1]);
        shape->transform(m);
        drawAll();
    }
}

template<unsigned D>
Matrix<D+1,D+1> Drawer<D>::translationMatrix(const double dx, const double dy) {
    return {{1,0,0},{0,1,0},{dx,dy,1}};
}

template<unsigned D>
Matrix<D+1,D+1> Drawer<D>::scalingMatrix(const double sx, const double sy) {
    return {{sx,0,0},{0,sy,0},{0,0,1}};
}

template<unsigned D>
Matrix<D+1,D+1> Drawer<D>::rotationMatrix(const double angle) {
    double a = angle * M_PI / 180;
    double s = sin(a);
    double c = cos(a);
    return {{c,s,0},{-s,c,0},{0,0,1}};
}

template<unsigned D>
void Drawer<D>::highlightObject(const long index) {
    if (highlighted != -1) {
        displayFile[highlighted]->setColor(0, 0, 0);
    }
    displayFile[index]->setColor(48, 160, 255);
    highlighted = index;
}

template<unsigned D>
void Drawer<D>::setZoom(const double rate) {
    zoomRate = rate;
}

template<unsigned D>
void Drawer<D>::setViewport(Point<2>* v1, Point<2>* v2) {
    viewport = std::make_pair(v1, v2);
}

template<unsigned D>
void Drawer<D>::zoom(const int d) {
    if (currentZoom + d * zoomRate > 0) {
        currentZoom += d * zoomRate;
        double factor = 1 / (2 * currentZoom);
        Point<2> delta(defaultWidth * factor, defaultHeight * factor);
        Point<2> center = (*window.first + *window.second) / 2;
        *window.first = center - delta;
        *window.second = center + delta;
    }
}

template<unsigned D>
void Drawer<D>::resizeViewport(const double length, const double height) {
    (*viewport.second)[0] = length;
    (*viewport.second)[1] = height;
}
