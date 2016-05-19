/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "BaseDrawer.hpp"
#include "Window.hpp"

template<unsigned D>
Wireframe<D>::Wireframe() : Drawable<D>("", DrawableType::Wireframe) { }

template<unsigned D>
template<typename ...Args>
Wireframe<D>::Wireframe(const Point<D>& p1, const Point<D>& p2, const Args&... args)
: Drawable<D>("", DrawableType::Wireframe) {
    init(p1, p2, args...);
}

template<unsigned D>
template<typename ...Args>
Wireframe<D>::Wireframe(const std::string& name, const Point<D>& p1, 
        const Point<D>& p2, const Args&... args)
: Drawable<D>(name, DrawableType::Wireframe) {
    init(p1, p2, args...);
}

template<unsigned D>
Wireframe<D>::Wireframe(const std::vector<Line<D>>& lines)
: Drawable<D>("", DrawableType::Wireframe) {
    for (unsigned i = 0; i < lines.size(); i++) {
        edgeList.push_back(lines[i]);
    }
}

template<unsigned D>
template<unsigned Dn>
Wireframe<D>::Wireframe(const Wireframe<Dn>& wireframe) : Drawable<D>("", DrawableType::Wireframe) {
    for (auto& ln : wireframe) {
        edgeList.push_back(Line<D>(ln));
    }
}

template<unsigned D>
void Wireframe<D>::draw(BaseDrawer<D>& drawer) {
    drawer.draw(*this);
}

template<unsigned D>
void Wireframe<D>::clip(Window& window) {
    window.clip(*this);
}

template<unsigned D>
void Wireframe<D>::transform(const Matrix<D+1,D+1>& matrix) {
    for (auto& edge : edgeList) {
        edge.transform(matrix);
    }
}

template<unsigned D>
Point<D> Wireframe<D>::center() const {
    Point<D> r;
    for (auto& edge : edgeList) {
        r += edge[0];
    }
    return r / numberOfPoints();
}

template<unsigned D>
std::vector<Point<D>> Wireframe<D>::points() const {
    std::vector<Point<D>> list;
    for (auto& edge : edgeList) {
        list.push_back(edge[0]);
    }
    return list;
}

template<unsigned D>
void Wireframe<D>::update(const Matrix<3,3>& matrix, const Window& window) {
    for (auto& edge : edgeList) {
        edge.update(matrix, window);
    }
}

template<unsigned D>
std::vector<Line<D>>& Wireframe<D>::edges() {
    return edgeList;
}

template<unsigned D>
size_t Wireframe<D>::numberOfPoints() const {
    return edgeList.size();
}

template<unsigned D>
Line<D>& Wireframe<D>::operator[](size_t index) {
    return edgeList[index];
}

template<unsigned D>
const Line<D>& Wireframe<D>::operator[](size_t index) const {
    return edgeList[index];
}

template<unsigned D>
typename std::vector<Line<D>>::iterator Wireframe<D>::begin() {
    return edgeList.begin();
}

template<unsigned D>
typename std::vector<Line<D>>::const_iterator Wireframe<D>::begin() const {
    return edgeList.cbegin();
}

template<unsigned D>
typename std::vector<Line<D>>::iterator Wireframe<D>::end() {
    return edgeList.end();
}

template<unsigned D>
typename std::vector<Line<D>>::const_iterator Wireframe<D>::end() const {
    return edgeList.cend();
}

template<unsigned D>
template<typename ...Args>
void Wireframe<D>::init(const Point<D>& p0, const Point<D>& p1, const Args&... args) {
    edgeList.push_back(Line<D>(p0, p1));
    init(p1, args...);
}
