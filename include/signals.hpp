/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <gtk/gtk.h>

class Controller;

namespace signals {

    // Navigation signals
    void down();
    void up();
    void left();
    void right();
    void set_zoom();
    void zoom_in();
    void zoom_out();

    // Set signals receiver
    void set_receiver(Controller&);
    
    // Shapes stuff signals
    void point_pressed();
    void line_pressed();
    void polygon_pressed();
    void point_ok();
    void line_ok();
    void polygon_ok();
    void vertex_ok();

    void remove_object(gpointer);
    void translate_object(gpointer);
    void finish_translation();
    void scale_object(gpointer);
    void finish_scaling();
    void rotate_object(gpointer);
    void finish_rotation();
    void update_entries();
    bool object_click(GtkWidget*, GdkEventButton*);
    void object_options(GtkWidget*, GdkEventButton*);
    void row_selected(GtkListBox*, GtkListBoxRow*);

    // Widget event signals
    bool configure_event(GtkWidget*, GdkEventConfigure*, gpointer);
    void draw(GtkWidget*, cairo_t*, gpointer);
    void close();
}

#endif /* SIGNAL_HPP */