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

    GtkWidget* new_window(const char*, bool = true, int = 0);

    GtkWidget* new_scrolled_window(GtkAdjustment* = NULL, GtkAdjustment* = NULL, 
                                   const GtkShadowType& = GTK_SHADOW_NONE, int = -1,
                                   int = -1, int = 0);

    GtkWidget* new_dialog(const GtkWidget*, const char*, int = 0);

    GtkWidget* new_paned(const GtkOrientation& = GTK_ORIENTATION_HORIZONTAL,
                         GtkWidget* = NULL, bool = false, bool = false,
                         GtkWidget* = NULL, bool = false, bool = false);

    GtkWidget* new_paned(const GtkOrientation& = GTK_ORIENTATION_HORIZONTAL,
                         GtkWidget* = NULL, GtkWidget* = NULL);

    GtkWidget* new_frame(const char*, float = 0, float = 0.5, int = 0);

    GtkWidget* new_entry(const std::string& = "", float = 0, int = 0, int = 50);

    GtkWidget* new_button(const char*, GtkWidget* = NULL, void (*)() = NULL);

    GtkWidget* new_box(const GtkOrientation& = GTK_ORIENTATION_HORIZONTAL,
                       const GtkWidget* = NULL, int = 0, bool = false, int = 0);

    GtkWidget* new_grid(const GtkWidget* = NULL, int = 1, int = 1, bool = false,
                        bool = false, int = 0);

    GtkWidget* new_list_box(GtkWidget* const = NULL);

    GtkWidget* new_button_box(const GtkOrientation& = GTK_ORIENTATION_HORIZONTAL,
                              bool = true, int = 0);
    template<typename T>
    GtkWidget* new_button(const char*, GtkWidget* = NULL, void (*)(T) = NULL, T = NULL);

    template<typename T>
    GtkWidget* new_button_icon(const char*, const GtkIconSize& = GTK_ICON_SIZE_BUTTON,
                               void (*)(T) = NULL, T = NULL);

    template<typename... Args>
    std::vector<GtkWidget*> new_radio_group(const std::string&, const Args...);

    GtkWidget* new_expander(const char*, const bool = true);

    GtkWidget* new_expander_with_frame(const char*, const bool = true,
                                       const GtkShadowType& = GTK_SHADOW_ETCHED_IN);

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

#include "gtk.ipp"

#endif /* GTK_HPP */