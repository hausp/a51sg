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
    : Drawable<D>("", DrawableType::Curve), accuracy(accuracy), 
      updater(updater), methodMatrix(matrix) {}

    template<typename Iterable>
    void build(const Iterable& params) {
        auto paramGroups = parseParams(params);
        for (auto& group : paramGroups) {
            SimpleCurve<D> curve(methodMatrix, updater, accuracy, group);
            curves.push_back(curve);
        }        
    }

    virtual std::vector<std::vector<Point<D>>> parseParams(const std::vector<Point<D>>&) = 0;

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
    const CurveAlgorithm<D>& updater;
    Matrix<4,4> methodMatrix;
    std::vector<SimpleCurve<D>> curves;
};

#include "Curve.ipp"

#endif /* CURVE_HPP */