/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "BaseDrawer.hpp"
#include "BicubicSurface.hpp"
#include "Point.hpp"
#include "Matrix.hpp"
#include "Window.hpp"

void BicubicSurface::draw(BaseDrawer<3>& BaseDrawer) {
    BaseDrawer.draw(*this);
}

void BicubicSurface::clip(Window& window) {
    window.clip(*this);
}

void BicubicSurface::transform(const Matrix<4,4>& matrix) {
    for (auto& c : curves) {
        c.transform(matrix);
    }
}

Point<3> BicubicSurface::center() const {
    return Point<3>();
}

std::vector<Point<3>> BicubicSurface::points() const {
    std::vector<Point<3>> list;
    for (auto& c : curves) {
        auto cList = c.points();
        for (auto p : cList) {
            list.push_back(p);
        }
    }
    return list;
}

typename std::vector<SimpleCurve<3>>::iterator BicubicSurface::begin() {
    return curves.begin();
}

typename std::vector<SimpleCurve<3>>::const_iterator BicubicSurface::begin() const {
    return curves.cbegin();
}

typename std::vector<SimpleCurve<3>>::iterator BicubicSurface::end() {
    return curves.end();
}

typename std::vector<SimpleCurve<3>>::const_iterator BicubicSurface::end() const {
    return curves.cend();
}
