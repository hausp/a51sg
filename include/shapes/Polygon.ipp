/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "Drawer.hpp"
#include "Window.hpp"

template<unsigned D>
Polygon<D>::Polygon() : Drawable<D>("", DrawableType::Polygon) { }

template<unsigned D>
Polygon<D>::Polygon(const Polygon& polygon) : Drawable<D>("", DrawableType::Polygon) {
    for (auto l : polygon) {
        vertices.push_back(l);
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
    for (unsigned i = 0; i < points.size(); i++) {
        vertices.push_back(points[i]);
    }
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
    for (auto& vertex : vertices) {
        vertex.transform(matrix);
    }
}

template<unsigned D>
Point<D> Polygon<D>::center() const {
    Point<D> r;
    for (auto& vertex : vertices) {
        r += vertex;
    }
    return r / numberOfPoints();
}

template<unsigned D>
std::vector<Point<D>> Polygon<D>::points() const {
    std::vector<Point<D>> list;
    for (auto& vertex : vertices) {
        list.push_back(vertex);
    }
    return list;
}

template<unsigned D>
void Polygon<D>::update(const Matrix<D+1,D+1>& matrix) {
    ndcVertices.clear();
    for (auto& vertex : vertices) {
        vertex.update(matrix);
        ndcVertices.push_back(vertex.ndc());
    }
}

template<unsigned D>
const size_t Polygon<D>::numberOfPoints() const {
    return vertices.size();
}

template<unsigned D>
Point<D>& Polygon<D>::operator[](size_t index) {
    return vertices[index];
}

template<unsigned D>
const Point<D>& Polygon<D>::operator[](size_t index) const {
    return vertices[index];
}

template<unsigned D>
typename std::vector<Point<D>>::iterator Polygon<D>::begin() {
    return vertices.begin();
}

template<unsigned D>
typename std::vector<Point<D>>::const_iterator Polygon<D>::begin() const {
    return vertices.cbegin();
}

template<unsigned D>
typename std::vector<Point<D>>::iterator Polygon<D>::end() {
    return vertices.end();
}

template<unsigned D>
typename std::vector<Point<D>>::const_iterator Polygon<D>::end() const {
    return vertices.cend();
}

template<unsigned D>
template<typename ...Args>
void Polygon<D>::init(const Point<D>& p0, const Point<D>& p1, const Args&... args) {
    vertices.push_back(p0);
    init(p1, args...);
}

template<unsigned D>
void Polygon<D>::init(const Point<D>& p) {
    vertices.push_back(p);
}

template<unsigned D>
std::vector<Point<D>>& Polygon<D>::ndc() {
    return ndcVertices;
}
