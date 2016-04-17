/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
and Marleson Graf<aszdrick@gmail.com> [2016] */

#include <cmath>
#include "Line.hpp"
#include "Polygon.hpp"
#include "Window.hpp"

Window::Window(const Point<2>& min, const Point<2>& max)
: min(min), max(max), angle(0), currentZoom(1) {
    defaultWidth  = max[0] - min[0];
    defaultHeight = max[1] - min[1];
}

Matrix<3, 3> Window::normalizerMatrix() {
    Point<2> center = (min + max)/-2;
    Matrix<3, 3> normalizer = utils::translationMatrix(center.toArray());
    normalizer *= utils::rotationMatrix(-angle);
    std::array<double, 2> s = {2/(max[0] - min[0]), 2/(max[1] - min[1])};
    normalizer *= utils::scalingMatrix(s);
    return normalizer;
}

void Window::moveHorizontal(const double displacement) {
    auto m = utils::rotationMatrix(-angle);
    m *= utils::translationMatrix(std::array<double, 2>{displacement, 0});
    m *= utils::rotationMatrix(angle);
    min *= m;
    max *= m;
    // double a = angle * M_PI / 180;
    // double s = sin(a);
    // double c = cos(a);
    // double dx = displacement;
    // double dy = 0;
    // min[0] += dx * c + dy * s;
    // min[1] += dy * c - dx * s;
    // max[0] += dx * c + dy * s;
    // max[1] += dy * c - dx * s;
}

void Window::moveVertical(const double displacement) {
    auto m = utils::rotationMatrix(-angle);
    m *= utils::translationMatrix(std::array<double, 2>{0, displacement});
    m *= utils::rotationMatrix(angle);
    min *= m;
    max *= m;
    // double a = angle * M_PI / 180;
    // double s = sin(a);
    // double c = cos(a);
    // double dx = 0;
    // double dy = displacement;
    // min[0] += dx * c + dy * s;
    // min[1] += dy * c - dx * s;
    // max[0] += dx * c + dy * s;
    // max[1] += dy * c - dx * s;
}

void Window::rotate(const double _angle) {
    angle = fmod(angle + _angle + 360, 360);
}

Point<2> Window::toViewport(const Viewport& viewport, Point<2>& p) {
    double width  = viewport.second[0] - viewport.first[0];
    double height = viewport.second[1] - viewport.first[1];
    Point<2> pn   = p.ndc();
    double x = (pn[0] + 1) / 2 * width + viewport.first[0];
    double y = (1 - (pn[1] + 1)/ 2) * height + viewport.first[1];
    //std::cout << "(" << x << "," << y << ")" << std::endl;
    return Point<2>(x, y);
}

void Window::zoom(const double zoomRate) {
    if (currentZoom + zoomRate > 0) {
        currentZoom += zoomRate;
        double factor = 1 / (2 * currentZoom);
        Point<2> delta(defaultWidth * factor, defaultHeight * factor);
        Point<2> center = (min + max) / 2;
        min = center - delta;
        max = center + delta;
    }
}

double Window::getAngle() {
    return angle;
}

double Window::getZoomLevel() {
    return currentZoom;
}

void Window::clip(Point<2>& p) {
    auto pn = p.ndc();
    p.setVisible(pn[0] >= -1 && pn[0] <= 1 && pn[1] >= -1 && pn[1] <= 1);
}

void Window::clip(Line<2>& ln) {
    std::cout << "Clipping..." << std::endl;
    clipCS(ln);
}

void Window::clip(Polygon<2>& p) {

}

void Window::clipCS(Line<2>& ln) {
    auto& p1 = ln[0].ndc();
    auto& p2 = ln[1].ndc();
    int rc1 = 0;
    int rc2 = 0;

    rc1 |= (p1[1] > 1);         // acima
    rc1 |= (p1[1] < -1) << 1;   // abaixo
    rc1 |= (p1[0] > 1) << 2;    // direita
    rc1 |= (p1[0] < -1) << 3;   // esquerda
    rc2 |= (p2[1] > 1);
    rc2 |= (p2[1] < -1) << 1;
    rc2 |= (p2[0] > 1) << 2;
    rc2 |= (p2[0] < -1) << 3;

    if (rc1 == 0 && rc2 == 0) {
        ln.setVisible(true);
        return;
    }

    if ((rc1 & rc2) != 0) {
        ln.setVisible(false);
        return;
    }

    ln.setVisible(true);
    int rc = (rc1 == 0) ? rc2 : rc1;
    double x, y;
    double m = utils::slope(Line<2>(p1, p2));
    if ((rc & 1) != 0) {
        x = p1[0] + (1 - p1[1]) / m;
        y = 1;
    } else if ((rc & 2) != 0) {
        x = p1[0] + (-1 - p1[1]) / m;
        y = -1;
    } else if ((rc & 4) != 0) {
        x = 1;
        y = p1[1] + m * (1 - p1[0]);
    } else {
        x = -1;
        y = p1[1] + m * (-1 - p1[0]);
    }

    if (rc == rc1) {
        std::cout << "(" << p1[0] << "," << p1[1] << ") -> (" << x << "," << y << ")" << std::endl;
        p1 = Point<2>(x, y);
    } else {
        std::cout << "(" << p2[0] << "," << p2[1] << ") -> (" << x << "," << y << ")" << std::endl;
        p2 = Point<2>(x, y);
    }

    clipCS(ln);
}

void Window::clipLB(Line<2>& ln) {
    auto& p1 = ln[0].ndc();
    auto& p2 = ln[1].ndc();

    double p[4], q[4];
    p[0] = -(p2[0] - p1[0]);
    p[1] = -p[0];
    p[2] = -(p2[1] - p1[1]);
    p[3] = -p[2];
    q[0] = p1[0] + 1;
    q[1] = 1 - p1[0];
    q[2] = p1[1] + 1;
    q[3] = 1 - p1[1];

    double u1 = 0;
    double u2 = 1;
    for (unsigned i = 0; i < 4; i++) {
        double r = q[i] / p[i];
        if (p[i] < 0) {
            if (r > u1) {
                u1 = r;
            }
        } else {
            if (r < u2) {
                u2 = r;
            }
        }
    }

    if (u1 > u2) {
        ln.setVisible(false);
        return;
    }

    ln.setVisible(true);
    double x1, y1, x2, y2;
    if (u1 != 0 && u1 != 1) {
        x1 = p1[0] + u1 * (p2[0] - p1[0]);
        y1 = p1[1] + u1 * (p2[1] - p1[1]);
    }

    if (u2 != 0 && u2 != 1) {
        x2 = p1[0] + u2 * (p2[0] - p1[0]);
        y2 = p1[1] + u2 * (p2[1] - p1[1]);
    }

    for (unsigned i = 0; i < 4; i++) {
        std::cout << "p[" << i << "] = " << p[i] << std::endl;
    }
    for (unsigned i = 0; i < 4; i++) {
        std::cout << "q[" << i << "] = " << q[i] << std::endl;
    }
    for (unsigned i = 0; i < 4; i++) {
        std::cout << "r[" << i << "] = " << q[i]/p[i] << std::endl;
    }
    std::cout << "u1 = " << u1 << std::endl;
    std::cout << "u2 = " << u2 << std::endl;

    if (u1 != 0 && u1 != 1) {
        std::cout << "(" << p1[0] << "," << p1[1] << ") -> (" << x1 << "," << y1 << ")" << std::endl;
        p1 = Point<2>(x1, y1);
    } else {
        std::cout << "(" << p1[0] << "," << p1[1] << ")" << std::endl;
    }

    if (u2 != 0 && u2 != 1) {
        std::cout << "(" << p2[0] << "," << p2[1] << ") -> (" << x2 << "," << y2 << ")" << std::endl;
        p2 = Point<2>(x2, y2);
    } else {
        std::cout << "(" << p2[0] << "," << p2[1] << ")" << std::endl;
    }
}