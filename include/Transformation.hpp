/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

#include "BaseTransformation.hpp"

template<unsigned N, unsigned M>
class Matrix;
class BaseVector;

template<unsigned N>
class Transformation : public BaseTransformation {
 public:
    Transformation();
    Transformation(std::initializer_list<Matrix<N,N>>);

    void push_front(const Matrix<N,N>&);
    void push_back(const Matrix<N,N>&);
    void emplace_back_translation(const std::array<double,N>&);
    void emplace_back_scaling(const std::array<double,N>&);
    void emplace_back_rotation(double, Plane = Plane::Z);
    void emplace_front_translation(const std::array<double,N>&);
    void emplace_front_scaling(const std::array<double,N>& ds);
    void emplace_front_rotation(double, Plane = Plane::Z);

    void transform(const BaseVector*);

 private:
    Matrix<N,N> translationMatrix(const std::array<double,N>&);
    Matrix<N,N> scalingMatrix(const std::array<double,N>& ds);
    Matrix<N,N> rotationMatrix(double, Plane = Plane::Z);
};

#include "Transformation.ipp"

#endif /* TRANSFORMATION_HPP */