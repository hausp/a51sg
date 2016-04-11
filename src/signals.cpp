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
    void set_receiver(Controller* c) {
        receiver = c;
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

    void remove_object(const long index) {
        receiver->removeObject(index);
    }

    void translate_object(const long index) {
        receiver->translateObject(index);
    }

    void finish_translation() {
        receiver->finishTranslation();
    }

    void scale_object(const long index) {
        receiver->scaleObject(index);
    }

    void finish_scaling() {
        receiver->finishScaling();
    }

    void rotate_object(const long index) {
        receiver->rotateObject(index);
    }

    void finish_rotation() {
        receiver->finishRotation();
    }

    void rotate_window(long direction) {
        receiver->rotateWindow(direction);
    }

    void update_entries() {
        receiver->updateEntries();
    }

    bool object_click(GtkWidget* const widget, GdkEventButton* const event) {
        return receiver->objectClick(widget, event);
    }

    void object_options(GtkWidget* const widget, GdkEventButton* const event) {
        receiver->objectOptions(widget, event);
    }

    void row_selected(GtkListBox* const list, GtkListBoxRow* const row) {
        receiver->rowSelected(list, row);
    }

// -------------------------- Widget event signals --------------------------- //
    void open_file_dialog() {
        receiver->openFileDialog();
    }

    void save_file_dialog() {
        receiver->saveFileDialog();
    }

    void open_file(const std::string& filename) {
        receiver->openFile(filename);
    }

    void save_file(const std::string& filename) {
        receiver->saveFile(filename);
    }

    void clear_objects() {
        receiver->clearObjects();
    }

    bool configure_event(GtkWidget* const widget,
                         GdkEventConfigure* const event) {
        return receiver->configure_event(widget, event);
    }

    void draw(GtkWidget* const widget, cairo_t* const cr) {
        receiver->draw(widget, cr);
    }

    void close() {
        receiver->quit();
    }
}
