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
        // std::vector<std::vector<Point<D>>> paramGroups;
        // for (unsigned i = 0; i < params.size() - 3; i++) {
        //     std::vector<Point<D>> points = {
        //         params[i], params[i+1],
        //         params[i+2], params[i+3]
        //     };
        //     paramGroups.push_back(points);
        //     for (auto p : points) {
        //         std::cout << "(" << p[0] << "," << p[1] << ") ";
        //     }
        //     std::cout << std::endl;
        // }
        // return paramGroups;
        return std::vector<std::vector<Point<3>>>();
    }
};

#endif /* B_SPLINE_SURFACE_HPP */