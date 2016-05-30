/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "BaseDrawer.hpp"
#include "Window.hpp"
#include "BaseVector.hpp"
#include "BaseTransformation.hpp"

template<unsigned D>
Line<D>::Line() : Drawable(DrawableType::Line) { }

template<unsigned D>
Line<D>::Line(const std::string& name, const Point<D>& p1, const Point<D>& p2)
: Drawable(name, DrawableType::Line) {
    pointList.push_back(p1);
    pointList.push_back(p2);
}

template<unsigned D>
Line<D>::Line(const Point<D>& p1, const Point<D>& p2) 
: Drawable(DrawableType::Line) {
    pointList.push_back(p1);
    pointList.push_back(p2);
}

template<unsigned D>
template<unsigned Dn>
Line<D>::Line(const Line<Dn>& line) : Drawable(DrawableType::Line) {
    for (auto& p : line) {
        pointList.push_back(Point<D>(p));
    }
}

template<unsigned D>
void Line<D>::draw(BaseDrawer& drawer) {
    drawer.draw(*this);
}

template<unsigned D>
void Line<D>::clip(Window& window) {
    window.clip(*this);
}

template<unsigned D>
void Line<D>::transform(const BaseTransformation& matrix) {
    ECHO("TODO");
    //(*this)[0] *= matrix;
    //(*this)[1] *= matrix;
}

template<unsigned D>
BaseVector Line<D>::center() const {
    return (pointList[0] + pointList[1]) / 2;
}

template<unsigned D>
std::vector<BaseVector> Line<D>::points() const {
    ECHO("TODO");
    //return pointList;
}

template<unsigned D>
void Line<D>::update(const BaseTransformation& matrix, const Window& window) {
    ECHO("TODO");
    // (*this)[0].update(matrix, window);
    // (*this)[1].update(matrix, window);
}

template<unsigned D>
Point<D>& Line<D>::operator[](size_t index) {
    return pointList[index];
}

template<unsigned D>
const Point<D>& Line<D>::operator[](size_t index) const {
    return pointList[index];
}

template<unsigned D>
bool Line<D>::operator==(const Line<D>& ln) const {
    return ((*this)[0] == ln[0] && (*this)[1] == ln[1])
        || ((*this)[0] == ln[1] && (*this)[1] == ln[0]);
}

template<unsigned D>
bool Line<D>::operator!=(const Line<D>& ln) const {
    return !(*this == ln);
}

template<unsigned D>
typename std::vector<Point<D>>::iterator Line<D>::begin() { 
    return pointList.begin();
}

template<unsigned D>
typename std::vector<Point<D>>::const_iterator Line<D>::begin() const {
    return pointList.cbegin();
}

template<unsigned D>
typename std::vector<Point<D>>::iterator Line<D>::end() {
    return pointList.end();
}

template<unsigned D>
typename std::vector<Point<D>>::const_iterator Line<D>::end() const {
    return pointList.cend();
}

template<unsigned D>
std::ostream& operator<<(std::ostream& stream, const Line<D>& line) {
    stream << '(' << line[0] << ',' << line[1] << ')';
    return stream;
}
