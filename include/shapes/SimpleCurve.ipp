/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "Point.hpp"
#include "BaseDrawer.hpp"
#include "Line.hpp"
#include "Matrix.hpp"
#include "Window.hpp"

template<unsigned D>
void SimpleCurve<D>::draw(BaseDrawer<D>& BaseDrawer) {
    BaseDrawer.draw(*this);
}

template<unsigned D>
void SimpleCurve<D>::clip(Window& window) {
    // window.clip(*this);
}

template<unsigned D>
void SimpleCurve<D>::transform(const Matrix<D+1,D+1>& matrix) {
    for (auto& line : lines) {
        line.transform(matrix);
    }
}

template<unsigned D>
Point<D> SimpleCurve<D>::center() const {
    return Point<D>();
}

template<unsigned D>
std::vector<Point<D>> SimpleCurve<D>::points() const {
    std::vector<Point<D>> points;
    points.reserve(lines.size());
    for (auto line : lines) {
        points.push_back(line[0]);
    }
    return points;
}

template<unsigned D>
void SimpleCurve<D>::update(const Matrix<3,3>& matrix, const Window& window) {
    for (auto& line : lines) {
        line.update(matrix, window);
    }
}

template<unsigned D>
typename std::vector<Line<D>>::iterator SimpleCurve<D>::begin() {
    return lines.begin();
}

template<unsigned D>
typename std::vector<Line<D>>::const_iterator SimpleCurve<D>::begin() const {
    return lines.cbegin();
}

template<unsigned D>
typename std::vector<Line<D>>::iterator SimpleCurve<D>::end() {
    return lines.end();
}

template<unsigned D>
typename std::vector<Line<D>>::const_iterator SimpleCurve<D>::end() const {
    return lines.cend();
}
