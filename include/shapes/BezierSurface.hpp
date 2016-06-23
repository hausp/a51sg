/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef BEZIER_SURFACE_HPP
#define BEZIER_SURFACE_HPP

#include "BicubicSurface.hpp"
#include "CurveAlgorithm.hpp"

class BezierSurface : public BicubicSurface {
 public:
    template<typename Iterable>
    BezierSurface(const CurveAlgorithm<3>& updater, double accuracyS, double accuracyT,
        const Iterable& params)
    : BicubicSurface(utils::BEZIER_MATRIX, updater, accuracyS, accuracyT, params) {
        BicubicSurface::build(params);
    }

    std::vector<std::vector<Point<3>>> parseParams(const std::vector<Point<3>>& params) override {
        // std::vector<std::vector<Point<D>>> paramGroups;
        // unsigned i = 0;
        // while(i < params.size()) {
        //     std::vector<Point<D>> points;
        //     bool copyLast = (i > 0);
        //     if (copyLast) {
        //         points.push_back(params[i-1]);
        //     }
        //     points.push_back(params[i]);
        //     points.push_back(params[i+1]);
        //     points.push_back(params[i+2]);
        //     if (!copyLast) {
        //         points.push_back(params[i+3]);
        //         i++;
        //     }
        //     i += 3;
        //     paramGroups.push_back(points);
        // }
        // return paramGroups;
        return std::vector<std::vector<Point<3>>>();
    }
};

#endif /* BEZIER_SURFACE_HPP */