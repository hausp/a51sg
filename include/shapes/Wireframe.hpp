/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef WIREFRAME_HPP
#define WIREFRAME_HPP

#include <vector>
#include "Drawable.hpp"
#include "Line.hpp"
#include "Point.hpp"

class BaseVector;
class BaseMatrix;
class Window;

template<unsigned D>
class Wireframe : public Drawable {
 public:
    Wireframe();

    template<typename ...Args>
    Wireframe(const Point<D>&, const Point<D>&, const Args&...);

    template<typename ...Args>
    Wireframe(const std::string&, const Point<D>&,
            const Point<D>&, const Args&...);

    Wireframe(const std::vector<Line<D>>&);

    template<unsigned Dn>
    Wireframe(const Wireframe<Dn>&);

    void draw(BaseDrawer&) override;
    void clip(Window&) override;
    void transform(const BaseMatrix&) override;
    BaseVector center() const override;
    std::vector<Point<D>> points() const;
    void update(const BaseMatrix&, const Window&) override;
    std::vector<Line<D>>& edges();
    size_t numberOfPoints() const;
    Line<D>& operator[](size_t);
    const Line<D>& operator[](size_t) const;
    typename std::vector<Line<D>>::iterator begin();
    typename std::vector<Line<D>>::const_iterator begin() const;
    typename std::vector<Line<D>>::iterator end();
    typename std::vector<Line<D>>::const_iterator end() const;

 private:
    std::vector<Line<D>> edgeList;

    template<typename ...Args>
    void init(const Point<D>&, const Point<D>&, const Args&...);
    void init(const Point<D>&) {}
};

#include "Wireframe.ipp"

#endif /* WIREFRAME_HPP */