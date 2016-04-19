/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <array>
#include <iostream>
#include <regex>

template<unsigned D>
class Point;
template<unsigned D>
class Line;
template<unsigned R, unsigned C>
class Matrix;

#define RECENT_COMPILER 1

using color_t = std::tuple<unsigned, unsigned, unsigned, double>;

namespace {
    std::string currentRegex;
    std::regex regex;
}

namespace utils {

    const std::string REGEX_INTEGER = "^(\\+|-)?\\d+";
    const std::string REGEX_REAL_UNSIGNED = "^\\d+(\\.\\d+)?";
    const std::string REGEX_REAL = "^(\\+|-)?\\d+(\\.\\d+)?";

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

    Matrix<3,3> rotationMatrix(const double angle);
    double slope(const Line<2>& line);
}

#endif