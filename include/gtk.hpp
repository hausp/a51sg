/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef GTK_HPP
#define GTK_HPP

#include <gtk/gtk.h>
#include <functional>
#include <iostream>
#include <vector>
#include <tuple>


namespace gtk {
    struct box_pack {
        box_pack(GtkWidget* w, bool a, bool e = false, int o = false)
        : widget(w), alloc(a), expand(e), offset(o) { }
        box_pack(GtkWidget* w, int o = 0)
        : widget(w), alloc(false), expand(false), offset(o) { }
        GtkWidget* widget;
        bool alloc;
        bool expand;
        int offset;
    };

	void init(int, char**);
	void queue_draw(GtkWidget*);
    GtkWidget* new_window(const char*, int = 0);
    GtkWidget* new_scrolled_window(GtkAdjustment* = NULL, GtkAdjustment* = NULL, 
                                   const GtkShadowType& = GTK_SHADOW_IN, int = 0,
                                   int = 0, int = 0);
    GtkWidget* new_dialog(const GtkWidget*, const char*, int = 0);
    GtkWidget* new_frame(const char*, float = 0, float = 0.5, int = 0);
    GtkWidget* new_entry(const std::string& = "", float = 0, int = 0);
    GtkWidget* new_button(const char*, GtkWidget* = NULL, void (*)() = NULL);
    GtkWidget* new_box(const GtkOrientation& = GTK_ORIENTATION_HORIZONTAL, int = 0,
                       bool = false, int = 0);
    GtkWidget* new_grid(int = 1, int = 1, bool = false, bool = false, int = 0);
    template<typename T>
    GtkWidget* new_button(const char*, GtkWidget* = NULL, void (*)(T) = NULL, T = NULL);
    void set_entry_max_length(const GtkWidget*, int);
    void box_push_back(const GtkWidget*, const std::vector<box_pack>&);
    void box_push_front(const GtkWidget*, const std::vector<box_pack>&);
    void box_push(const std::vector<box_pack>&,
                   const std::function<void(GtkWidget*, bool, bool, int)>&);
	void main();
	void quit();
}
    
template<typename T>
GtkWidget* gtk::new_button(const char* name, GtkWidget* parent, void (*action)(T), T data) {
    GtkWidget* button = gtk_button_new_with_label(name);
    if (parent) {
        gtk_container_add(GTK_CONTAINER(parent), button);
    }
    if (action) {
        g_signal_connect_swapped(button, "clicked", G_CALLBACK(action), data);
    }
    return button;
}


#endif /* GTK_HPP */