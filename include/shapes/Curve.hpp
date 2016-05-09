/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef CURVE_HPP
#define CURVE_HPP

#include <vector>
#include "Drawable.hpp"
#include "CurveAlgorithm.hpp"

template<unsigned D>
class SimpleCurve;

template<unsigned R, unsigned C>
class Matrix;

template<unsigned D>
class Curve : public Drawable<D> {
 public:
    template<typename Iterable>
    Curve(const CurveAlgorithm<D>& updater, double accuracy, const Iterable& params)
    : Curve<D>(Matrix<4,4>(), updater, accuracy, params) {}

    template<typename Iterable>
    Curve(const Matrix<4,4>& matrix, const CurveAlgorithm<D>& updater,
        double accuracy, const Iterable& params)
    : Drawable<D>("", DrawableType::Curve), accuracy(accuracy), methodMatrix(matrix) {
        unsigned i = 0;
        while(i < params.size()) {
            std::vector<Point<D>> points;
            bool copyLast = (i > 0);
            if (copyLast) {
                points.push_back(params[i-1]);
            }
            points.push_back(params[i]);
            points.push_back(params[i+1]);
            points.push_back(params[i+2]);
            if (!copyLast) {
                points.push_back(params[i+3]);
                i++;
            }
            i += 3;
            SimpleCurve<D> curve(methodMatrix, updater, accuracy, points);
            curves.push_back(curve);
        }
    }

    void draw(Drawer<D>&) override;
    void clip(Window&) override;
    void transform(const Matrix<D+1,D+1>&) override;
    Point<D> center() const override;
    std::vector<Point<D>> points() const override;
    void update(const Matrix<D+1,D+1>&) override;

    typename std::vector<SimpleCurve<D>>::iterator begin();
    typename std::vector<SimpleCurve<D>>::const_iterator begin() const;
    typename std::vector<SimpleCurve<D>>::iterator end();
    typename std::vector<SimpleCurve<D>>::const_iterator end() const;

 private:
    double accuracy;
    Matrix<4,4> methodMatrix;
    std::vector<SimpleCurve<D>> curves;
};

#include "Curve.ipp"

#endif /* CURVE_HPP */