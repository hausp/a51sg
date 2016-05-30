/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include "Drawable.hpp"
#include "Point.hpp"

class BaseVector;
class BaseTransformation;
class Window;

template<unsigned D>
class Polygon : public Drawable {
 public:
    Polygon();

    template<typename ...Args>
    Polygon(const Point<D>&, const Point<D>&, const Args&...);

    template<typename ...Args>
    Polygon(const std::string&, const Point<D>&,
            const Point<D>&, const Args&...);

    Polygon(const std::vector<Point<D>>&);

    template<unsigned Dn>
    Polygon(const Polygon<Dn>&);

    void draw(BaseDrawer&) override;
    void clip(Window&) override;
    void transform(const BaseTransformation&) override;
    BaseVector center() const override;
    std::vector<BaseVector> points() const override;
    void update(const BaseTransformation&, const Window&) override;
    size_t numberOfPoints() const;
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

template<unsigned D>
std::ostream& operator<<(std::ostream&, const Polygon<D>&);

//#include "Polygon.ipp"

#endif /* POLYGON_HPP */