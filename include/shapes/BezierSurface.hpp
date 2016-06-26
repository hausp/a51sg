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
        std::vector<std::vector<Point<3>>> paramGroups;
        unsigned i = 0;
        while(i < params.size()) {
            std::vector<Point<3>> points;
            // bool copyLast = (i >= 16);
            // if (copyLast) {
            //     points.push_back(params[i-4]);
            //     points.push_back(params[i-3]);
            //     points.push_back(params[i-2]);
            //     points.push_back(params[i-1]);
            // }
            // int numPointsCopied = (copyLast) ? 12 : 16;
            int numPointsCopied = 16;
            for (int j = 0; j < numPointsCopied; j++) {
                points.push_back(params[i + j]);
            }
            i += numPointsCopied;
            paramGroups.push_back(points);
        }
        return paramGroups;
    }
};

#endif /* BEZIER_SURFACE_HPP */