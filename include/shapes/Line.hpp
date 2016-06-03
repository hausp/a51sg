/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef LINE_HPP
#define LINE_HPP

#include <vector>
#include "Drawable.hpp"
#include "Point.hpp"

class BaseVector;
class BaseMatrix;
class BaseDrawer;
class Window;

template<unsigned D>
class Line : public Drawable {
 public:
    Line();
    Line(const std::string&, const Point<D>&, const Point<D>&);
    Line(const Point<D>&, const Point<D>&);

    template<unsigned Dn>
    Line(const Line<Dn>&);

    void draw(BaseDrawer&) override;
    void clip(Window&) override;
    void transform(const BaseMatrix&) override;
    BaseVector center() const override;
    std::vector<Point<D>> points() const;
    void update(const BaseMatrix&, const Window&) override;
    Point<D>& operator[](size_t);
    const Point<D>& operator[](size_t) const;
    bool operator==(const Line<D>&) const;
    bool operator!=(const Line<D>&) const;
    typename std::vector<Point<D>>::iterator begin();
    typename std::vector<Point<D>>::const_iterator begin() const;
    typename std::vector<Point<D>>::iterator end();
    typename std::vector<Point<D>>::const_iterator end() const;

 private:
    std::vector<Point<D>> pointList;
};

template<unsigned D>
std::ostream& operator<<(std::ostream&, const Line<D>&);

#include "Line.ipp"

#endif /* LINE_HPP */