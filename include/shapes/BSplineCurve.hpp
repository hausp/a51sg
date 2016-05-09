/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef B_SPLINE_CURVE_HPP
#define B_SPLINE_CURVE_HPP

#include "Curve.hpp"
#include "CurveAlgorithm.hpp"

template<unsigned D>
class BSplineCurve : public Curve<D> {
 public:
    template<typename Iterable>
    BSplineCurve(const CurveAlgorithm<D>& updater, double accuracy, const Iterable& params)
    : Curve<D>(utils::B_SPLINE_MATRIX, updater, accuracy, params) {}
};

#endif /* B_SPLINE_CURVE_HPP */