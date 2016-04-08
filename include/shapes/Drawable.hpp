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
class Drawer;
template<unsigned D>
class Point;

enum class DrawableType {
    Point,
    Line,
    Polygon,
    Undefined
};

namespace {
    std::map<DrawableType, std::string> types = {
    {DrawableType::Point, "Point"},
    {DrawableType::Line, "Line"},
    {DrawableType::Polygon, "Polygon"},
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

    //Drawable() {}

    virtual ~Drawable() { }

    virtual void draw(Drawer<D>&) { }
    virtual void transform(const Matrix<D+1,D+1>&) = 0;
    virtual Point<D> center() const = 0;
    virtual std::vector<Point<D>> points() const = 0;

    const std::string& getName() const { return name; }
    const DrawableType getType() const { return type; }
    std::string getFormattedName() const {
        return types[type] + "(" + name + ")";
    }

    void setName(const std::string& name) { this->name = name; }
 
 protected:
    Drawable(const std::string& name, 
        DrawableType type = DrawableType::Undefined) 
        : name(name), type(type), color(std::make_tuple(0, 0, 0, 1)) { }
 private:
    std::string name;
    DrawableType type;
    color_t color;
};

#endif /* DRAWABLE_HPP */