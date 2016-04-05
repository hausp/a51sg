/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "Controller.hpp"

#include <iostream>
#include <regex>

#include "gtk.hpp"
#include "cairo.hpp"
#include "Interface.hpp"
#include "Drawer.hpp"
#include "Drawer2D.hpp"
#include "Point.hpp"
#include "Line.hpp"
#include "Polygon.hpp"

Controller::Controller(Interface& interface, Drawer2D& drawer) 
: interface(interface), drawer(drawer) { }

Controller::~Controller() { }

// -------------------------- Window Navigation --------------------------- //

void Controller::moveVertical(int direction) {
    drawer.moveVertical(direction);
    drawer.drawAll();
    interface.queueDraw();
}

void Controller::moveHorizontal(int direction) {
    drawer.moveHorizontal(direction);
    drawer.drawAll();
    interface.queueDraw();
}

void Controller::setZoom() {
    GtkWidget* zoomLevel = interface.getZoomLevel();
    std::string z = gtk_entry_get_text(GTK_ENTRY(zoomLevel));
    std::regex numeric("^\\d+");
    if (!std::regex_match(z, numeric)) return;
    drawer.setZoom(stoi(z)/100.0);
}

void Controller::zoom(int d) {
    drawer.zoom(d);
    drawer.drawAll();
    interface.queueDraw();
}

// ----------------------------- Shapes stuff ------------------------------ //

void Controller::pointPressed() {
    interface.buildPointWindow();
}

void Controller::linePressed() {
    interface.buildLineWindow();
}

void Controller::polygonPressed() {
    interface.buildVertexWindow();
}

void Controller::vertexOk() {
    interface.buildPolygonWindow();
}

void Controller::createPoint() {
    auto entries = interface.getEntries();
    std::string name = interface.getShapeName();
    std::regex numeric("^\\d+");

    for (auto entry : entries) {
        if (!std::regex_match(entry, numeric)) return;
    }
    if (name != "") {
        Point2D* p = new Point2D(name, stoi(entries[0]), stoi(entries[1]));
        drawer.addShape(p);
        interface.addShape(p->getFormattedName());
        interface.closeDialog();
        drawer.draw(*p);
        interface.queueDraw();
    }
}

void Controller::createLine() {
    auto entries = interface.getEntries();
    std::string name = interface.getShapeName();
    std::regex numeric("^\\d+");

    for (auto entry : entries) {
        if (!std::regex_match(entry, numeric)) return;
    }

    if (name != "") {
        Point2D p1(stoi(entries[0]), stoi(entries[1]));
        Point2D p2(stoi(entries[2]), stoi(entries[3]));
        Line2D* line = new Line2D(name, p1, p2);
        drawer.addShape(line);
        interface.addShape(line->getFormattedName());
        interface.closeDialog();
        drawer.draw(*line);
        interface.queueDraw();
    }
}

void Controller::createPolygon() {
    auto entries = interface.getEntries();
    std::string name = interface.getShapeName();
    std::regex numeric("^\\d+");

    for (auto entry : entries) {
        if (!std::regex_match(entry, numeric)) return;
    }

    if (name != "") {
        std::vector<Point<2>> polygonPoints;
        for (unsigned i = 0; i < entries.size() - 1; i += 2) {
            polygonPoints.push_back(Point2D(stoi(entries[i]), stoi(entries[i + 1])));
        }
        Polygon2D* polygon = new Polygon2D(polygonPoints);
        polygon->setName(name);
        drawer.addShape(polygon);
        interface.addShape(polygon->getFormattedName());
        interface.closeDialog();
        drawer.draw(*polygon);
        interface.queueDraw();
    }
}

void Controller::removeObject(long index) {
    interface.removeShape(index);
    drawer.removeShape(index);
    drawer.drawAll();
    interface.queueDraw();
}

void Controller::translateObject(long index) {
    currentIndex = index;
    interface.buildTranslationWindow();
}

void Controller::finishTranslation() {
    auto entries = interface.getEntries();
    std::regex numeric("^(\\+|-)?\\d+");

    for (auto entry : entries) {
        if (!std::regex_match(entry, numeric)) return;
    }

    drawer.translate(currentIndex, stod(entries[0]), stod(entries[1]));
    interface.closeDialog();
    drawer.drawAll();
    interface.queueDraw();
}

void Controller::scaleObject(long index) {
    currentIndex = index;
    interface.buildScalingWindow();
}

void Controller::finishScaling() {
    auto entries = interface.getEntries();
    std::regex numeric("^\\d+(\\.\\d+)?");

    for (auto entry : entries) {
        if (!std::regex_match(entry, numeric)) return;
    }

    drawer.scale(currentIndex, stod(entries[0]), stod(entries[1]));
    interface.closeDialog();
    drawer.drawAll();
    interface.queueDraw();
}

void Controller::rotateObject(long index) {
    currentIndex = index;
    interface.buildRotationWindow();
}

void Controller::finishRotation() {
    auto entries = interface.getEntries();
    std::regex numeric("^(\\+|-)?\\d+(\\.\\d+)?");

    int type = interface.getSelectedRadio();

    int i = entries.size();
    while (--i >= 0) {
        if (!std::regex_match(entries[i], numeric)) return;
        if (type != 2) break;
    }

    double angle = stod(entries.back());
    entries.pop_back();
    drawer.rotate(currentIndex, angle, type, entries);
    interface.closeDialog();
    drawer.drawAll();
    interface.queueDraw();
}

void Controller::updateEntries() {
    interface.updateEntries();
}

bool Controller::objectClick(GtkWidget* list, GdkEventButton* event) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
        objectOptions(list, event);
        return true;
    }
    return false;
}

void Controller::objectOptions(GtkWidget* list, GdkEventButton* event) {
    return interface.showObjectOptions(list, event);
}

void Controller::rowSelected(GtkListBox* list, GtkListBoxRow* row) {
    if (row) {
        long index = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(row));
        drawer.highlightObject(index);
        drawer.drawAll();
        interface.queueDraw();
    }
}

// ----------------------------- Widget events ----------------------------- //

bool Controller::configure_event(GtkWidget* widget,
                                 GdkEventConfigure* event,
                                 gpointer data) {
    drawer.resizeViewport(gtk_widget_get_allocated_width(widget),
                          gtk_widget_get_allocated_height(widget));
    return cairo::update(widget);
}

void Controller::draw(GtkWidget* widget, cairo_t* cr, gpointer data) {
    cairo::draw(cr);
}

void Controller::quit() {
    cairo::destroy();
    gtk::quit();
}
