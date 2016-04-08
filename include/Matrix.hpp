/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <functional>
#include <vector>

template<unsigned R = 3, unsigned C = 3>
class Matrix {
public:
    /*template<typename ...Args>
    Matrix(typename std::enable_if<sizeof...(Args)+1 == R, const std::initializer_list<double>&>::type c,
          const Args... args) {
        init(c, args...);
    }*/
    Matrix() {
    	for (unsigned i = 0; i < R; i++) {
    		std::vector<double> container(C);
    		data.push_back(container);
    	}
    }

    Matrix(const std::initializer_list<std::initializer_list<double>>& matrix) {
    	for (auto row : matrix) {
    		std::vector<double> container;
    		for (auto value : row) {
    			container.push_back(value);
    		}
    		data.push_back(container);
    	}
    }

	std::vector<double>& operator[](unsigned index) {
		return data.at(index);
	}

	const std::vector<double>& operator[](unsigned index) const {
		return data.at(index);
	}

	Matrix<R,C> operator+=(const Matrix<R,C>& m) {
		return process(m, std::plus<double>());
	}

	Matrix<R,C> operator-=(const Matrix<R,C>& m) {
		return process(m, std::minus<double>());
	}

	Matrix<R,C> operator*=(typename std::enable_if<R == C, const Matrix<R,R>&> m) {
		*this = *this * m;
		return *this;
	}

	template<unsigned C2>
	Matrix<R,C2> operator*(const Matrix<C,C2>& m) {
		Matrix<R,C2> r;
		for (unsigned i = 0; i < R; i++) {
			for (unsigned j = 0; j < C2; j++) {
				r[i][j] = 0;
				for (unsigned k = 0; k < C; k++) {
					r[i][j] += (*this)[i][k] * m[k][j];
				}
			}
		}
		return r;
	}

	Matrix<R,C> operator+(const Matrix<R,C>& m) const {
		Matrix<R,C> r = *this;
		return r += m;
	}

	Matrix<R,C> operator-(const Matrix<R,C>& m) const {
		Matrix<R,C> r = *this;
		return r -= m;
	}

	Matrix<R,C> operator*(const double value) const {
		Matrix<R,C> r;
		for (unsigned i = 0; i < R; i++) {
			for (unsigned j = 0; j < C; j++) {
				r[i][j] = (*this)[i][j] * value;
			}
		}
		return r;		
	}

	typename std::vector<std::vector<double>>::iterator begin() {
		return data.begin();
	}

	typename std::vector<std::vector<double>>::iterator end() {
		return data.end();
	}

	typename std::vector<std::vector<double>>::const_iterator begin() const {
		return data.cbegin();
	}

	typename std::vector<std::vector<double>>::const_iterator end() const {
		return data.cend();
	}

private:
	std::vector<std::vector<double>> data;

    /*template<typename ...Args>
    void init(const std::initializer_list<double>& c, const Args... args) {
    	if (c.size() != C) {
    		throw "banana";
    	}
        data.push_back(std::vector<double>(c.begin(), c.end()));
        init(args...);
    }
    void init() {}*/

	Matrix<R,C> process(const Matrix<R,C>& m, const std::function<double(double, double)>& fn) {
		for (unsigned i = 0; i < R; i++) {
			for (unsigned j = 0; j < C; j++) {
				(*this)[i][j] = fn((*this)[i][j], m[i][j]);
			}
		}
		return *this;
	}
};

#endif