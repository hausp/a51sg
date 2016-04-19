/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
and Marleson Graf<aszdrick@gmail.com> [2016] */

#include <cmath>
#include "Line.hpp"
#include "Polygon.hpp"
#include "Window.hpp"

#define XMIN -1
#define YMIN -1
#define XMAX 1
#define YMAX 1

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
    clipNLN(ln);
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
    double dx = p2[0] - p1[0];
    double dy = p2[1] - p1[1];
    double u1 = 0;
    double u2 = 1;
    double p[4], q[4];

    p[0] = -(p2[0] - p1[0]);
    p[1] = -p[0];
    p[2] = -(p2[1] - p1[1]);
    p[3] = -p[2];
    q[0] = p1[0] + 1;
    q[1] = 1 - p1[0];
    q[2] = p1[1] + 1;
    q[3] = 1 - p1[1];

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

    if (u2 != 0 && u2 != 1) {
        p2[0] = p1[0] + u2 * dx;
        p2[1] = p1[1] + u2 * dy;
    }

    if (u1 != 0 && u1 != 1) {
        p1[0] += u1 * dx;
        p1[1] += u1 * dy;
    }
}
    // for (unsigned i = 0; i < 4; i++) {
    //     std::cout << "p[" << i << "] = " << p[i] << std::endl;
    // }
    // for (unsigned i = 0; i < 4; i++) {
    //     std::cout << "q[" << i << "] = " << q[i] << std::endl;
    // }
    // for (unsigned i = 0; i < 4; i++) {
    //     std::cout << "r[" << i << "] = " << q[i]/p[i] << std::endl;
    // }
    // std::cout << "u1 = " << u1 << std::endl;
    // std::cout << "u2 = " << u2 << std::endl;

    // if (u1 != 0 && u1 != 1) {
    //     std::cout << "(" << p1[0] << "," << p1[1] << ") -> (" << x1 << "," << y1 << ")" << std::endl;
    //     p1 = Point<2>(x1, y1);
    // } else {
    //     std::cout << "(" << p1[0] << "," << p1[1] << ")" << std::endl;
    // }

    // if (u2 != 0 && u2 != 1) {
    //     std::cout << "(" << p2[0] << "," << p2[1] << ") -> (" << x2 << "," << y2 << ")" << std::endl;
    //     p2 = Point<2>(x2, y2);
    // } else {
    //     std::cout << "(" << p2[0] << "," << p2[1] << ")" << std::endl;
    // }
// }

void Window::clipNLN(Line<2>& ln) {
    auto& p1 = ln[0].ndc();
    auto& p2 = ln[1].ndc();

    if (p1[0] < XMIN) {
        // P1 in left column
        ln.setVisible(leftcolumn(p1, p2));
    } else if (p1[0] > XMAX) {
        // P1 in right column, rotate 180°,
        // moving it to left column
        utils::nln_rotate180c(p1);
        utils::nln_rotate180c(p2);
        ln.setVisible(leftcolumn(p1, p2));
        // Rotate back
        utils::nln_rotate180c(p1);
        utils::nln_rotate180c(p2);
    } else {
        // P1 is in center column
        ln.setVisible(centercolumn(p1, p2));
    }
}

bool Window::leftcolumn(Point<2>& p1, Point<2>& p2) {
    if (p2[0] < XMIN) {
        // Line is out of window
        return false;
    }
    if (p1[1] > YMAX) {
        // P1 is in top left corner
        return topleftcorner(p1, p2);
    }
    if (p1[1] < YMIN) {
        // P1 is in bottom left corner, reflect in x axis,
        // moving to top left corner
        utils::nln_reflect_xaxis(p1);
        utils::nln_reflect_xaxis(p2);
        bool visible = topleftcorner(p1, p2);
        // reflect back
        utils::nln_reflect_xaxis(p1);
        utils::nln_reflect_xaxis(p2);
        return visible;
    }
    return leftedge(p1, p2);
}

bool Window::topleftcorner(Point<2>& p1, Point<2>& p2) {
    if (p2[1] > YMAX) {
        // Line is out of window
        return false;
    }

    double m0 = (p2[1] - p1[1]) / (p2[0] - p1[0]);
    double m1 = (YMAX - p1[1]) / (XMIN - p1[0]);

    if (m0 <= m1) {
        return leftbottomregion(p1, p2, m0);
    } else {
        utils::nln_reflect_xmy(p1);
        utils::nln_reflect_xmy(p2);
        bool visible = leftbottomregion(p1, p2, 1/m0);
        utils::nln_reflect_xmy(p1);
        utils::nln_reflect_xmy(p2);
        return visible;
    }
}

bool Window::leftbottomregion(Point<2>& p1, Point<2>& p2, double m0) {
    if (p2[1] >= YMIN) {
        if (p2[0] > XMAX) {
            p2[0] = XMAX;
            p2[1] = p1[1] + (XMAX - p1[0]) * m0;
        }
    } else {
        double m2 = (YMIN - p1[1]) / (XMIN - p1[0]);
        if (m2 >= m0) {
            return false;
        }
        if (p2[0] > XMAX) {
            double m3 = (YMIN - p1[1]) / (XMAX - p1[0]);
            if (m3 > m0) {
                p2[0] = p1[0] + (YMIN - p1[1]) / m0;
                p2[1] = YMIN;
            } else {
                p2[0] = XMAX;
                p2[1] = p1[1] + (XMAX - p1[0]) * m0;
            }
        } else {
            p2[0] = p1[0] + (YMIN - p1[1]) / m0;
            p2[1] = YMIN;
        }
    }

    p1[1] += (XMIN - p1[0]) * m0;
    p1[0] = XMIN;
    return true;
}

bool Window::leftedge(Point<2>& p1, Point<2>& p2) {
    if (p2[0] < XMIN) {
        return false;
    }

    if (p2[1] < YMIN) {
        return p2bottom(p1, p2);
    }

    if (p2[1] > YMAX) {
        utils::nln_reflect_xaxis(p1);
        utils::nln_reflect_xaxis(p2);
        bool visible = p2bottom(p1, p2);
        utils::nln_reflect_xaxis(p1);
        utils::nln_reflect_xaxis(p2);
        return visible;
    }

    double m0 = (p2[1] - p1[1]) / (p2[0] - p1[0]);
    if (p2[0] > XMAX) {
        p2[0] = XMAX;
        p2[1] = p1[1] + (XMAX - p1[0]) * m0;
    }
    p1[1] += (XMIN - p1[0]) * m0;
    p1[0] = XMIN;
    return true;
}

bool Window::p2bottom(Point<2>& p1, Point<2>& p2) {
    double m0 = (p2[1] - p1[1]) / (p2[0] - p1[0]);
    double m2 = (YMIN - p1[1]) / (XMIN - p1[0]);

    if (m2 >= m0) {
        return false;
    }
    if (p2[0] <= XMAX) {
        p2[0] = p1[0] + (YMIN - p1[1]) / m0;
        p2[1] = YMIN;
    } else {
        double m3 = (YMIN - p1[1]) / (XMAX - p1[0]);
        if (m3 > m0) {
            p2[0] = p1[0] + (YMIN - p1[1]) / m0;
            p2[1] = YMIN;
        } else {
            p2[0] = XMAX;
            p2[1] = p1[1] + (XMAX - p1[0]) * m0;
        }
    }
    p1[1] += (XMIN - p1[0]) * m0;
    p1[0] = XMIN;
    return true;
}

bool Window::centercolumn(Point<2>& p1, Point<2>& p2) {
    if (p1[1] > YMAX) {
        utils::nln_rotate270c(p1);
        utils::nln_rotate270c(p2);
        bool visible = leftedge(p1, p2);
        utils::nln_rotate90c(p1);
        utils::nln_rotate90c(p2);
        return visible;
    }

    if (p1[1] < YMIN) {
        utils::nln_rotate90c(p1);inside
        utils::nln_rotate90c(p2);
        bool visible = leftedge(p1, p2);
        utils::nln_rotate270c(p1);
        utils::nln_rotate270c(p2);
        return visible;
    }
    return inside(p1, p2);
}

bool Window::inside(Point<2>& p1, Point<2>& p2) {
    if (p2[0] < XMIN) {
        p2left(p1, p2);
    } else if (p2[0] > XMAX) {
        utils::nln_rotate180c(p1);
        utils::nln_rotate180c(p2);
        p2left(p1, p2);
        utils::nln_rotate180c(p1);
        utils::nln_rotate180c(p2);
    } else if (p2[1] > YMAX) {
        p2[0] = p1[0] + (YMAX - p1[1]) * (p2[0] - p1[0]) / (p2[1] - p1[1]);
        p2[1] = YMAX;
    } else if (p2[1] < YMIN) {
        p2[0] = p1[0] + (YMIN - p1[1]) * (p2[0] - p1[0]) / (p2[1] - p1[1]);
        p2[1] = YMIN;
    }
    return true;
}

void Window::p2left(Point<2>& p1, Point<2>& p2) {
    if (p2[1] > YMAX) {
        p2lefttop(p1, p2);
    } else if (p2[1] < YMIN) {
        utils::nln_rotate90c(p1);
        utils::nln_rotate90c(p2);
        p2lefttop(p1, p2);
        utils::nln_rotate270c(p1);
        utils::nln_rotate270c(p2);
    } else {
        p2[1] = p1[1] + (XMIN - p1[0]) * (p2[1] - p1[1]) / (p2[0] - p1[0]);
        p2[0] = XMIN;
    }
}

void Window::p2lefttop(Point<2>& p1, Point<2>& p2) {
    double m0 = (p2[1] - p1[1]) / (p2[0] - p1[0]);
    double m1 = (YMAX - p1[1]) / (XMIN - p1[0]);

    if (m0 < m1) {
        p2[0] = p1[0] + (YMAX - p1[1]) / m0;
        p2[1] = YMAX;
    } else {
        p2[0] = XMIN;
        p2[1] = p1[1] + (XMIN - p1[0]) * m0;
    }
}