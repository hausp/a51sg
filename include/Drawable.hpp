/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <map>
#include <string>
#include <tuple>
#include "Matrix.hpp"
#include "utils.hpp"

template<unsigned D>
class BaseDrawer;
template<unsigned D>
class Point;
class Window;

enum class DrawableType {
    Point,
    Line,
    Polygon,
    Curve,
    Wireframe,
    BicubicSurface,
    Undefined
};

namespace {
    std::map<DrawableType, std::string> types = {
    {DrawableType::Point, "Point"},
    {DrawableType::Line, "Line"},
    {DrawableType::Polygon, "Polygon"},
    {DrawableType::Curve, "Curve"},
    {DrawableType::Wireframe, "Wireframe"},
    {DrawableType::Undefined, "Undefined"}};
}


template<unsigned D>
class Drawable {
 public:
    void setColor(const color_t& _color) {
        color = _color;
    }

    void setColor(unsigned r, unsigned g, unsigned b, unsigned a = 1) {
        color = std::make_tuple(r, g, b, a);
    }

    const color_t& getColor() const {
        return color;
    }

    virtual ~Drawable() { }

    virtual void draw(BaseDrawer<D>&) = 0;
    virtual void clip(Window&) = 0;
    virtual void transform(const Matrix<D+1,D+1>&) = 0;
    virtual Point<D> center() const = 0;
    virtual std::vector<Point<D>> points() const = 0;
    virtual void update(const Matrix<3,3>&, const Window&) { throw 666; }

    void setName(const std::string& name) { this->name = name; }
    const std::string& getName() const { return name; }
    const DrawableType getType() const { return type; }
    std::string getFormattedName() const {
        return types[type] + "(" + name + ")";
    }

    void setVisible(bool visibility) { visible = visibility; }
    bool isVisible() const { return visible; }
 
 protected:
    Drawable(const std::string& name, 
        DrawableType type = DrawableType::Undefined) 
        : name(name), type(type), color(std::make_tuple(0, 0, 0, 1)) { }
 private:
    std::string name;
    DrawableType type;
    color_t color;
    bool visible = true;
};

#endif /* DRAWABLE_HPP */