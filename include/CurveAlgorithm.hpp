/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef CURVE_ALGORITHM_HPP
#define CURVE_ALGORITHM_HPP

#include <vector>

template<unsigned D>
class Line;

template<unsigned R, unsigned C>
class Matrix;

template<unsigned D>
class CurveAlgorithm {
public:
    virtual std::vector<Line<D>> update(double accuracy, const std::vector<Matrix<4,1>>&) const = 0;
};

#endif /* CURVE_ALGORITHM_HPP */