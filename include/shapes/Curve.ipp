/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "Point.hpp"
#include "BaseDrawer.hpp"
#include "Matrix.hpp"
#include "Window.hpp"

template<unsigned D>
void Curve<D>::draw(BaseDrawer<D>& BaseDrawer) {
    BaseDrawer.draw(*this);
}

template<unsigned D>
void Curve<D>::clip(Window& window) {
    window.clip(*this);
}

template<unsigned D>
void Curve<D>::transform(const Matrix<D+1,D+1>& matrix) {
    for (auto& c : curves) {
        c.transform(matrix);
    }
}

template<unsigned D>
Point<D> Curve<D>::center() const {
    return Point<D>();
}

template<unsigned D>
std::vector<Point<D>> Curve<D>::points() const {
    std::vector<Point<D>> list;
    for (auto& c : curves) {
        auto cList = c.points();
        for (auto p : cList) {
            list.push_back(p);
        }
    }
    return list;
}

// template<unsigned D>
// void Curve<D>::update(const Matrix<3,3>& matrix, const Window& window) {
//     for (auto& c : curves) {
//         c.update(matrix, window);
//     }
// }

template<unsigned D>
typename std::vector<SimpleCurve<D>>::iterator Curve<D>::begin() {
    return curves.begin();
}

template<unsigned D>
typename std::vector<SimpleCurve<D>>::const_iterator Curve<D>::begin() const {
    return curves.cbegin();
}

template<unsigned D>
typename std::vector<SimpleCurve<D>>::iterator Curve<D>::end() {
    return curves.end();
}

template<unsigned D>
typename std::vector<SimpleCurve<D>>::const_iterator Curve<D>::end() const {
    return curves.cend();
}
