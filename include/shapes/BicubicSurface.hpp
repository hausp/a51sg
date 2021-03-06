/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef BICUBIC_SURFACE_HPP
#define BICUBIC_SURFACE_HPP

#include <vector>
#include "Drawable.hpp"
#include "CurveAlgorithm.hpp"

template<unsigned D>
class SimpleCurve;

template<unsigned R, unsigned C>
class Matrix;

class BicubicSurface : public Drawable<3> {
 public:
    template<typename Iterable>
    BicubicSurface(const CurveAlgorithm<3>& updater, double accuracyS, double accuracyT,
        const Iterable& params)
    : BicubicSurface(Matrix<4,4>(), updater, accuracyS, accuracyT, params) {}

    template<typename Iterable>
    BicubicSurface(const Matrix<4,4>& matrix, const CurveAlgorithm<3>& updater,
        double accuracyS, double accuracyT, const Iterable& params)
    : Drawable<3>("", DrawableType::BicubicSurface), accuracyS(accuracyS), 
      accuracyT(accuracyT), updater(updater), methodMatrix(matrix) {}

    BicubicSurface(const BicubicSurface& curve)
    : Drawable<3>("", DrawableType::BicubicSurface), updater(curve.updater) {
        for (auto& c : curve) {
            curves.push_back(c);
        }
    }

    template<typename Iterable>
    void build(const Iterable& params) {
        auto paramGroups = parseParams(params);
        for (auto& group : paramGroups) {
            SimpleCurve<3> curve(methodMatrix, updater, accuracyS, accuracyT, group);
            curves.push_back(curve);
        }
    }

    virtual std::vector<std::vector<Point<3>>> parseParams(const std::vector<Point<3>>&) = 0;

    void draw(BaseDrawer<3>&) override;
    void clip(Window&) override;
    void transform(const Matrix<4,4>&) override;
    Point<3> center() const override;
    std::vector<Point<3>> points() const override;

    typename std::vector<SimpleCurve<3>>::iterator begin();
    typename std::vector<SimpleCurve<3>>::const_iterator begin() const;
    typename std::vector<SimpleCurve<3>>::iterator end();
    typename std::vector<SimpleCurve<3>>::const_iterator end() const;

 private:
    double accuracyS;
    double accuracyT;
    const CurveAlgorithm<3>& updater;
    Matrix<4,4> methodMatrix;
    std::vector<SimpleCurve<3>> curves;
};

#endif /* BICUBIC_SURFACE_HPP */