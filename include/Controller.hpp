/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <vector>
#include <gtk/gtk.h>

class Drawer2D;
class Interface;
template<unsigned D>
class Point;

class Controller {
 public:
 	Controller(Interface&, Drawer2D&);
 	~Controller();

	// Window Navigation
	void moveVertical(int);
	void moveHorizontal(int);
	void setZoom();
	void zoom(int);
	
	// Shapes stuff
	void pointPressed();
	void linePressed();
	void polygonPressed();
	void createPoint();
	void createLine();
	void createPolygon();

	void removeObject(long index);
	void translateObject(long index);
	void finishTranslation();
	void scaleObject(long index);
	void finishScaling();
	void rotateObject(long index);
	void finishRotation();

	void updateEntries();
	bool objectClick(GtkWidget*, GdkEventButton*);
	void objectOptions(GtkWidget*, GdkEventButton*);
	void vertexOk();
	void rowSelected(GtkListBox*, GtkListBoxRow*);

	// Widget events
	bool configure_event(GtkWidget*, GdkEventConfigure*, gpointer);
	void draw(GtkWidget*, cairo_t*, gpointer);
	void quit();

 private:
 	Interface& interface;
 	Drawer2D& drawer;
 	long currentIndex;
};

#endif /* CONTROLLER_HPP */