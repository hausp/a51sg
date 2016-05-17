/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <gtk/gtk.h>
#include <memory>
#include <string>
#include <vector>

class Drawer;
class FileManager;
class Interface;
template<unsigned D>
class Point;

class Controller {
 public:
 	Controller(Interface&, Drawer&);
 	~Controller();

	// Window Navigation
	void moveVertical(const int);
	void moveHorizontal(const int);
	void setZoom();
	void zoom(const int);
	
	// Shapes stuff
	void pointPressed();
	void linePressed();
	void polygonPressed();
	void curvePressed();
	void createPoint();
	void createLine();
	void createPolygon();
	void createCurve();

	void removeObject(const long);
	void translateObject(const long);
	void finishTranslation();
	void scaleObject(const long);
	void finishScaling();
	void rotateObject(const long);
	void finishRotation();
	void rotateWindow(long);

	void updateEntries();
	bool objectClick(GtkWidget* const, GdkEventButton* const);
	void objectOptions(GtkWidget* const, GdkEventButton* const);
	void vertexOk(bool);
	void curveVertexOk();
	void rowSelected(GtkListBox* const, GtkListBoxRow* const);
	void openFileDialog();
	void saveFileDialog();
	void openFile(const std::string&);
	void saveFile(const std::string&);
	void clearObjects();
	void clippingSelection();

	// Widget events
	bool configure_event(GtkWidget* const, GdkEventConfigure* const);
	void draw(GtkWidget* const, cairo_t* const);
	void quit();

 private:
 	Interface& interface;
 	Drawer& drawer;
 	std::shared_ptr<FileManager> fileManager;
 	long currentIndex;
 	bool filledPolygon;
};

#endif /* CONTROLLER_HPP */