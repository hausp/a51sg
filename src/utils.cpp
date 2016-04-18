#include "Line.hpp"
#include "Matrix.hpp"
#include "Point.hpp"
#include "utils.hpp"


void utils::nln_rotate90c(Point<2>& p) {
    double temp = p[0];
    p[0] = p[1];
    p[1] = -temp;
}

void utils::nln_rotate180c(Point<2>& p) {
    p[0] = -p[0];
    p[1] = -p[1];
}

void utils::nln_rotate270c(Point<2>& p) {
    double temp = p[0];
    p[0] = -p[1];
    p[1] = temp;
}

void utils::nln_reflect_xmy(Point<2>& p) {
    double temp = p[0];
    p[0] = -p[1];
    p[1] = -temp;
}

void utils::nln_reflect_xaxis(Point<2>& p) {
    p[1] = -p[1];
}


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
