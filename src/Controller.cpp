/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "Controller.hpp"

#include <iostream>
#include "cairo.hpp"
#include "gtk.hpp"
#include "BezierCurve.hpp"
#include "Drawer.hpp"
#include "Drawer2D.hpp"
#include "FileManager.hpp"
#include "Interface.hpp"
#include "Point.hpp"
#include "Line.hpp"
#include "Polygon.hpp"

Controller::Controller(Interface& interface, Drawer2D& drawer) 
: interface(interface), drawer(drawer) {
    fileManager = std::shared_ptr<FileManager>(new FileManager());
}

Controller::~Controller() { }

// -------------------------- Window Navigation --------------------------- //

void Controller::moveVertical(const int direction) {
    drawer.moveVertical(direction);
    drawer.drawAll();
    interface.queueDraw();
}

void Controller::moveHorizontal(const int direction) {
    drawer.moveHorizontal(direction);
    drawer.drawAll();
    interface.queueDraw();
}

void Controller::setZoom() {
    GtkWidget* zoomLevel = interface.getZoomLevel();
    std::string z = gtk_entry_get_text(GTK_ENTRY(zoomLevel));
    // std::regex numeric("^\\d+");
    // if (!std::regex_match(z, numeric)) return;
    drawer.setZoom(stoi(z)/100.0);
}

void Controller::zoom(const int d) {
    drawer.zoom(d);
    drawer.drawAll();
    interface.queueDraw();
}

// ----------------------------- Shapes stuff ------------------------------ //

void Controller::pointPressed() {
    std::vector<Point<2>> points = {Point<2>(100, 100), Point<2>(200, 400), Point<2>(300, 400), Point<2>(400, 100)};
    auto curve = new BezierCurve<2>(0.05, points);
    curve->setName("c1");
    drawer.addShape(curve);
    interface.addShape(curve->getFormattedName());
    drawer.draw(*curve);
    interface.queueDraw();

    interface.buildPointWindow();
}

void Controller::linePressed() {
    interface.buildLineWindow();
}

void Controller::polygonPressed() {
    interface.buildVertexWindow();
}

void Controller::vertexOk(bool filled) {
    filledPolygon = filled;
    interface.buildPolygonWindow();
}

void Controller::createPoint() {
    auto entries = interface.getEntries();
    std::string name = interface.getShapeName();

    for (auto entry : entries) {
        if (!utils::regex_match(entry, utils::REGEX_INTEGER)) return;
    }

    if (name != "") {
        Point2D* p;
        #if !RECENT_COMPILER
        try {
        #endif
            p = new Point2D(name, stoi(entries[0]), stoi(entries[1]));
        #if !RECENT_COMPILER
        } catch(...) {
            return;
        }
        #endif
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

    for (auto entry : entries) {
        if (!utils::regex_match(entry, utils::REGEX_INTEGER)) return;
    }

    if (name != "") {
        Point2D p1, p2;
        #if !RECENT_COMPILER
        try {
        #endif
            p1 = Point2D(stoi(entries[0]), stoi(entries[1]));
            p2 = Point2D(stoi(entries[2]), stoi(entries[3]));
        #if !RECENT_COMPILER
        } catch(...) {
            return;
        }
        #endif
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

    for (auto entry : entries) {
        if (!utils::regex_match(entry, utils::REGEX_INTEGER)) return;
    }

    if (name != "") {
        std::vector<Point<2>> polygonPoints;
        #if !RECENT_COMPILER
        try {
        #endif
        for (unsigned i = 0; i < entries.size() - 1; i += 2) {
            polygonPoints.push_back(Point2D(stoi(entries[i]), stoi(entries[i + 1])));
        }
        #if !RECENT_COMPILER
        } catch(...) {
            return;
        }
        #endif
        Polygon2D* polygon = new Polygon2D(polygonPoints);
        polygon->setName(name);
        polygon->setFilled(filledPolygon);
        drawer.addShape(polygon);
        interface.addShape(polygon->getFormattedName());
        interface.closeDialog();
        drawer.draw(*polygon);
        interface.queueDraw();
    }
}

void Controller::removeObject(const long index) {
    interface.removeShape(index);
    drawer.removeShape(index);
    drawer.drawAll();
    interface.queueDraw();
}

void Controller::translateObject(const long index) {
    currentIndex = index;
    interface.buildTranslationWindow();
}

void Controller::finishTranslation() {
    auto entries = interface.getEntries();

    for (auto entry : entries) {
        if (!utils::regex_match(entry, utils::REGEX_INTEGER)) return;
    }

    #if !RECENT_COMPILER
    try {
    #endif
        drawer.translate(currentIndex, {stod(entries[0]), stod(entries[1])});
    #if !RECENT_COMPILER
    } catch(...) {
        return;
    }
    #endif
    interface.closeDialog();
    drawer.drawAll();
    interface.queueDraw();
}

void Controller::scaleObject(const long index) {
    currentIndex = index;
    interface.buildScalingWindow();
}

void Controller::finishScaling() {
    auto entries = interface.getEntries();

    for (auto entry : entries) {
        if (!utils::regex_match(entry, utils::REGEX_REAL_UNSIGNED)) return;
    }

    #if !RECENT_COMPILER
    try {
    #endif
        drawer.scale(currentIndex, {stod(entries[0]), stod(entries[1])});
    #if !RECENT_COMPILER
    } catch(...) {
        return;
    }
    #endif
    interface.closeDialog();
    drawer.drawAll();
    interface.queueDraw();
}

void Controller::rotateObject(const long index) {
    currentIndex = index;
    interface.buildRotationWindow();
}

void Controller::finishRotation() {
    auto entries = interface.getEntries();
    auto type = interface.getSelectedRadio();

    int i = entries.size();
    while (--i >= 0) {
        if (!utils::regex_match(entries[i], utils::REGEX_REAL)) return;
        if (type != 2) break;
    }

    double angle;
    #if !RECENT_COMPILER
    try {
    #endif
        angle = stod(entries.back());
    #if !RECENT_COMPILER
    } catch(...) {
        return;
    }
    #endif
    entries.pop_back();
    drawer.rotate(currentIndex, angle, type, entries);
    interface.closeDialog();
    drawer.drawAll();
    interface.queueDraw();
}

void Controller::rotateWindow(long direction) {
    drawer.rotateWindow(direction);
    interface.updateAngle(drawer.getWindowAngle());
    drawer.drawAll();
    interface.queueDraw();
}

void Controller::updateEntries() {
    interface.updateEntries();
}

bool Controller::objectClick(GtkWidget* const list,
                             GdkEventButton* const event) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
        objectOptions(list, event);
        return true;
    }
    return false;
}

void Controller::objectOptions(GtkWidget* const list,
                               GdkEventButton* const event) {
    return interface.showObjectOptions(list, event);
}

void Controller::rowSelected(GtkListBox* const list,
                             GtkListBoxRow* const row) {
    if (row) {
        long index = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(row));
        drawer.highlightObject(index);
        drawer.drawAll();
        interface.queueDraw();
    }
}

void Controller::openFileDialog() {
    interface.openFileDialog();
}

void Controller::saveFileDialog() {
    interface.saveFileDialog();
}

void Controller::openFile(const std::string& filename) {
    clearObjects();
    auto displayFile = fileManager->fromObj2D(filename);
    for (auto shape : displayFile) {
        interface.addShape(shape->getFormattedName());
    }
    drawer.swap(displayFile);
    drawer.drawAll();
    interface.queueDraw();
}

void Controller::saveFile(const std::string& filename) {
    auto content = fileManager->toObj(drawer.getDisplayFile());
    std::ofstream output(filename);
    output << content << std::endl;
}

void Controller::clearObjects() {
    interface.clearObjects(drawer.getDisplayFile().size());
    drawer.clearDisplayFile();
    drawer.drawAll();
    interface.queueDraw();
}

void Controller::clippingSelection() {
    auto result = interface.clippingSelection();
    if (result > -1) {
        drawer.setClippingAlgorithm(result);    
    }
}

// ----------------------------- Widget events ----------------------------- //

bool Controller::configure_event(GtkWidget* const widget,
                                 GdkEventConfigure* const event) {
    bool r = cairo::update(widget);
    drawer.resizeViewport(gtk_widget_get_allocated_width(widget),
                          gtk_widget_get_allocated_height(widget));
    return r;
}

void Controller::draw(GtkWidget* const widget, cairo_t* const cr) {
    cairo::draw(cr);
}

void Controller::quit() {
    cairo::destroy();
    gtk::quit();
}
