/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

#include <list>

#include "Matrix.hpp"

enum class Plane {
    X, Y, Z
};

template<size_t N>
class Transformation {
 public:
    Transformation();
    Transformation(std::initializer_list<Matrix<N,N>>);

    void push_front(const Matrix<N, N>&);
    void push_back(const Matrix<N, N>&);
    void emplace_back_translation(const std::array<double, N>&);
    void emplace_back_scaling(const std::array<double, N>&);
    void emplace_back_rotation(double, Plane = Plane::Z);
    void emplace_front_translation(const std::array<double, N>&);
    void emplace_front_scaling(const std::array<double, N>& ds);
    void emplace_front_rotation(double, Plane = Plane::Z);

 private:
    std::list<Matrix<N, N>> transformations;
    Matrix<N, N> translationMatrix(const std::array<double, N>&);
    Matrix<N, N> scalingMatrix(const std::array<double, N>& ds);
    Matrix<N, N> rotationMatrix(double, Plane = Plane::Z);
};

#include "Transformation.ipp"

#endif /* TRANSFORMATION_HPP */