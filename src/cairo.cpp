/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "cairo.hpp"

namespace {
    cairo_t* cr = NULL;
    cairo_surface_t*  surface = NULL;
}

namespace cairo {

    void clear() {
        cairo_set_source_rgb(cr, 1, 1, 1);
        cairo_paint(cr);
        cairo_set_source_rgb(cr, 0, 0, 0);
    }

    void close_path() {
        cairo_close_path(cr);
    }

    void destroy() {
        if (surface)
            cairo_surface_destroy(surface);

        if (cr)
            cairo_destroy(cr);
    }

    bool draw(cairo_t* _cr) {
        cairo_set_source_surface(_cr, surface, 0, 0);
        cairo_paint(_cr);
        return false;
    }

    void fill() {
        cairo_fill(cr);
    }

    void line_to(const double x, const double y) {
        cairo_line_to(cr, x, y);
    }

    void move_to(const double x, const double y) {
        cairo_move_to(cr, x, y);
    }

    void point(const double x, const double y) {
        cairo_set_line_width(cr, 2);
        cairo_rectangle(cr, x - 1, y - 1, 2, 2);
        cairo_fill(cr);
        //cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
        //cairo_move_to(cr, x, y);
        //cairo_line_to(cr, x, y);
        //cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    }

    void rectangle(const double x, const double y, 
                   const double l, const double h) {
        cairo_rectangle(cr, x, y, l, h);
    }

    void set_color(int r, int g, int b) {
        cairo_set_source_rgb(cr, r, g, b);
    }

    void stroke() {
        cairo_stroke(cr);
    }

    bool update(GtkWidget* widget) {
        if (surface)
            cairo_surface_destroy(surface);

        if (cr)
            cairo_destroy(cr);

        surface = gdk_window_create_similar_surface(
                    gtk_widget_get_window(widget),
                    CAIRO_CONTENT_COLOR,
                    gtk_widget_get_allocated_width(widget),
                    gtk_widget_get_allocated_height(widget));

        cr = cairo_create(surface);

        cairo::clear();

        return true;
    }
}