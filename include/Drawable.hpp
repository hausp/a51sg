/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <map>
#include <string>
#include <tuple>
#include "utils.hpp"

class BaseDrawer;
class BaseVector;
class Window;

enum class DrawableType {
    Point,
    Line,
    Polygon,
    Curve,
    Wireframe,
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

class Drawable {
 public:
    virtual ~Drawable() { }

    virtual void draw(BaseDrawer&) = 0;
    virtual void clip(Window&) = 0;
    virtual void transform(const BaseMatrix&) = 0;
    virtual void update(const BaseMatrix&, const Window&) = 0;
    virtual BaseVector center() const = 0;

    std::string getFormattedName() const {
        return types[type] + "(" + name + ")";
    }
    const std::string& getName() const { return name; }
    const DrawableType getType() const { return type; }
    const color_t& getColor() const { return color; }
    bool isVisible() const { return visible; }
    void setVisible(bool visibility) { visible = visibility; }
    void setName(const std::string& name) { this->name = name; }
    void setColor(const color_t& _color) { color = _color; }
    void setColor(unsigned r, unsigned g, unsigned b, unsigned a = 1) {
        color = std::make_tuple(r, g, b, a);
    }
 
 protected:
    Drawable(DrawableType type = DrawableType::Undefined)
    : name{""}, type{type}, color{std::make_tuple(0, 0, 0, 1)} { }

    Drawable(const std::string& name, DrawableType type = DrawableType::Undefined)
    : name{name}, type{type}, color{std::make_tuple(0, 0, 0, 1)} { }
 
 private:
    std::string name;
    DrawableType type;
    color_t color;
    bool visible = true;
};

#endif /* DRAWABLE_HPP */