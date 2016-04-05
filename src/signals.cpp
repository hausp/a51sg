/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "signals.hpp"

#include <iostream>

#include "gtk.hpp"
#include "Controller.hpp"


namespace {
    Controller* receiver = NULL;
}

namespace signals {

// -------------------------- Window Navigation --------------------------- //
    void down() {
        receiver->moveVertical(-1);
    }

    void up() {
        receiver->moveVertical(1);
    }

    void left() {
        receiver->moveHorizontal(-1);
    }

    void right() {
        receiver->moveHorizontal(1);
    }

    void set_zoom() {
        receiver->setZoom();
    }

    void zoom_in() {
        receiver->zoom(1);
    }

    void zoom_out() {
        receiver->zoom(-1);
    }

// -------------------------- Set signal receiver -------------------------- //
    void set_receiver(Controller& c) {
        receiver = &c;
    }

// ------------------------- Shapes stuff signals -------------------------- //
    void point_pressed() {
        receiver->pointPressed();
    }

    void line_pressed() {
        receiver->linePressed();
    }

    void polygon_pressed() {
        receiver->polygonPressed();
    }

    void point_ok() {
        receiver->createPoint();
    }

    void line_ok() {
        receiver->createLine();
    }

    void polygon_ok() {
        receiver->createPolygon();
    }

    void vertex_ok() {
        receiver->vertexOk();
    }

    void remove_object(gpointer index) {
        long i = (long)index;
        receiver->removeObject(i);
    }

    void translate_object(gpointer index) {
        long i = (long)index;
        receiver->translateObject(i);
    }

    void finish_translation() {
        receiver->finishTranslation();
    }

    void scale_object(gpointer index) {
        long i = (long)index;
        receiver->scaleObject(i);
    }

    void finish_scaling() {
        receiver->finishScaling();
    }

    void rotate_object(gpointer index) {
        long i = (long)index;
        receiver->rotateObject(i);
    }

    void finish_rotation() {
        receiver->finishRotation();
    }

    void update_entries() {
        receiver->updateEntries();
    }

    bool object_click(GtkWidget* widget, GdkEventButton* e) {
        return receiver->objectClick(widget, e);
    }

    void object_options(GtkWidget* widget, GdkEventButton* event) {
        receiver->objectOptions(widget, event);
    }

    void row_selected(GtkListBox* list, GtkListBoxRow* row) {
        receiver->rowSelected(list, row);
    }

// -------------------------- Widget event signals --------------------------- //
    bool configure_event(GtkWidget* widget,
                         GdkEventConfigure* event,
                         gpointer data) {
        return receiver->configure_event(widget, event, data);
    }

    void draw(GtkWidget* widget, cairo_t* cr, gpointer data) {
        receiver->draw(widget, cr, data);
    }

    void close() {
        receiver->quit();
    }
}
