/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <gtk/gtk.h>
#include <map>
#include "Drawable.hpp"

namespace {
	std::map<DrawableType, std::string> types = {
    {DrawableType::Point, "Ponto"},
    {DrawableType::Line, "Linha"},
    {DrawableType::Polygon, "Polígono"},
    {DrawableType::Undefined, "Indefinido"}};
}

class Interface {
private:
	GtkWidget* window;
	GtkWidget* dialog;
	GtkWidget* canvas;
	GtkWidget* objList;

	std::vector<GtkWidget*> entries;
	std::vector<GtkWidget*> radioButtons;
	GtkWidget* shapeName;
	GtkWidget* numVertices;
	GtkWidget* zoomLevel;

	void buildSidebar(const GtkWidget*);
	void buildMainbar(const GtkWidget*);
	void buildCreationWindow(const char*, int, int, void (*)());
	void buildPointWindow(const GtkWidget*, const std::string, const std::string, void (*)());

public:
	Interface();

	const char* getShapeName() const { return gtk_entry_get_text(GTK_ENTRY(shapeName)); }
	GtkWidget* getZoomLevel() const { return zoomLevel; }
	const std::vector<std::string> getEntries() const {
		std::vector<std::string> sentries;
		for (auto g : entries) {
			sentries.push_back(gtk_entry_get_text(GTK_ENTRY(g)));
		}
		return sentries;
	}
	/*const std::vector<bool> getRadioButtons() const {
		std::vector<bool> radioList;
		for (auto r : radioButtons) {
			radioList.push_back(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(r)));
		}
		return radioList;
	}*/
	const unsigned getSelectedRadio() const {
	    for (unsigned i = 0; i < radioButtons.size(); i++) {
	        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radioButtons[i]))) {
	        	return i;
	        }
	    }
	}


	void build();
	void buildPointWindow();
	void buildLineWindow();
	void buildPolygonWindow();
	void buildVertexWindow();
	void buildTranslationWindow();
	void buildScalingWindow();
	void buildRotationWindow();
	void updateEntries();
	void closeDialog();


	template<unsigned D>
	void addShape(Drawable<D>* d) {
	    GtkWidget* label = gtk_label_new((types[d->getType()] + "(" + d->getName() + ")").c_str());
	    gtk_label_set_xalign(GTK_LABEL(label), 0);
	    gtk_widget_show(label);
	    gtk_list_box_insert(GTK_LIST_BOX(objList), label, -1);
	}
	void removeShape(long);

	void showPopupMenu(GtkWidget*, GdkEventButton*, gpointer);
	void queueDraw();
	void show();
};

#endif /* INTERFACE_HPP */