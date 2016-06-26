/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef FORWARD_DIFFERENCE_ALGORITHM_HPP
#define FORWARD_DIFFERENCE_ALGORITHM_HPP

#include <vector>
#include "CurveAlgorithm.hpp"

template<unsigned D>
class ForwardDifferenceAlgorithm : public CurveAlgorithm<D> {
public:
    std::vector<Line<D>> update(double accuracy,
    	const std::vector<Matrix<4,1>>& coefs) const override {

	    unsigned numLines = 1/accuracy + 1;
	    std::vector<Line<D>> lines(numLines);
	    std::vector<double> powers(3, accuracy);
	    powers[1] *= accuracy;  // accuracy^2
	    powers[2] *= powers[1]; // accuracy^3
	    std::vector<std::vector<double>> params;
	    params.reserve(D);
	    for (unsigned i = 0; i < D; i++) {
	        std::vector<double> container = {
	            coefs[i][3][0],
	            coefs[i][0][0] * powers[2] + coefs[i][1][0] * powers[1] + coefs[i][2][0] * powers[0],
	            6 * coefs[i][0][0] * powers[2] + 2 * coefs[i][1][0] * powers[1],
	            6 * coefs[i][0][0] * powers[2]
	        };
	        params.push_back(container);
	    }

	    for (unsigned i = 0; i < numLines; i++) {
	        Point<D> p;
	        for (unsigned j = 0; j < D; j++) {
	            if (i > 0) {
	                params[j][0] += params[j][1];
	                params[j][1] += params[j][2];
	                params[j][2] += params[j][3];
	            }
	            p[j] = params[j][0];
	        }

	        lines[i] = Line<D>(p, Point<D>());
	        if (i > 0) {
	            lines[i - 1][1] = p;
	        }
	    }
	    lines.pop_back();
	    return lines;
	}

    std::vector<Line<D>> update(double accuracyS, double accuracyT,
    	const std::vector<Matrix<4,4>>& coefs) const override {

    	// TODO
    	std::vector<Line<D>> lines;
    	return lines;
    }
};

#endif /* FORWARD_DIFFERENCE_ALGORITHM_HPP */