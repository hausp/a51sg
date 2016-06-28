/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "gtk.hpp"

namespace {
    void _box_push_back(const GtkWidget*) { (void)_box_push_back; }
    template<typename... Args>
    void _box_push_back(const GtkWidget*, GtkWidget*, bool, bool, Args...);
    template<typename... Args>
    void _box_push_back(const GtkWidget*, GtkWidget*, bool, Args...);
    template<typename... Args>
    void _box_push_back(const GtkWidget*, GtkWidget*, int, Args...);
    template<typename... Args>
    void _box_push_back(const GtkWidget*, GtkWidget*, Args...);

    template<typename... Args>
    void _box_push_back(const GtkWidget* box, GtkWidget* child) {
        gtk_box_pack_start(GTK_BOX(box), child, false, false, 0);
    }


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
        g_signal_connect_swapped(button, "clicked", G_CALLBACK(action), (gpointer) data);
    }
    return button;
}

template<typename T>
GtkWidget* gtk::new_button_icon(const char* icon, const GtkIconSize& size,
                           void (*action)(T), T data) {
    auto button = gtk_button_new_from_icon_name(icon, size);
    if (action) {
        g_signal_connect_swapped(button, "clicked", G_CALLBACK(action), (gpointer) data);
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