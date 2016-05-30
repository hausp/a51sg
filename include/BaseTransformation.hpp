/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef BASE_TRANSFORMATION_HPP
#define BASE_TRANSFORMATION_HPP

#include <list>

#include "BaseMatrix.hpp"

class BaseVector;

enum class Plane {
    X, Y, Z
};

class BaseTransformation {
 public:
    virtual void transform(const BaseVector*);
 protected:
    void push_front(const BaseMatrix&);
    void push_back(const BaseMatrix&);
 private:
    std::list<BaseMatrix> transformations;
};

inline void BaseTransformation::push_front(const BaseMatrix& m) {
    transformations.push_front(m);
}

inline void BaseTransformation::push_back(const BaseMatrix& m) {
    transformations.push_back(m);
}

#endif /* BASE_TRANSFORMATION_HPP */