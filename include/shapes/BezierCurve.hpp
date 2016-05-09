/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef BEZIER_CURVE_HPP
#define BEZIER_CURVE_HPP

#include "Curve.hpp"
#include "CurveAlgorithm.hpp"

template<unsigned D>
class BezierCurve : public Curve<D> {
 public:
    template<typename Iterable>
    BezierCurve(const CurveAlgorithm<D>& updater, double accuracy, const Iterable& params)
    : Curve<D>(utils::BEZIER_MATRIX, updater, accuracy, params) {}
};

#endif /* BEZIER_CURVE_HPP */