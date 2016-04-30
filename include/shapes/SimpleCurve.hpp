/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef SIMPLE_CURVE_HPP
#define SIMPLE_CURVE_HPP

#include <iostream>
#include <vector>
#include "Drawable.hpp"

template<unsigned D>
class Line;

template<unsigned R, unsigned C>
class Matrix;

template<unsigned D>
class SimpleCurve : public Drawable<D> {
 public:
    template<typename Iterable>
    SimpleCurve(double accuracy, const Iterable& params)
    : SimpleCurve<D>(Matrix<4,4>(), accuracy, params) {}

    void update() {
        unsigned numLines = 1/accuracy + 1;
        lines.clear();
        lines.resize(numLines);

        std::vector<Matrix<4,1>> coefficients(D);
        for (unsigned i = 0; i < D; i++) {
            coefficients[i] = methodMatrix * geometryVectors[i];
        }

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
            lines[j] = Line<D>(p, Point<D>(0, 0));
            if (j > 0) {
                lines[j - 1][1] = p;
            }
            j++;
        }
    }

    void draw(Drawer<D>&) override;
    void clip(Window&) override;
    void transform(const Matrix<D+1,D+1>&) override;
    Point<D> center() const override;
    std::vector<Point<D>> points() const override;
    void update(const Matrix<D+1,D+1>&) override;

    typename std::vector<Line<D>>::iterator begin();
    typename std::vector<Line<D>>::const_iterator begin() const;
    typename std::vector<Line<D>>::iterator end();
    typename std::vector<Line<D>>::const_iterator end() const;

 protected:
    template<typename Iterable>
    SimpleCurve(const Matrix<4,4>& matrix, double accuracy, const Iterable& params)
    : Drawable<D>("", DrawableType::Curve), accuracy(accuracy), methodMatrix(matrix) {
        geometryVectors.resize(D);
        unsigned j = 0;
        for (Point<D> p : params) {
            for (unsigned i = 0; i < D; i++) {
                geometryVectors[i][j][0] = p[i];
            }
            j++;
        }
        update();
    }

 private:
    double accuracy;
    Matrix<4,4> methodMatrix;
    std::vector<Matrix<4,1>> geometryVectors;
    std::vector<Line<D>> lines;
};

#include "SimpleCurve.ipp"

#endif /* SIMPLE_CURVE_HPP */