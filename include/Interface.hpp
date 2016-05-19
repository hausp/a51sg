/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <gtk/gtk.h>
#include "Drawable.hpp"
#include "utils.hpp"

class Interface {

 public:
    Interface(unsigned, unsigned);

    const char* getShapeName() const {
        return gtk_entry_get_text(GTK_ENTRY(shapeName));
    }

    GtkWidget* getZoomLevel() const {
        return zoomLevel;
    }

    void updateAngle(double angle) const {
        auto text = std::to_string(static_cast<int>(angle)) + "º";
        gtk_label_set_text(GTK_LABEL(rotationAngle), text.c_str());
    }

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
    int getSelectedRadio() {
        if (radioButtons.size() > 0 && GTK_IS_TOGGLE_BUTTON(radioButtons[0])) {
            for (unsigned i = 0; i < radioButtons.size(); i++) {
                if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radioButtons[i]))) {
                    selectedRadio = i;
                    break;
                }
            }
        }
        return selectedRadio;
    }

    void build();
    void buildPointWindow();
    void buildLineWindow();
    void buildPolygonWindow();
    void buildCurveWindow();
    void buildWireframeWindow();
    void buildWireframeCreationWindow(int);
    void buildVertexWindow(const char*, bool = true);
    void buildWireframeSetup();
    void buildTranslationWindow();
    void buildScalingWindow();
    void buildRotationWindow();
    void updateEntries();
    void closeDialog();

    int clippingSelection();

    void addShape(const std::string& name);
    void removeShape(long);
    void clearObjects(long);

    void buildFileDialog(const GtkFileChooserAction&, const std::string&,
                         const std::string&, void (*)(const std::string&));
    void openFileDialog();
    void saveFileDialog();

    void showObjectOptions(GtkWidget* const, GdkEventButton*);
    void queueDraw();
    void show();

 private:
    unsigned canvasWidth;
    unsigned canvasHeight;
    int selectedRadio = -1;

    GtkWidget* window;
    GtkWidget* dialog;
    GtkWidget* canvas;
    GtkWidget* objList;

    std::vector<GtkWidget*> entries;
    std::vector<GtkWidget*> radioButtons;
    GtkWidget* shapeName;
    GtkWidget* numVertices;
    GtkWidget* zoomLevel;
    GtkWidget* rotationAngle;

    GtkWidget* buildMenubar();
    GtkWidget* buildSidebar();
    GtkWidget* buildMainbar();
    void buildCreationWindow(const char*, int, int, void (*)());
    void buildPointWindow(GtkWidget* const, const std::string,
                          const std::string, void (*)());
};

#endif /* INTERFACE_HPP */