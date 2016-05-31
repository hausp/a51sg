/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <array>
#include <iostream>
#include <regex>
#include <cmath>
#include "Matrix.hpp"

template<unsigned D>
class Point;
template<unsigned D>
class Line;

#define RECENT_COMPILER 1
#define TRACE(x) std::cout << (#x) << " = " << (x) << std::endl
#define TRACE_L(x,y) std::cout << (x) << " = " << (y) << std::endl
#define TRACE_IT(x) \
    unsigned counter = 0; \
    for (auto& elem : (x)) { \
        std::cout << (#x) << "[" << std::to_string(counter++) << "] = " << elem << std::endl; \
    }
#define ECHO(x) std::cout << (x) << std::endl

using color_t = std::tuple<unsigned, unsigned, unsigned, double>;

namespace {
    std::string currentRegex;
    std::regex regex;
}

namespace utils {

    const std::string REGEX_INTEGER = "^(\\+|-)?\\d+";
    const std::string REGEX_INTEGER_UNSIGNED = "^\\d+";
    const std::string REGEX_REAL_UNSIGNED = "^\\d+(\\.\\d+)?";
    const std::string REGEX_REAL = "^(\\+|-)?\\d+(\\.\\d+)?";
    const Matrix<4,4> BEZIER_MATRIX = {
        {-1, 3, -3, 1},
        {3, -6, 3, 0},
        {-3, 3, 0, 0},
        {1, 0, 0, 0}
    };
    const Matrix<4,4> B_SPLINE_MATRIX = {
        {-1.0/6, 1.0/2, -1.0/2, 1.0/6},
        {1.0/2, -1, 1.0/2, 0},
        {-1.0/2, 0, 1.0/2, 0},
        {1.0/6, 2.0/3, 1.0/6, 0}
    };

    enum class RotationPlane {
        X, Y, Z
    };

    bool regex_match(const std::string& text, const std::string& regexp);

    void nln_rotate90c(Point<2>&);
    void nln_rotate180c(Point<2>&);
    void nln_rotate270c(Point<2>&);
    void nln_reflect_xmy(Point<2>&);
    void nln_reflect_xaxis(Point<2>&);

    template<size_t D>
    inline Matrix<D+1, D+1> translationMatrix(const std::array<double, D>& ds) {
        Matrix<D+1, D+1> matrix;
        for (unsigned i = 0; i < D + 1; i++) {
            matrix[i][i] = 1;
            if (i < D) {
                matrix[D][i] = ds[i];
            }
        }
        return matrix;
    }

    template<size_t D>
    inline Matrix<D+1, D+1> scalingMatrix(const std::array<double, D>& ds) {
        Matrix<D+1, D+1> matrix;
        for (unsigned i = 0; i < D; i++) {
            matrix[i][i] = ds[i];
        }
        matrix[D][D] = 1;
        return matrix;
    }

    template<size_t D>
    Matrix<D+1,D+1> rotationMatrix(double angle, RotationPlane plane = RotationPlane::Z) {
        double a = angle * M_PI / 180;
        double s = sin(a);
        double c = cos(a);
        auto planeIndex = static_cast<int>(plane);
        Matrix<D+1, D+1> matrix;
        for (unsigned i = 0; i < D + 1; i++) {
            matrix[i][i] = 1;
        }
        auto x = (planeIndex + 1) % (D + 1);
        auto y = (planeIndex - 1) % (D + 1);
        matrix[x][x] = c;
        matrix[y][y] = c;
        matrix[x][y] = -s;
        matrix[y][x] = s;
        return matrix;
    }

    double slope(const Line<2>& line);
}

#endif
