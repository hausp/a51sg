/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef SIMPLE_CURVE_HPP
#define SIMPLE_CURVE_HPP

#include <iostream>
#include <vector>
#include "CurveAlgorithm.hpp"
#include "Drawable.hpp"

template<unsigned D>
class Line;

template<unsigned R, unsigned C>
class Matrix;

template<unsigned D>
class SimpleCurve : public Drawable<D> {
 public:
    template<typename Iterable>
    SimpleCurve(const CurveAlgorithm<D>& updater, double accuracy, const Iterable& params)
    : SimpleCurve<D>(Matrix<4,4>(), updater, accuracy, params) {}

    template<typename Iterable>
    SimpleCurve(const Matrix<4,4>& matrix, const CurveAlgorithm<D>& updater,
        double accuracy,const Iterable& params)
    : Drawable<D>("", DrawableType::Curve), accuracy(accuracy), methodMatrix(matrix) {
        geometryVectors.resize(D);
        unsigned j = 0;
        for (Point<D> p : params) {
            for (unsigned i = 0; i < D; i++) {
                geometryVectors[i][j][0] = p[i];
            }
            j++;
        }

        std::vector<Matrix<4,1>> coefficients(D);
        for (unsigned i = 0; i < D; i++) {
            coefficients[i] = methodMatrix * geometryVectors[i];
        }
        lineList = updater.update(accuracy, coefficients);
    }

    template<unsigned Dn>
    SimpleCurve(const SimpleCurve<Dn>& c) : Drawable<D>("", DrawableType::Curve) {
        for (auto& line : c) {
            lineList.push_back(line);
        }
    }

    void draw(BaseDrawer<D>&) override;
    void clip(Window&) override;
    void transform(const Matrix<D+1,D+1>&) override;
    Point<D> center() const override;
    std::vector<Point<D>> points() const override;
    std::vector<Line<D>>& lines();
    void update(const Matrix<3,3>&, const Window&) override;

    typename std::vector<Line<D>>::iterator begin();
    typename std::vector<Line<D>>::const_iterator begin() const;
    typename std::vector<Line<D>>::iterator end();
    typename std::vector<Line<D>>::const_iterator end() const;

 private:
    double accuracy;
    Matrix<4,4> methodMatrix;
    std::vector<Matrix<4,1>> geometryVectors;
    std::vector<Line<D>> lineList;
};

#include "SimpleCurve.ipp"

#endif /* SIMPLE_CURVE_HPP */