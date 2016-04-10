/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <cmath>
#include "Point.hpp"

using Viewport = std::pair<Point<2>, Point<2>>;

class Window {

 public:
    Window(const Point<2>& min, const Point<2>& max)
    : min(min), max(max), angle(0), currentZoom(1) {
        defaultWidth  = max[0] - min[0];
        defaultHeight = max[1] - min[1];
    }

    Matrix<3, 3> normalizerMatrix() {
        Point<2> center = (min + max)/-2;
        Matrix<3, 3> normalizer = utils::translationMatrix(center.toArray());
        normalizer *= utils::rotationMatrix(-angle);
        std::array<double, 2> s = {2/(max[0] - min[0]), 2/(max[1] - min[1])};
        normalizer *= utils::scalingMatrix(s);
        return normalizer;
    }

    void moveHorizontal(const double displacement) {
        min[0] += displacement;
        max[0] += displacement;
    }

    void moveVertical(const double displacement) {
        min[1] += displacement;
        max[1] += displacement;
    }

    void rotate(const double _angle) {
        angle = fmod((angle + _angle), 360);
    }

    Point<2> toViewport(const Viewport& viewport, Point<2>& p) {
        double width  = viewport.second[0] - viewport.first[0];
        double height = viewport.second[1] - viewport.first[1];
        Point<2> pn   = p.ndc();
        double x = (pn[0] + 1) / 2 * width + viewport.first[0];
        double y = (2 - pn[1]) / 2 * height + viewport.first[1];
        return Point<2>(x, y);
    }

    void zoom(const double zoomRate) {
        if (currentZoom + zoomRate > 0) {
            currentZoom += zoomRate;
            double factor = 1 / (2 * currentZoom);
            Point<2> delta(defaultWidth * factor, defaultHeight * factor);
            Point<2> center = (min + max) / 2;
            min = center - delta;
            max = center + delta;
        }
    }

 private:
    Point<2> min;
    Point<2> max;
    double angle;
    double currentZoom;
    double defaultWidth;
    double defaultHeight;
};

#endif /* WINDOW_HPP */
