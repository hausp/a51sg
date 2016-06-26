/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef B_SPLINE_SURFACE_HPP
#define B_SPLINE_SURFACE_HPP

#include "BicubicSurface.hpp"
#include "CurveAlgorithm.hpp"

class BSplineSurface : public BicubicSurface {
 public:
    template<typename Iterable>
    BSplineSurface(const CurveAlgorithm<3>& updater, double accuracyS,
        double accuracyT, const Iterable& params)
    : BicubicSurface(utils::B_SPLINE_MATRIX, updater, accuracyS, accuracyT, params) {
        BicubicSurface::build(params);
    }

    std::vector<std::vector<Point<3>>> parseParams(const std::vector<Point<3>>& params) override {
        std::vector<std::vector<Point<3>>> paramGroups;
        for (unsigned i = 0; i < params.size(); i += 16) {
            // std::vector<Point<3>> points = {
            //     params[i], params[i+1],
            //     params[i+2], params[i+3]
            // };
            std::vector<Point<3>> points;
            for (unsigned j = 0; j < 16; j++) {
                points.push_back(params[i + j]);
            }
            paramGroups.push_back(points);
        }
        // for (auto& group : paramGroups) {
        //     ECHO("##############");
        //     TRACE_IT(group);
        //     ECHO("##############");
        // }
        return paramGroups;
    }
};

#endif /* B_SPLINE_SURFACE_HPP */