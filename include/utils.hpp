#ifndef UTILS_HPP
#define UTILS_HPP

#include <array>
#include <iostream>
#include <regex>
#include "Matrix.hpp"

#define RECENT_COMPILER 1

using color_t = std::tuple<unsigned, unsigned, unsigned, double>;

namespace {

	std::string currentRegex;
	std::regex regex;
}

namespace utils {
	inline bool regex_match(const std::string& text, const std::string& regexp) {
		#if RECENT_COMPILER
		if (regexp != currentRegex) {
			currentRegex = regexp;
			regex = std::regex(regexp);
		}
		return std::regex_match(text, regex);
		#else
		return true;
		#endif
	}

	template<size_t D>
	inline Matrix<D+1, D+1> translationMatrix(const std::array<double, D>& ds) {
		Matrix<D+1, D+1> matrix;
		for (unsigned i = 0; i < D + 1; i++) {
			matrix[i][i] = 1;
			if (i < D) {
				matrix[D][i] = ds[i];
			}
		}
		return matrix;
	}

	template<size_t D>
	inline Matrix<D+1, D+1> scalingMatrix(const std::array<double, D>& ds) {
		Matrix<D+1, D+1> matrix;
		for (unsigned i = 0; i < D; i++) {
			matrix[i][i] = ds[i];
		}
		matrix[D][D] = 1;
		return matrix;
	}

	inline Matrix<3,3> rotationMatrix(const double angle) {
	    double a = angle * M_PI / 180;
	    double s = sin(a);
	    double c = cos(a);
	    return {{c,s,0},{-s,c,0},{0,0,1}};
	}
}

#endif