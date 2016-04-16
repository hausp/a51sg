/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "Drawer.hpp"
#include "Window.hpp"

template<unsigned D>
Polygon<D>::Polygon() : Drawable<D>("", DrawableType::Polygon) { }

template<unsigned D>
Polygon<D>::Polygon(const Polygon& polygon) : Drawable<D>("", DrawableType::Polygon) {
    for (auto l : polygon) {
        lines.push_back(l);
    }
}

template<unsigned D>
template<typename ...Args>
Polygon<D>::Polygon(const Point<D>& p1, const Point<D>& p2, const Args&... args)
: Drawable<D>("", DrawableType::Polygon) {
    init(p1, p2, args...);
}

template<unsigned D>
template<typename ...Args>
Polygon<D>::Polygon(const std::string& name, const Point<D>& p1, 
        const Point<D>& p2, const Args&... args)
: Drawable<D>(name, DrawableType::Polygon) {
    init(p1, p2, args...);
}

template<unsigned D>
Polygon<D>::Polygon(const std::vector<Point<D>>& points)
: Drawable<D>("", DrawableType::Polygon) {
    for (unsigned i = 0; i < points.size()-1; i++) {
        lines.push_back(Line<D>(points[i], points[i+1]));
    }
    lines.push_back(Line<D>(points.back(), points.front()));
}

template<unsigned D>
void Polygon<D>::draw(Drawer<D>& drawer) {
    drawer.draw(*this);
}

template<unsigned D>
void Polygon<D>::clip(Window& window) {
    window.clip(*this);
}

template<unsigned D>
void Polygon<D>::transform(const Matrix<D+1,D+1>& matrix) {
    for (auto& line : lines) {
        line.transform(matrix);
    }
}

template<unsigned D>
Point<D> Polygon<D>::center() const {
    Point<D> r;
    for (auto& line : lines) {
        r += line[0];
    }
    return r / numberOfPoints();
}

template<unsigned D>
std::vector<Point<D>> Polygon<D>::points() const {
    std::vector<Point<D>> list;
    for (auto& line : lines) {
        list.push_back(line[0]);
    }
    return list;
}

template<unsigned D>
void Polygon<D>::update(const Matrix<D+1,D+1>& matrix) {
    for (auto& line : lines) {
        line.update(matrix);
    }
}

template<unsigned D>
const size_t Polygon<D>::numberOfPoints() const {
    return lines.size();
}

template<unsigned D>
Point<D>& Polygon<D>::operator[](size_t index) {
    return lines[index][0];
}

template<unsigned D>
const Point<D>& Polygon<D>::operator[](size_t index) const {
    return lines[index][0];
}

template<unsigned D>
typename std::vector<Line<D>>::iterator Polygon<D>::begin() {
    return lines.begin();
}

template<unsigned D>
typename std::vector<Line<D>>::const_iterator Polygon<D>::begin() const {
    return lines.cbegin();
}

template<unsigned D>
typename std::vector<Line<D>>::iterator Polygon<D>::end() {
    return lines.end();
}

template<unsigned D>
typename std::vector<Line<D>>::const_iterator Polygon<D>::end() const {
    return lines.cend();
}

template<unsigned D>
template<typename ...Args>
void Polygon<D>::init(const Point<D>& p0, const Point<D>& p1, const Args&... args) {
    lines.push_back(Line<D>(p0, p1));
    init(p1, args...);
}

template<unsigned D>
void Polygon<D>::init(const Point<D>& p) {
    lines.push_back(Line<D>(p, lines[0][0]));
}