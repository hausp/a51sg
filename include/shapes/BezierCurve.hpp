/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef BEZIER_CURVE_HPP
#define BEZIER_CURVE_HPP

#include "SimpleCurve.hpp"

template<unsigned D>
class BezierCurve : public SimpleCurve<D> {
 public:
    template<typename Iterable>
    BezierCurve(double accuracy, const Iterable& params)
    : SimpleCurve<D>(utils::BEZIER_MATRIX, accuracy, params) {}
};

#endif /* BEZIER_CURVE_HPP */