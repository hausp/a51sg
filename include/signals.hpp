/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <gtk/gtk.h>
#include <string>

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
    void set_receiver(Controller*);
    
    // Shapes stuff signals
    void point_pressed();
    void line_pressed();
    void polygon_pressed();
    void point_ok();
    void line_ok();
    void polygon_ok();
    void vertex_ok();

    void remove_object(const long);
    void translate_object(const long);
    void finish_translation();
    void scale_object(const long);
    void finish_scaling();
    void rotate_object(const long);
    void finish_rotation();
    void rotate_window(long);
    void update_entries();
    bool object_click(GtkWidget* const, GdkEventButton* const);
    void object_options(GtkWidget* const, GdkEventButton* const);
    void row_selected(GtkListBox* const, GtkListBoxRow* const);

    // Widget event signals
    void open_file_dialog();
    void save_file_dialog();
    void open_file(const std::string&);
    void save_file(const std::string&);
    void clear_objects();
    bool configure_event(GtkWidget* const, GdkEventConfigure* const);
    void draw(GtkWidget* const, cairo_t* const);
    void close();
}

#endif /* SIGNAL_HPP */