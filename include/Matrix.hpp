/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <functional>
#include <vector>
#include "BaseMatrix.hpp"

template<unsigned N = 3, unsigned M = 3>
class Matrix : public BaseMatrix {
public:
    Matrix() : BaseMatrix(N,M) { }

    Matrix(const std::initializer_list<std::initializer_list<double>>& matrix)
    : BaseMatrix(matrix) { }

	Matrix<N,M>& operator+=(const Matrix<N,M>& m) {
		return process(m, std::plus<double>());
	}

	Matrix<N,M>& operator-=(const Matrix<N,M>& m) {
		return process(m, std::minus<double>());
	}

	Matrix<N,M>& operator*=(const Matrix<M,M>& m) {
		*this = *this * m;
		return *this;
	}

    Matrix<N,M> operator+(const Matrix<N,M>& m) const {
        Matrix<N,M> r = *this;
        return r += m;
    }

    Matrix<N,M> operator-(const Matrix<N,M>& m) const {
        Matrix<N,M> r = *this;
        return r -= m;
    }

	template<unsigned P>
	Matrix<N,P> operator*(const Matrix<M,P>& m) const {
		Matrix<N,P> r;
		for (unsigned i = 0; i < N; i++) {
			for (unsigned j = 0; j < P; j++) {
				r[i][j] = 0;
				for (unsigned k = 0; k < M; k++) {
					r[i][j] += (*this)[i][k] * m[k][j];
				}
			}
		}
		return r;
	}

private:
	Matrix<N,M> process(const Matrix<N,M>& m, 
                        const std::function<double(double, double)>& fn) {
		for (unsigned i = 0; i < N; i++) {
			for (unsigned j = 0; j < M; j++) {
				(*this)[i][j] = fn((*this)[i][j], m[i][j]);
			}
		}
		return *this;
	}

    using BaseMatrix::operator+=;
    using BaseMatrix::operator-=;
    using BaseMatrix::operator*=;
    using BaseMatrix::operator*;
    using BaseMatrix::operator+;
    using BaseMatrix::operator-;
};

#endif
