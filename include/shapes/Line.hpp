/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef LINE_HPP
#define LINE_HPP

#include <vector>
#include "Drawable.hpp"
#include "Point.hpp"
#include "Drawer.hpp"

template<unsigned D>
class Line : public Drawable<D> {
 public:

    Line() : Drawable<D>("", DrawableType::Line) { }

    Line(const Line& line) : Drawable<D>("", DrawableType::Line) {
        for (auto p : line) {
            points.push_back(p);
        }
    }

    Line(const std::string& name, const Point<D>& p1, const Point<D>& p2)
    : Drawable<D>(name, DrawableType::Line) {
        points.push_back(p1);
        points.push_back(p2);
    }

    Line(const Point<D>& p1, const Point<D>& p2) 
    : Drawable<D>("", DrawableType::Line) {
        points.push_back(p1);
        points.push_back(p2);
    }

    ~Line() {}

    void draw(Drawer<D>& drawer) override {
        drawer.draw(*this);
    }

    void transform(const Matrix<D+1,D+1>& matrix) override {
        (*this)[0] *= matrix;
        (*this)[1] *= matrix;
    }

    Point<D> center() const override {
        return (points[0] + points[1]) / 2;
    }

    Point<D>& operator[](size_t index) {
        return points[index];
    }

    const Point<D>& operator[](size_t index) const {
        return points[index];
    }

    typename std::vector<Point<D>>::iterator begin() { 
        return points.begin();
    }

    typename std::vector<Point<D>>::const_iterator begin() const {
        return points.cbegin();
    }

    typename std::vector<Point<D>>::iterator end() {
        return points.end();
    }

    typename std::vector<Point<D>>::const_iterator end() const {
        return points.cend();
    }

 private:
    std::vector<Point<D>> points;
};

#endif /* LINE_HPP */