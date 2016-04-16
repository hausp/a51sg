#include "Line.hpp"
#include "Matrix.hpp"
#include "utils.hpp"

bool utils::regex_match(const std::string& text, const std::string& regexp) {
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

Matrix<3,3> utils::rotationMatrix(const double angle) {
    double a = angle * M_PI / 180;
    double s = sin(a);
    double c = cos(a);
    return {{c,s,0},{-s,c,0},{0,0,1}};
}

double utils::slope(const Line<2>& line) {
	return (line[1][1] - line[0][1]) / (line[1][0] - line[0][0]);
}
