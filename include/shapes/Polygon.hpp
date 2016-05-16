/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include "Drawable.hpp"
#include "Point.hpp"

class Window;

template<unsigned D>
class Polygon : public Drawable<D> {
 public:
    Polygon();
    template<typename ...Args>
    Polygon(const Point<D>&, const Point<D>&, const Args&...);
    template<typename ...Args>
    Polygon(const std::string&, const Point<D>&,
            const Point<D>&, const Args&...);
    Polygon(const std::vector<Point<D>>&);

    void draw(BaseDrawer<D>&) override;
    void clip(Window&) override;
    void transform(const Matrix<D+1,D+1>&) override;
    Point<D> center() const override;
    std::vector<Point<D>> points() const override;
    void update(const Matrix<D+1,D+1>&) override;
    const size_t numberOfPoints() const;
    Point<D>& operator[](size_t);
    const Point<D>& operator[](size_t) const;
    typename std::vector<Point<D>>::iterator begin();
    typename std::vector<Point<D>>::const_iterator begin() const;
    typename std::vector<Point<D>>::iterator end();
    typename std::vector<Point<D>>::const_iterator end() const;
    std::vector<Point<D>>& ndc();
    bool isFilled() { return filled; }
    void setFilled(bool f) { filled = f; }

 private:
    std::vector<Point<D>> vertices;
    std::vector<Point<D>> ndcVertices;
    bool filled = false;

    template<typename ...Args>
    void init(const Point<D>&, const Point<D>&, const Args&...);
    void init(const Point<D>&);
};

#include "Polygon.ipp"

#endif /* POLYGON_HPP */