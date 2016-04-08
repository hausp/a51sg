/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef GTK_HPP
#define GTK_HPP

#include <gtk/gtk.h>
#include <functional>
#include <iostream>
#include <vector>

namespace {
    std::vector<GtkWidget*> new_radio_group_aux(std::vector<GtkWidget*>& list) {
        (void)new_radio_group_aux;
        return list;
    }

    void add_menus(const GtkWidget* menubar, std::vector<GtkWidget*> menus) {
        (void)add_menus;
    }

    template<typename... Args>
    std::vector<GtkWidget*> new_radio_group_aux(std::vector<GtkWidget*>& list,
                                                const std::string& text, Args... args) {
        list.push_back(gtk_radio_button_new_with_label_from_widget(
            GTK_RADIO_BUTTON(list.front()), text.c_str()));
        return new_radio_group_aux(list, args...);
    }

    template<typename... Args>
    void add_menus(const GtkWidget* menubar, std::vector<GtkWidget*>& menus,
                   const char* name, Args... args) {
        auto menuitem = gtk_menu_item_new_with_mnemonic(name);
        gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem);
        menus.push_back(menuitem);
        add_menus(menubar, menus, args...);
    }
}

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

    using menu_item_pack = std::pair<const char*, void (*)()>;

	void init(int, char**);
	void queue_draw(GtkWidget*);
    GtkWidget* new_window(const char*, int = 0);
    GtkWidget* new_scrolled_window(GtkAdjustment* = NULL, GtkAdjustment* = NULL, 
                                   const GtkShadowType& = GTK_SHADOW_NONE, int = 0,
                                   int = 0, int = 0);
    GtkWidget* new_dialog(const GtkWidget*, const char*, int = 0);
//    template<typename... Args>
//    GtkWidget* new_dialog(const char*, GtkWindow* = NULL, bool = true, Args...);
    GtkWidget* new_frame(const char*, float = 0, float = 0.5, int = 0);
    GtkWidget* new_entry(const std::string& = "", float = 0, int = 0, int = 50);
    GtkWidget* new_button(const char*, GtkWidget* = NULL, void (*)() = NULL);
    GtkWidget* new_box(const GtkWidget* = NULL, 
                       const GtkOrientation& = GTK_ORIENTATION_HORIZONTAL,
                       int = 0, bool = false, int = 0);
    GtkWidget* new_grid(const GtkWidget* = NULL, int = 1, int = 1, bool = false,
                        bool = false, int = 0);
    GtkWidget* new_button_box(const GtkOrientation& = GTK_ORIENTATION_HORIZONTAL,
                              bool = true, int = 0);
//    template<typename... Args>
//    void add_button(GtkDialog*, const char*, gint = GTK_RESPONSE_NONE, Args...);
    template<typename T>
    GtkWidget* new_button(const char*, GtkWidget* = NULL, void (*)(T) = NULL, T = NULL);
    template<typename... Args>
    std::vector<GtkWidget*> new_radio_group(const std::string&, const Args... args);
    void set_entry_max_length(const GtkWidget*, int, int = 50);
    void box_push_back(const GtkWidget*, const std::vector<box_pack>&);
    void box_push_front(const GtkWidget*, const std::vector<box_pack>&);
    void box_push(const std::vector<box_pack>&,
                   const std::function<void(GtkWidget*, bool, bool, int)>&);
    template<typename T>
    void menu_push(GtkWidget*, const std::string&, void (*)(gpointer) = NULL, T = NULL);
    void new_submenu(GtkWidget* parent, const std::initializer_list<menu_item_pack>& args);
    template<typename... Args>
    std::vector<GtkWidget*> new_menubar(GtkWidget*&, const char*, Args...);
    void set_margins(GtkWidget*, int = 0, int = 0, int = 0, int = 0);
	void main();
	void quit();
}

// template<typename... Args>
// GtkWidget* gtk::new_dialog(const char* name, GtkWindow* parent, bool modal, Args... args) {
//     auto dialog = gtk_dialog_new();
//     gtk_window_set_title(GTK_WINDOW(dialog), name);
//     gtk_window_set_modal(GTK_WINDOW(dialog), modal);
//     gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(parent));
//     add_button(GTK_DIALOG(dialog), args...);
//     return dialog;
// }


// template<typename... Args>
// void gtk::add_button(GtkDialog* dialog, const char* name, gint response, Args... args) {
//    gtk_dialog_add_button(dialog, name, response);
//    add_button(dialog, name, args...);
// }
    
template<typename T>
GtkWidget* gtk::new_button(const char* name, GtkWidget* parent, void (*action)(T), T data) {
    GtkWidget* button = gtk_button_new_with_mnemonic(name);
    if (parent) {
        gtk_container_add(GTK_CONTAINER(parent), button);
    }
    if (action) {
        g_signal_connect_swapped(button, "clicked", G_CALLBACK(action), data);
    }
    return button;
}

template<typename... Args>
std::vector<GtkWidget*> gtk::new_radio_group(const std::string& text, const Args... args) {
    std::vector<GtkWidget*> list;
    list.reserve(sizeof...(Args) + 1);
    list.push_back(gtk_radio_button_new_with_label(NULL, text.c_str()));
    return new_radio_group_aux(list, args...);
}

template<typename T>
void gtk::menu_push(GtkWidget* menu, const std::string& text, void (*fn)(gpointer), T data) {
    GtkWidget* item = gtk_menu_item_new_with_mnemonic(text.c_str());
    g_signal_connect_swapped(item, "activate", G_CALLBACK(fn),(gpointer)data);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
}

template<typename ...Args>
std::vector<GtkWidget*> gtk::new_menubar(GtkWidget*& menubar, const char* name, Args... args) {
    menubar = gtk_menu_bar_new();
    std::vector<GtkWidget*> menus;
    add_menus(menubar, menus, name, args...);
    return menus;
}

#endif /* GTK_HPP */