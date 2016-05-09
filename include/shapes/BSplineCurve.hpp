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

    std::vector<std::vector<Point<D>>> parseParams(const std::vector<Point<D>>& params) override {
        std::vector<std::vector<Point<D>>> paramGroups;
        for (unsigned i = 0; i < params.size() - 3; i++) {
            std::vector<Point<D>> points = {
                params[i], params[i+1],
                params[i+2], params[i+3]
            };
            paramGroups.push_back(points);
            for (auto p : points) {
                std::cout << "(" << p[0] << "," << p[1] << ") ";
            }
            std::cout << std::endl;
        }
        return paramGroups;
    }
};

#endif /* B_SPLINE_CURVE_HPP */