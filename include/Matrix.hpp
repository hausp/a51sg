/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <functional>
#include <ostream>
#include <vector>

template<unsigned N = 3, unsigned M = 3>
class Matrix {
public:
    Matrix() {
        for (unsigned i = 0; i < N; i++) {
            std::vector<double> container(M);
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

    Matrix<M,N> transpose() const {
        Matrix<M,N> result;
        for (unsigned i = 0; i < N; i++) {
            for (unsigned j = 0; j < M; j++) {
                result[j][i] = (*this)[i][j];
            }
        }
        return result;
    }

    std::vector<double>& operator[](unsigned index) {
        return data.at(index);
    }

    const std::vector<double>& operator[](unsigned index) const {
        return data.at(index);
    }

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

    Matrix<N,M> operator+(const Matrix<N,M>& m) const {
        Matrix<N,M> r = *this;
        return r += m;
    }

    Matrix<N,M> operator-(const Matrix<N,M>& m) const {
        Matrix<N,M> r = *this;
        return r -= m;
    }

    Matrix<N,M> operator*(double value) const {
        Matrix<N,M> r;
        for (unsigned i = 0; i < N; i++) {
            for (unsigned j = 0; j < M; j++) {
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

    Matrix<N,M> process(const Matrix<N,M>& m, 
                        const std::function<double(double, double)>& fn) {
        for (unsigned i = 0; i < N; i++) {
            for (unsigned j = 0; j < M; j++) {
                (*this)[i][j] = fn((*this)[i][j], m[i][j]);
            }
        }
        return *this;
    }
};

template<unsigned R, unsigned C>
std::ostream& operator<<(std::ostream& stream, const Matrix<R,C>& matrix) {
    for (unsigned i = 0; i < R; i++) {
        for (unsigned j = 0; j < C; j++) {
            stream << "m[" << i << "][" << j << "] = " << matrix[i][j] << std::endl;
        }
    }
    return stream;
}

#endif
