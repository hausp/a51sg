/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef ITERATIVE_ALGORITHM_HPP
#define ITERATIVE_ALGORITHM_HPP

#include <vector>
#include "CurveAlgorithm.hpp"

template<unsigned D>
class IterativeAlgorithm : public CurveAlgorithm<D> {
public:
    std::vector<Line<D>> update(double accuracy,
    	const std::vector<Matrix<4,1>>& coefs) const override {

	    unsigned numLines = 1/accuracy + 1;
	    std::vector<Line<D>> lines(numLines);

	    Matrix<1,4> tExponents;
	    tExponents[0][3] = 1;
	    unsigned j = 0;
	    for (double t = 0; t <= 1.001; t += accuracy) {
	        tExponents[0][2] = t;
	        tExponents[0][1] = t * t;
	        tExponents[0][0] = tExponents[0][1] * t;

	        Matrix<1,1> coord;
	        Point<D> p;
	        for (unsigned i = 0; i < D; i++) {
	            coord = tExponents * coefs[i];
	            p[i] = coord[0][0];
	        }
	        lines[j] = Line<D>(p, Point<D>());
	        if (j > 0) {
	            lines[j - 1][1] = p;
	        }
	        j++;
	    }
	    return lines;
    }
};

#endif /* ITERATIVE_ALGORITHM_HPP */