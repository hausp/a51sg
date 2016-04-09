/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef GTK_HPP
#define GTK_HPP

#include <gtk/gtk.h>
#include <functional>
#include <iostream>
#include <vector>

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
    template<typename T>
    GtkWidget* new_button(const char*, GtkWidget* = NULL, void (*)(T) = NULL, T = NULL);

    template<typename... Args>
    std::vector<GtkWidget*> new_radio_group(const std::string&, const Args...);

    void set_entry_max_length(const GtkWidget*, int, int = 50);

    template<typename... Args>
    void box_push_back(const GtkWidget*, Args...);

    void box_push_back(const GtkWidget*, const std::vector<box_pack>&);

    void box_push_front(const GtkWidget*, const std::vector<box_pack>&);

    void box_push(const std::vector<box_pack>&,
                   const std::function<void(GtkWidget*, bool, bool, int)>&);

    template<typename T>
    void menu_push(GtkWidget*, const std::string&, void (*)(T) = NULL, T = NULL);

    template<typename... Args>
    GtkWidget* new_menubar(std::vector<GtkWidget*>&, Args...);

    template<typename... Args>
    void new_submenu(GtkWidget*, Args...);

    void set_margins(GtkWidget*, int = 0, int = 0, int = 0, int = 0);

    void main();

    void quit();
}

namespace {
    void _box_push_back(const GtkWidget*) { (void)_box_push_back; }

    template<typename... Args>
    void _box_push_back(const GtkWidget* box, GtkWidget* child, bool alloc,
                       bool extend, int offset, Args... args) {
        gtk_box_pack_start(GTK_BOX(box), child, alloc, extend, offset);
        _box_push_back(box, args...);
    }

    template<typename... Args>
    void _box_push_back(const GtkWidget* box, GtkWidget* child, bool alloc,
                       bool extend, Args... args) {
        gtk_box_pack_start(GTK_BOX(box), child, alloc, extend, 0);
        _box_push_back(box, args...);
    }

    template<typename... Args>
    void _box_push_back(const GtkWidget* box, GtkWidget* child, bool alloc,
                       Args... args) {
        gtk_box_pack_start(GTK_BOX(box), child, alloc, false, 0);
        _box_push_back(box, args...);
    }

    template<typename... Args>
    void _box_push_back(const GtkWidget* box, GtkWidget* child, Args... args) {
        gtk_box_pack_start(GTK_BOX(box), child, false, false, 0);
        _box_push_back(box, args...);
    }

    template<typename... Args>
    void _box_push_back(const GtkWidget* box, GtkWidget* child, int offset,
                        Args... args) {
        gtk_box_pack_start(GTK_BOX(box), child, false, false, offset);
        _box_push_back(box, args...);
    }

    std::vector<GtkWidget*> add_radio_buttons(std::vector<GtkWidget*>& list) {
        (void)add_radio_buttons;
        return list;
    }

    template<typename... Args>
    std::vector<GtkWidget*> add_radio_buttons(std::vector<GtkWidget*>& list,
                                              const std::string& text,
                                              Args... args) {
        list.push_back(gtk_radio_button_new_with_label_from_widget(
            GTK_RADIO_BUTTON(list.front()), text.c_str()));
        return add_radio_buttons(list, args...);
    }

    void add_menus(const GtkWidget*, std::vector<GtkWidget*>&) {
        (void)add_menus;
    }

    template<typename... Args>
    void add_menus(const GtkWidget* menubar, std::vector<GtkWidget*>& menus,
                   const char* name, Args... args) {
        auto menuitem = gtk_menu_item_new_with_mnemonic(name);
        gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem);
        menus.push_back(menuitem);
        add_menus(menubar, menus, args...);
    }

    void add_items(GtkMenuShell*) { (void)add_items; }

    template<typename... Args>
    void add_items(GtkMenuShell* container, const char* name,
                   void (*funct)(), Args... args) {
    auto item = gtk_menu_item_new_with_mnemonic(name);
    gtk_menu_shell_append(container, item);
    g_signal_connect(GTK_MENU_ITEM(item), "activate", G_CALLBACK(funct), NULL);
    add_items(container, args...);
    }

    template<typename T, typename... Args>
    void add_items(GtkMenuShell* container, const char* name, void (*funct)(T),
                   T data, Args... args) {
        auto item = gtk_menu_item_new_with_mnemonic(name);
        gtk_menu_shell_append(GTK_MENU_SHELL(container), item);
        g_signal_connect_swapped(GTK_MENU_ITEM(item), "activate", 
                                 G_CALLBACK(funct), (gpointer)data);
        add_items(container, args...);
    }
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

template<typename... Args>
void gtk::box_push_back(const GtkWidget* parent, Args... args) {
    _box_push_back(parent, args...);
}


template<typename T>
GtkWidget* gtk::new_button(const char* name, GtkWidget* parent, void (*action)(T), T data) {
    auto button = gtk_button_new_with_mnemonic(name);
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
    return add_radio_buttons(list, args...);
}

template<typename T>
void gtk::menu_push(GtkWidget* menu, const std::string& text, void (*fn)(T), T data) {
    auto item = gtk_menu_item_new_with_mnemonic(text.c_str());
    g_signal_connect_swapped(item, "activate", G_CALLBACK(fn),(gpointer)data);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
}

template<typename ...Args>
GtkWidget* gtk::new_menubar(std::vector<GtkWidget*>& menus, Args... args) {
    auto menubar = gtk_menu_bar_new();
    add_menus(menubar, menus, args...);
    return menubar;
}

template<typename... Args>
void gtk::new_submenu(GtkWidget* parent, Args... args) {
    auto container = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(parent), container);
    add_items(GTK_MENU_SHELL(container), args...);
}

#endif /* GTK_HPP */