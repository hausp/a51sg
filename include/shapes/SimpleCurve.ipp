/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "Point.hpp"
#include "BaseDrawer.hpp"
#include "Line.hpp"
#include "Matrix.hpp"
#include "Window.hpp"
#include "BaseVector.hpp"
#include "BaseMatrix.hpp"

template<unsigned D>
void SimpleCurve<D>::draw(BaseDrawer& BaseDrawer) {
    BaseDrawer.draw(*this);
}

template<unsigned D>
void SimpleCurve<D>::clip(Window& window) {
    window.clip(*this);
}

template<unsigned D>
void SimpleCurve<D>::transform(const BaseMatrix& matrix) {
    for (auto& line : lineList) {
        line.transform(matrix);
    }
}

template<unsigned D>
BaseVector SimpleCurve<D>::center() const {
    return Point<D>();
}

template<unsigned D>
std::vector<Point<D>> SimpleCurve<D>::points() const {
    std::vector<Point<D>> points;
    points.reserve(lineList.size());
    for (auto line : lineList) {
        points.push_back(line[0]);
    }
    return points;
}

template<unsigned D>
std::vector<Line<D>>& SimpleCurve<D>::lines() {
    return lineList;
}

template<unsigned D>
void SimpleCurve<D>::update(const BaseMatrix& matrix, const Window& window) {
    for (auto& line : lineList) {
        line.update(matrix, window);
    }
}

template<unsigned D>
typename std::vector<Line<D>>::iterator SimpleCurve<D>::begin() {
    return lineList.begin();
}

template<unsigned D>
typename std::vector<Line<D>>::const_iterator SimpleCurve<D>::begin() const {
    return lineList.cbegin();
}

template<unsigned D>
typename std::vector<Line<D>>::iterator SimpleCurve<D>::end() {
    return lineList.end();
}

template<unsigned D>
typename std::vector<Line<D>>::const_iterator SimpleCurve<D>::end() const {
    return lineList.cend();
}
