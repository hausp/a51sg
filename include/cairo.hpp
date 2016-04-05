/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef CAIRO_HPP
#define CAIRO_HPP

#include <gtk/gtk.h>
#include <tuple>
#include "types.hpp"

namespace cairo {
    void clear();
    void close_path();
    void destroy();
    bool draw(cairo_t*);
    void fill();
    void line_to(const double, const double);
    void move_to(const double, const double);
    void point(const double, const double);
    void rectangle(const double, const double, const double, const double);
    void set_color(double, double, double, double = 1);
    void set_color(const color_t&);
    void stroke();
    bool update(GtkWidget*);
}

#endif /* CAIRO_HPP */