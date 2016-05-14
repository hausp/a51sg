/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "Controller.hpp"

#include <iostream>
#include "cairo.hpp"
#include "gtk.hpp"
#include "BezierCurve.hpp"
#include "BSplineCurve.hpp"
#include "BaseDrawer.hpp"
#include "Drawer.hpp"
#include "FileManager.hpp"
#include "ForwardDifferenceAlgorithm.hpp"
#include "IterativeAlgorithm.hpp"
#include "Interface.hpp"
#include "Point.hpp"
#include "Line.hpp"
#include "Polygon.hpp"

Controller::Controller(Interface& interface, Drawer& BaseDrawer) 
: interface(interface), BaseDrawer(BaseDrawer) {
    fileManager = std::shared_ptr<FileManager>(new FileManager());
}

Controller::~Controller() { }

// -------------------------- Window Navigation --------------------------- //

void Controller::moveVertical(const int direction) {
    BaseDrawer.moveVertical(direction);
    BaseDrawer.drawAll();
    interface.queueDraw();
}

void Controller::moveHorizontal(const int direction) {
    BaseDrawer.moveHorizontal(direction);
    BaseDrawer.drawAll();
    interface.queueDraw();
}

void Controller::setZoom() {
    GtkWidget* zoomLevel = interface.getZoomLevel();
    std::string z = gtk_entry_get_text(GTK_ENTRY(zoomLevel));
    // std::regex numeric("^\\d+");
    // if (!std::regex_match(z, numeric)) return;
    BaseDrawer.setZoom(stoi(z)/100.0);
}

void Controller::zoom(const int d) {
    BaseDrawer.zoom(d);
    BaseDrawer.drawAll();
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
    interface.buildVertexWindow("Create Polygon");
}

void Controller::curvePressed() {
    interface.buildVertexWindow("Create Curve", false);
}

void Controller::vertexOk(bool filled) {
    filledPolygon = filled;
    interface.buildPolygonWindow();
}

void Controller::curveVertexOk() {
    interface.buildCurveWindow();
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
        BaseDrawer.addShape(p);
        interface.addShape(p->getFormattedName());
        interface.closeDialog();
        BaseDrawer.draw(*p);
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
        BaseDrawer.addShape(line);
        interface.addShape(line->getFormattedName());
        interface.closeDialog();
        BaseDrawer.draw(*line);
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
        BaseDrawer.addShape(polygon);
        interface.addShape(polygon->getFormattedName());
        interface.closeDialog();
        BaseDrawer.draw(*polygon);
        interface.queueDraw();
    }
}

void Controller::createCurve() {
    auto entries = interface.getEntries();
    std::string name = interface.getShapeName();

    for (auto entry : entries) {
        if (!utils::regex_match(entry, utils::REGEX_INTEGER)) return;
    }

    if (name != "") {
        std::vector<Point<2>> curvePoints;
        #if !RECENT_COMPILER
        try {
        #endif
        for (unsigned i = 0; i < entries.size() - 1; i += 2) {
            curvePoints.push_back(Point2D(stoi(entries[i]), stoi(entries[i + 1])));
        }
        #if !RECENT_COMPILER
        } catch(...) {
            return;
        }
        #endif

        Curve<2>* curve;

        if (interface.getSelectedRadio() == 0) {
            curve = new BezierCurve<2>(ForwardDifferenceAlgorithm<2>(), 0.05, curvePoints);
        } else {
            curve = new BSplineCurve<2>(ForwardDifferenceAlgorithm<2>(), 0.05, curvePoints);
        }
        curve->setName(name);
        BaseDrawer.addShape(curve);
        interface.addShape(curve->getFormattedName());
        interface.closeDialog();
        BaseDrawer.draw(*curve);
        interface.queueDraw();
    }
    // std::vector<Point<2>> points = {Point<2>(100, 100), Point<2>(200, 400), 
    //                                 Point<2>(300, 400), Point<2>(400, 100)};
    
}

void Controller::removeObject(const long index) {
    interface.removeShape(index);
    BaseDrawer.removeShape(index);
    BaseDrawer.drawAll();
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
        BaseDrawer.translate(currentIndex, {stod(entries[0]), stod(entries[1])});
    #if !RECENT_COMPILER
    } catch(...) {
        return;
    }
    #endif
    interface.closeDialog();
    BaseDrawer.drawAll();
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
        BaseDrawer.scale(currentIndex, {stod(entries[0]), stod(entries[1])});
    #if !RECENT_COMPILER
    } catch(...) {
        return;
    }
    #endif
    interface.closeDialog();
    BaseDrawer.drawAll();
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
    BaseDrawer.rotate(currentIndex, angle, type, entries);
    interface.closeDialog();
    BaseDrawer.drawAll();
    interface.queueDraw();
}

void Controller::rotateWindow(long direction) {
    BaseDrawer.rotateWindow(direction);
    interface.updateAngle(BaseDrawer.getWindowAngle());
    BaseDrawer.drawAll();
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
        BaseDrawer.highlightObject(index);
        BaseDrawer.drawAll();
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
    BaseDrawer.swap(displayFile);
    BaseDrawer.drawAll();
    interface.queueDraw();
}

void Controller::saveFile(const std::string& filename) {
    auto content = fileManager->toObj(BaseDrawer.getDisplayFile());
    std::ofstream output(filename);
    output << content << std::endl;
}

void Controller::clearObjects() {
    interface.clearObjects(BaseDrawer.getDisplayFile().size());
    BaseDrawer.clearDisplayFile();
    BaseDrawer.drawAll();
    interface.queueDraw();
}

void Controller::clippingSelection() {
    auto result = interface.clippingSelection();
    if (result > -1) {
        BaseDrawer.setClippingAlgorithm(result);    
    }
}

// ----------------------------- Widget events ----------------------------- //

bool Controller::configure_event(GtkWidget* const widget,
                                 GdkEventConfigure* const event) {
    bool r = cairo::update(widget);
    BaseDrawer.resizeViewport(gtk_widget_get_allocated_width(widget),
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
