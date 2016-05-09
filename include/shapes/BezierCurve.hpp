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
    : Curve<D>(utils::BEZIER_MATRIX, updater, accuracy, params) {
    	Curve<D>::build(params);
    }

    std::vector<std::vector<Point<D>>> parseParams(const std::vector<Point<D>>& params) override {
        std::vector<std::vector<Point<D>>> paramGroups;
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
            paramGroups.push_back(points);
        }
        return paramGroups;
    }
};

#endif /* BEZIER_CURVE_HPP */