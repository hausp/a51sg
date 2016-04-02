/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef GTK_HPP
#define GTK_HPP

#include <gtk/gtk.h>
#include <iostream>

namespace gtk {
	void init(int*, char***);
	void queue_draw(GtkWidget*);

    GtkWidget* new_window(const char*, int = 0);
    GtkWidget* new_dialog(const GtkWidget*, const char*, int = 0);

    template<typename T>
    GtkWidget* new_button(const char*, GtkWidget* = NULL, void (*)(T) = NULL, T = NULL);
    GtkWidget* new_button(const char*, GtkWidget* = NULL, void (*)() = NULL);
    
    void set_entry_max_length(const GtkWidget*, int);

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