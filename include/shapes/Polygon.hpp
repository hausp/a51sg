/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include "Point.hpp"
#include "Drawer.hpp"

template<unsigned D>
class Polygon : public Drawable<D> {
 public:

    Polygon() : Drawable<D>("", DrawableType::Polygon) { }

    Polygon(const Polygon& polygon) : Drawable<D>("", DrawableType::Polygon) {
        for (auto l : polygon) {
            lines.push_back(l);
        }
    }

    template<typename ...Args>
    Polygon(const Point<D>& p1, const Point<D>& p2, const Args&... args)
    : Drawable<D>("", DrawableType::Polygon) {
        init(p1, p2, args...);
    }

    template<typename ...Args>
    Polygon(const std::string& name, const Point<D>& p1, 
            const Point<D>& p2, const Args&... args)
    : Drawable<D>(name, DrawableType::Polygon) {
        init(p1, p2, args...);
    }

    Polygon(const std::vector<Point<D>>& points)
    : Drawable<D>("", DrawableType::Polygon) {
        for (unsigned i = 0; i < points.size()-1; i++) {
            lines.push_back(Line<D>(points[i], points[i+1]));
        }
        lines.push_back(Line<D>(points.back(), points.front()));
    }

    void draw(Drawer<D>& drawer) override {
        drawer.draw(*this);
    }

    void transform(const Matrix<D+1,D+1>& matrix) override {
        for (auto& line : lines) {
            line.transform(matrix);
        }
    }

    Point<D> center() const override {
        Point<D> r;
        for (auto& line : lines) {
            r += line[0];
        }
        return r / numberOfPoints();
    }

    std::vector<Point<D>> points() const override {
        std::vector<Point<D>> list;
        for (auto& line : lines) {
            list.push_back(line[0]);
        }
        return list;
    }

    const size_t numberOfPoints() const {
        return lines.size();
    }

    Point<D>& operator[](size_t index) {
        return lines[index][0];
    }

    const Point<D>& operator[](size_t index) const {
        return lines[index][0];
    }

    typename std::vector<Line<D>>::iterator begin() {
        return lines.begin();
    }

    typename std::vector<Line<D>>::const_iterator begin() const {
        return lines.cbegin();
    }

    typename std::vector<Line<D>>::iterator end() {
        return lines.end();
    }

    typename std::vector<Line<D>>::const_iterator end() const {
        return lines.cend();
    }

 private:
    std::vector<Line<D>> lines;

    template<typename ...Args>
    void init(const Point<D>& p0, const Point<D>& p1, const Args&... args) {
        lines.push_back(Line<D>(p0, p1));
        init(p1, args...);
    }
    void init(const Point<D>& p) {
        lines.push_back(Line<D>(p, lines[0][0]));
    }
};

#endif /* POLYGON_HPP */