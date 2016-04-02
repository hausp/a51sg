/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "gtk.hpp"
#include <iostream>

namespace gtk {
    void init(int* argc, char*** argv) {
        gtk_init(argc, argv);
    }

    void main() {
        gtk_main();
    }

    GtkWidget* new_window(const char* name, int bwidth) {
        GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), name);
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
        gtk_container_set_border_width(GTK_CONTAINER(window), bwidth);
        return window;
    }

    GtkWidget* new_dialog(const GtkWidget* parent, const char* name, int bwidth) {
        GtkWidget* dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(dialog), name);
        gtk_window_set_resizable(GTK_WINDOW(dialog), false);
        gtk_window_set_modal(GTK_WINDOW(dialog), true);
        gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(parent));
        gtk_window_set_type_hint(GTK_WINDOW(dialog), GDK_WINDOW_TYPE_HINT_DIALOG);
        gtk_container_set_border_width(GTK_CONTAINER(dialog), bwidth);
        return dialog;
    }

    GtkWidget* new_button(const char* name, GtkWidget* parent, void (*action)()) {
        GtkWidget* button = gtk_button_new_with_label(name);
        if (parent) {
            gtk_container_add(GTK_CONTAINER(parent), button);
        }
        if (action) {
            g_signal_connect(button, "clicked", G_CALLBACK(action), NULL);
        }
        return button;
    }

    void set_entry_max_length(const GtkWidget* entry, int length) {
        gtk_entry_set_max_length(GTK_ENTRY(entry), length);
        gtk_entry_set_width_chars(GTK_ENTRY(entry), length);
        gtk_entry_set_max_width_chars(GTK_ENTRY(entry), length);
    }

    void quit() {
        gtk_main_quit();
    }

    void queue_draw(GtkWidget* widget) {
        gtk_widget_queue_draw(widget);
    }
}
