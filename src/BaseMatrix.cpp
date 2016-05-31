/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "BaseMatrix.hpp"
#include <cassert>

//------------------------------- BaseMatrix --------------------------------//

BaseMatrix::BaseMatrix() : n_rows{3}, n_columns{3} {
    matrix.resize(9, 0);
}

BaseMatrix::BaseMatrix(size_t n_rows, size_t n_columns)
: n_rows{n_rows}, n_columns{n_columns} {
    matrix.resize(n_rows * n_columns, 0);
}

BaseMatrix::BaseMatrix(std::initializer_list<std::initializer_list<double>> l) {
    n_rows = l.size();
    n_columns = (*l.begin()).size();
    for (auto row : l) {
        for (auto value : row) {
            matrix.push_back(value);
        }
    }
}

BaseMatrix::row BaseMatrix::operator[](size_t index) {
    return row{n_columns, matrix.begin() + (n_columns * index)};
}

BaseMatrix::const_row BaseMatrix::operator[](size_t index) const {
    return const_row{n_columns, matrix.begin() + (n_columns * index)};
}

BaseMatrix::row BaseMatrix::begin() {
    return row{n_columns, matrix.begin()};
}

BaseMatrix::row BaseMatrix::end() {
    return row{n_columns, matrix.end()};
}

BaseMatrix::const_row BaseMatrix::begin() const {
    return const_row{n_columns, matrix.begin()};
}

BaseMatrix::const_row BaseMatrix::end() const {
    return const_row{n_columns, matrix.end()};
}

BaseMatrix& BaseMatrix::operator+=(const BaseMatrix& rhs) {
    assert(this->n_rows == rhs.n_rows);
    assert(this->n_columns == rhs.n_columns);
    return process(rhs, std::plus<double>());
}

BaseMatrix& BaseMatrix::operator-=(const BaseMatrix& rhs) {
    assert(this->n_rows == rhs.n_rows);
    assert(this->n_columns == rhs.n_columns);
    return process(rhs, std::minus<double>());
}

BaseMatrix& BaseMatrix::operator*=(const BaseMatrix& rhs) {
    assert(this->n_rows == rhs.n_rows);
    assert(this->n_columns == rhs.n_columns);
    for (size_t i = 0; i < this->n_rows; i++) {
        for (size_t j = 0; j < rhs.n_columns; j++) {
            (*this)[i][j] = 0;
            for (size_t k = 0; k < this->n_columns; k++) {
                (*this)[i][j] += (*this)[i][k] * rhs[k][j];
            }
        }
    }
    return (*this);
}

BaseMatrix BaseMatrix::operator+(const BaseMatrix& rhs) const {
    assert(this->n_rows == rhs.n_rows);
    assert(this->n_columns == rhs.n_columns);
    BaseMatrix temp = *this;
    return temp += rhs;
}

BaseMatrix BaseMatrix::operator-(const BaseMatrix& rhs) const {
    assert(this->n_rows == rhs.n_rows);
    assert(this->n_columns == rhs.n_columns);
    BaseMatrix temp = *this;
    return temp -= rhs;
}

BaseMatrix BaseMatrix::operator*(const BaseMatrix& rhs) const {
    assert(this->n_columns == rhs.n_rows);
    BaseMatrix temp(this->n_rows, rhs.n_columns);
    for (size_t i = 0; i < this->n_rows; i++) {
        for (size_t j = 0; j < rhs.n_columns; j++) {
            temp[i][j] = 0;
            for (size_t k = 0; k < this->n_columns; k++) {
                temp[i][j] += (*this)[i][k] * rhs[k][j];
            }
        }
    }
    return temp;
}

BaseMatrix& BaseMatrix::process(const BaseMatrix& m,
            const std::function<double(double,double)>& fn) {
    for (size_t i = 0; i < n_rows; i++) {
        for (size_t j = 0; j < n_columns; j++) {
            (*this)[i][j] = fn((*this)[i][j], m[i][j]);
        }
    }
    return *this;
}

std::ostream& operator<<(std::ostream& out, const BaseMatrix& matrix) {
    out << "[";
    for (size_t i = 0; i < matrix.rowSize(); i++) {
        if (i != 0) out << " ";
        for (size_t j = 0; j < matrix.columnSize(); j++) {
            if (j > 0) out << ", ";
            out << matrix[i][j];
        }
        if (i != matrix.rowSize() - 1) out << std::endl;
    }
    out << "]" << std::endl;
    return out;
}

//---------------------------------- row -----------------------------------//

BaseMatrix::row::row(size_t s, vector_iterator v)
: size{s}, iterator{v} { }

BaseMatrix::row& BaseMatrix::row::operator++() {
    iterator += size;
    return *this;
}

BaseMatrix::row& BaseMatrix::row::operator--() {
    iterator -= size;
    return *this;
}

BaseMatrix::row BaseMatrix::row::operator++(int) {
    BaseMatrix::row old = *this;
    (*this)++;
    return old;
}

BaseMatrix::row BaseMatrix::row::operator--(int) {
    BaseMatrix::row old = *this;
    (*this)--;
    return old;
}


BaseMatrix::row& BaseMatrix::row::operator=(double rhs) {
    *iterator = rhs;
    return *this;
}

BaseMatrix::row& BaseMatrix::row::operator=(std::initializer_list<double> rhs) {
    assert(rhs.size() == size);
    size_t i = 0;
    for (auto value : rhs) {
        *(iterator + i) = value;
        i++;
    }
    return *this;
}

double& BaseMatrix::row::operator[](size_t index) {
    return *(iterator + index);
}

vector_iterator BaseMatrix::row::begin() {
    return iterator;
}

vector_iterator BaseMatrix::row::end() {
    return iterator + size;
}

bool BaseMatrix::row::operator==(const row& rhs) {
    return this->iterator == rhs.iterator;
}

bool BaseMatrix::row::operator!=(const row& rhs) {
    return !(*this == rhs);
}


//------------------------------- const_row --------------------------------//

BaseMatrix::const_row::const_row(size_t s, const_vector_iterator v)
: size{s}, iterator{v} { }

BaseMatrix::const_row& BaseMatrix::const_row::operator++() {
    iterator += size;
    return *this;
}

BaseMatrix::const_row& BaseMatrix::const_row::operator--() {
    iterator -= size;
    return *this;
}

BaseMatrix::const_row BaseMatrix::const_row::operator++(int) {
    BaseMatrix::const_row old = *this;
    (*this)++;
    return old;
}

BaseMatrix::const_row BaseMatrix::const_row::operator--(int) {
    BaseMatrix::const_row old = *this;
    (*this)--;
    return old;
}

double BaseMatrix::const_row::operator[](size_t index) const {
    return *(iterator + index);
}

const_vector_iterator BaseMatrix::const_row::begin() const {
    return iterator;
}

const_vector_iterator BaseMatrix::const_row::end() const {
    return iterator + size;
}

bool BaseMatrix::const_row::operator==(const const_row& rhs) {
    return this->iterator == rhs.iterator;
}

bool BaseMatrix::const_row::operator!=(const const_row& rhs) {
    return !(*this == rhs);
}