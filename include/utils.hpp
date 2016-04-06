#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <regex>

#define RECENT_COMPILER 1

using color_t = std::tuple<unsigned, unsigned, unsigned, double>;

namespace {
	std::string currentRegex;
	std::regex regex;
}

namespace utils {
	inline bool regex_match(const std::string& text, const std::string& regexp) {
		if (regexp != currentRegex) {
			currentRegex = regexp;
			regex = std::regex(regexp);
		}
		#if RECENT_COMPILER
		return std::regex_match(text, regex);
		#else
		return true;
		#endif
	}
}

#endif