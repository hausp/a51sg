/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include <cmath>
#include "Transformation.hpp"

template<size_t N>
Transformation<N>::Transformation() {

}

template<size_t N>
Transformation<N>::Transformation(std::initializer_list<Matrix<N,N>> init)
: transformations(init) { }

template<size_t N>
void Transformation<N>::push_front(const Matrix<N, N>& m) {
    transformations.push_front(m);
}

template<size_t N>
void Transformation<N>::push_back(const Matrix<N, N>& m) {
    transformations.push_back(m);
}

template<size_t N>
void Transformation<N>::emplace_front_translation(const std::array<double, N>& ds) {
    transformations.push_front(translationMatrix(ds));
}

template<size_t N>
void Transformation<N>::emplace_front_scaling(const std::array<double, N>& ds) {
    transformations.push_front(scalingMatrix(ds));
}

template<size_t N>
void Transformation<N>::emplace_front_rotation(double angle, Plane p) {
    transformations.push_front(rotationMatrix(angle, p));
}

template<size_t N>
void Transformation<N>::emplace_back_translation(const std::array<double, N>& ds) {
    transformations.push_back(translationMatrix(ds));
}

template<size_t N>
void Transformation<N>::emplace_back_scaling(const std::array<double, N>& ds) {
    transformations.push_back(scalingMatrix(ds));
}

template<size_t N>
void Transformation<N>::emplace_back_rotation(double angle, Plane p) {
    transformations.push_back(rotationMatrix(angle, p));
}

template<size_t N>
Matrix<N, N> Transformation<N>::translationMatrix(const std::array<double, N>& ds) {
    Matrix<N, N> matrix;
    for (unsigned i = 0; i < N; i++) {
        matrix[i][i] = 1;
        if (i < N-1) {
            matrix[N][i] = ds[i];
        }
    }
    return matrix;
}

template<size_t N>
Matrix<N, N> Transformation<N>::scalingMatrix(const std::array<double, N>& ds) {
    Matrix<N, N> matrix;
    for (unsigned i = 0; i < N-1; i++) {
        matrix[i][i] = ds[i];
    }
    matrix[N][N] = 1;
    return matrix;
}

template<size_t N>
Matrix<N, N> Transformation<N>::rotationMatrix(double angle, Plane plane) {
    double a = angle * M_PI / 180;
    double s = sin(a);
    double c = cos(a);
    auto planeIndex = static_cast<int>(plane);
    Matrix<N, N> matrix;
    for (unsigned i = 0; i < N; i++) {
        matrix[i][i] = 1;
    }
    auto x = (planeIndex + 1) % N;
    auto y = (planeIndex - 1) % N;
    matrix[x][x] = c;
    matrix[y][y] = c;
    matrix[x][y] = -s;
    matrix[y][x] = s;
    return matrix;
}