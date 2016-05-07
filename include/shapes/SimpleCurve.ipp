/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "Point.hpp"
#include "Drawer.hpp"
#include "Matrix.hpp"
#include "Window.hpp"

template<unsigned D>
void SimpleCurve<D>::updateIterative(const std::vector<Matrix<4,1>>& coefficients) {
    unsigned numLines = 1/accuracy + 1;
    lines.clear();
    lines.resize(numLines);

    Matrix<1,4> tExponents;
    tExponents[0][3] = 1;
    unsigned j = 0;
    double t;
    for (t = 0; t <= 1.001; t += accuracy) {
        tExponents[0][2] = t;
        tExponents[0][1] = t * t;
        tExponents[0][0] = tExponents[0][1] * t;

        Matrix<1,1> coord;
        Point<D> p;
        for (unsigned i = 0; i < D; i++) {
            coord = tExponents * coefficients[i];
            p[i] = coord[0][0];
        }
        lines[j] = Line<D>(p, Point<D>());
        if (j > 0) {
            lines[j - 1][1] = p;
        }
        j++;
    }
}

template<unsigned D>
void SimpleCurve<D>::updateForwardDifference(const std::vector<Matrix<4,1>>& coefs) {
    unsigned numLines = 1/accuracy + 1;
    lines.clear();
    lines.resize(numLines);

    std::vector<double> powers(3, accuracy);
    powers[1] *= accuracy;  // accuracy^2
    powers[2] *= powers[1]; // accuracy^3
    std::vector<std::vector<double>> params;
    params.reserve(D);
    for (unsigned i = 0; i < D; i++) {
        std::vector<double> container = {
            coefs[i][3][0],
            coefs[i][0][0] * powers[2] + coefs[i][1][0] * powers[1] + coefs[i][2][0] * powers[0],
            6 * coefs[i][0][0] * powers[2] + 2 * coefs[i][1][0] * powers[1],
            6 * coefs[i][0][0] * powers[2]
        };
        params.push_back(container);
    }

    // for (unsigned i = 0; i < D; i++) {
    //     for (unsigned j = 0; j < 4; j++) {
    //         std::cout << "coefs[" << i << "][" << j << "][0] = " << coefs[i][j][0] << std::endl;
    //     }
    // }

    for (auto& v : params) {
        for (auto d : v) {
            std::cout << d << " ";
        }
        std::cout << std::endl;
    }

    for (unsigned i = 0; i < numLines; i++) {
        Point<D> p;
        for (unsigned j = 0; j < D; j++) {
            if (i > 0) {
                params[j][0] += params[j][1];
                params[j][1] += params[j][2];
                params[j][2] += params[j][3];
            }
            p[j] = params[j][0];
        }

        lines[i] = Line<D>(p, Point<D>());
        if (i > 0) {
            lines[i - 1][1] = p;
        }
    }
}

template<unsigned D>
void SimpleCurve<D>::draw(Drawer<D>& drawer) {
    drawer.draw(*this);
}

template<unsigned D>
void SimpleCurve<D>::clip(Window& window) {
    window.clip(*this);
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
void SimpleCurve<D>::update(const Matrix<D+1,D+1>& matrix) {
    for (auto& line : lines) {
        line.update(matrix);
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
