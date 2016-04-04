/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "Interface.hpp"

#include <iostream>
#include <regex>
#include <string>
#include "gtk.hpp"
#include "signals.hpp"

Interface::Interface() { }

void Interface::build() {
    window = gtk::new_window("Area51 - Interactive Graphical System", 10);

    // Workaround for i3's inconveniency
    gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_DIALOG);

    g_signal_connect(window, "delete-event", G_CALLBACK(signals::close), NULL);

    GtkWidget* outerbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
    gtk_container_add(GTK_CONTAINER(window), outerbox);

    buildSidebar(outerbox);
    buildMainbar(outerbox);
}

void Interface::buildSidebar(const GtkWidget* outerbox) {
    auto sidebox   = gtk::new_box(GTK_ORIENTATION_VERTICAL, 3);
    auto scwin     = gtk::new_scrolled_window(NULL, NULL, GTK_SHADOW_IN, 200, 300);
    auto objLabel  = gtk_label_new("Objects list");
    auto objFrame  = gtk::new_frame("Create Object", 0.5);
    auto winFrame  = gtk::new_frame("Window", 0.5);
    auto navFrame  = gtk::new_frame("Navigation", 0.5);
    auto zoomFrame = gtk::new_frame("Zoom", 0.5);
    auto innerbox  = gtk::new_box(GTK_ORIENTATION_VERTICAL, 0, false, 3);
    auto zoombox   = gtk::new_box(GTK_ORIENTATION_HORIZONTAL, 1, false, 3);
    zoomLevel      = gtk::new_entry("5", 1, 3);
    auto objgrid   = gtk::new_grid(3, 3, true, true, 5);
    auto navGrid   = gtk::new_grid(3, 3, true, true, 5);
    objList        = gtk_list_box_new();
    auto set       = gtk::new_button("Set", NULL, signals::set_zoom);
    auto zoomIn    = gtk::new_button("+", NULL, signals::zoom_in);
    auto zoomOut   = gtk::new_button("-", NULL, signals::zoom_out);
    auto percent   = gtk_label_new("%");
    auto point     = gtk::new_button("Point", NULL, signals::point_pressed);
    auto line      = gtk::new_button("Line", NULL, signals::line_pressed);
    auto polygon   = gtk::new_button("Polygon", NULL, signals::polygon_pressed);
    auto up        = gtk::new_button("\u25B2", NULL, signals::up);
    auto left      = gtk::new_button("\u25C0", NULL, signals::left);
    auto right     = gtk::new_button("\u25B6", NULL, signals::right);
    auto down      = gtk::new_button("\u25BC", NULL, signals::down);

    g_signal_connect(objList, "popup-menu", G_CALLBACK(signals::popup_menu), NULL);
    g_signal_connect(objList, "button-press-event", G_CALLBACK(signals::button_press), NULL);
    
    gtk::box_push_back(outerbox, {{sidebox}});
    gtk::box_push_back(innerbox, {{navFrame}, {zoomFrame}});
    gtk::box_push_back(sidebox, {{objLabel}, {scwin}, {objFrame}, {winFrame}});
    gtk::box_push_back(zoombox, {{zoomIn}, {zoomOut}, {zoomLevel}, {percent, 3}, {set}});

    gtk_container_add(GTK_CONTAINER(winFrame), innerbox);
    gtk_container_add(GTK_CONTAINER(navFrame), navGrid);
    gtk_container_add(GTK_CONTAINER(scwin), objList);
    gtk_container_add(GTK_CONTAINER(objFrame), objgrid);
    gtk_container_add(GTK_CONTAINER(zoomFrame), zoombox);

    gtk_grid_attach(GTK_GRID(objgrid), point, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(objgrid), line, 2, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(objgrid), polygon, 1, 1, 2, 1);

    gtk_grid_attach(GTK_GRID(navGrid), up, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(navGrid), left, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(navGrid), right, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(navGrid), down, 1, 2, 1, 1);
}

void Interface::buildMainbar(const GtkWidget* outerbox) {
    auto centerbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    canvas = gtk_drawing_area_new();
    gtk_widget_set_size_request(canvas, 600, 600);
    gtk::box_push_back(outerbox, {{centerbox}});
    gtk::box_push_back(centerbox, {{gtk_label_new("viewport")}, {canvas}});
    g_signal_connect(canvas, "configure-event", G_CALLBACK(signals::configure_event), NULL);
    g_signal_connect(canvas, "draw", G_CALLBACK(signals::draw), NULL);
}


void Interface::buildPointWindow() {
    buildCreationWindow("Create Point", 1, 1, signals::point_ok);
}

void Interface::buildLineWindow() {
    buildCreationWindow("Create Line", 2, 2, signals::line_ok);
}

void Interface::buildPolygonWindow() {
    std::string value = gtk_entry_get_text(GTK_ENTRY(numVertices));
    int n = 0;
    std::regex numeric("^\\d+");
    if (std::regex_match(value, numeric)) {            
        n = stoi(value);
    }
    if (n < 3) return;
    gtk_widget_destroy(dialog);
    buildCreationWindow("Create Polygon", n, 3, signals::polygon_ok);
}

void Interface::buildVertexWindow() {
    dialog         = gtk::new_dialog(window, "Create Polygon", 10);
    auto mainbox   = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    auto vertexbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    auto buttonbox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    numVertices    = gtk_entry_new();

    gtk_container_add(GTK_CONTAINER(dialog), mainbox);
    gtk::box_push_back(mainbox, {{vertexbox}, {buttonbox}});
    
    gtk::set_entry_max_length(numVertices, 3);
    gtk::box_push_back(vertexbox, {{gtk_label_new("Number of vertices:")}, {numVertices}});

    gtk_button_box_set_layout(GTK_BUTTON_BOX(buttonbox), GTK_BUTTONBOX_EXPAND);
    gtk::new_button("Ok", buttonbox, signals::vertex_ok);
    gtk::new_button("Cancel", buttonbox, gtk_widget_destroy, dialog);

    gtk_widget_show_all(dialog);
}

void Interface::buildCreationWindow(const char* name, int points, int m, void (*ok)()) {
    dialog         = gtk::new_dialog(window, name, 6);
    auto mainbox   = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    auto namebox   = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    auto buttonbox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    auto pointbox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    auto scwin     = gtk_scrolled_window_new(NULL, NULL);
    shapeName      = gtk_entry_new();
    std::string frameName("Point");
    const char* labels[2] = {"x:", "y:"};
    std::vector<GtkWidget*> frames;
    std::vector<GtkWidget*> pointboxes;

    // Window group
    gtk_container_add(GTK_CONTAINER(dialog), mainbox);

    gtk::box_push_back(mainbox, {{namebox}, {scwin, true}, {buttonbox, 3}});
    gtk::box_push_back(namebox, {{gtk_label_new("Name:")}, {shapeName, true, true}});

    // Buttons group
    gtk_button_box_set_layout(GTK_BUTTON_BOX(buttonbox), GTK_BUTTONBOX_EXPAND);
    gtk_widget_set_receives_default(gtk::new_button("Ok", buttonbox, ok), true);
    gtk::new_button("Cancel", buttonbox, gtk_widget_destroy, dialog);

    // Points group
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scwin), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scwin), pointbox);
    entries.clear();

    if (points > 1) frameName += " 1";
    for (int i = 0; i < points; i++) {
        frames.push_back(gtk::new_frame(frameName.c_str(), 0, 0.5, 3));
        pointboxes.push_back(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2));

        gtk_container_set_border_width(GTK_CONTAINER(pointboxes.back()), 3);
        gtk_box_pack_start(GTK_BOX(pointbox), frames.back(), false, false, 0);
        gtk_container_add(GTK_CONTAINER(frames.back()), pointboxes.back());

        for (int j = 0; j < 2; j++) {
            entries.push_back(gtk::new_entry("", 1, 5));
            gtk::box_push_back(pointboxes.back(), {{gtk_label_new(labels[j])},
                                                   {entries.back(), true, true, 5}});
        }
        frameName = "Point " + std::to_string(i+2);
    }

    gtk_entry_set_activates_default(GTK_ENTRY(entries.back()), true);
    gtk_widget_show_all(dialog);

    int size = m * (gtk_widget_get_allocated_height(frames.back()));
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scwin), size);
}

void Interface::buildPointWindow(const GtkWidget* parent, const std::string title, const std::string label, void (*ok)()) {
    dialog             = gtk::new_dialog(parent, title.c_str(), 10);
    auto mainbox       = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    auto buttonbox     = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    auto entriesGrid   = gtk_grid_new();
    const char* labelX = std::string("X " + label + ":").c_str();
    const char* labelY = std::string("Y " + label + ":").c_str();
    
    gtk_container_add(GTK_CONTAINER(dialog), mainbox);
    gtk::box_push_back(mainbox, {{entriesGrid, true}, {buttonbox}});
    
    gtk_grid_set_row_homogeneous(GTK_GRID(entriesGrid), true);
    entries.clear();
    entries.push_back(gtk_entry_new());
    gtk::set_entry_max_length(entries.back(), 5);
    gtk_grid_attach(GTK_GRID(entriesGrid), gtk_label_new(labelX), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(entriesGrid), entries.back(), 1, 0, 1, 1);

    entries.push_back(gtk_entry_new());
    gtk::set_entry_max_length(entries.back(), 5);
    gtk_grid_attach(GTK_GRID(entriesGrid), gtk_label_new(labelY), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(entriesGrid), entries.back(), 1, 1, 1, 1);

    gtk::new_button("Ok", buttonbox, ok);
    gtk::new_button("Cancel", buttonbox, gtk_widget_destroy, dialog);

    gtk_widget_show_all(dialog);
}

void Interface::buildTranslationWindow() {
    buildPointWindow(window, "Translate object", "Offset", signals::finish_translation);
}

void Interface::buildScalingWindow() {
    buildPointWindow(window, "Scale object", "Scale", signals::finish_scaling);  
}

void Interface::buildRotationWindow() {
    dialog         = gtk::new_dialog(window, "Rotate object", 10);
    auto mainbox   = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    auto buttonbox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    auto angle     = gtk_entry_new();
    std::vector<GtkWidget*> rows(4);

    gtk_container_add(GTK_CONTAINER(dialog), mainbox);
    
    for (unsigned i = 0; i < rows.size(); i++) {
        rows[i] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
        gtk_box_pack_start(GTK_BOX(mainbox), rows[i], true, false, 0);
    }
    gtk_box_pack_end(GTK_BOX(mainbox), buttonbox, false, false, 0);
    
    gtk::set_entry_max_length(angle, 5);
    gtk::box_push_back(rows[0], {{gtk_label_new("Angle:")}, {angle, true}});

    radioButtons = gtk::new_radio_group("object center", "world center", "arbitrary point");
    GtkWidget* i1 = gtk_entry_new();
    GtkWidget* i2 = gtk_entry_new();
    entries.assign({i1, i2, angle});
    gtk::set_entry_max_length(i1, 5);
    gtk::set_entry_max_length(i2, 5);
    gtk_widget_set_sensitive(i1, false);
    gtk_widget_set_sensitive(i2, false);
    gtk::box_push_back(rows[1], {{radioButtons[0], true}, {radioButtons[1], true}});
    gtk::box_push_back(rows[2], {{radioButtons[2], true}});
    gtk::box_push_back(rows[3], {{gtk_label_new("X:"), true}, {i1, true},
                                      {gtk_label_new("Y:"), true}, {i2, true}});

    for (auto radio : radioButtons) {
        g_signal_connect(radio, "toggled", G_CALLBACK(signals::update_entries), NULL);
    }
    gtk::new_button("Ok", buttonbox, signals::finish_rotation);
    gtk::new_button("Cancel", buttonbox, gtk_widget_destroy, dialog);

    gtk_widget_show_all(dialog);
}

void Interface::updateEntries() {
    auto selected = getSelectedRadio();
    for (unsigned i = 0; i < entries.size() - 1; i++) {
        gtk_widget_set_sensitive(entries[i], selected == 2);
    }
}

void Interface::closeDialog() {
    gtk_widget_destroy(dialog);
}

void Interface::showPopupMenu(GtkWidget* objList, GdkEventButton* event, gpointer data) {
    GtkListBoxRow* selected_row = gtk_list_box_get_selected_row(GTK_LIST_BOX(objList));
    long index = -1;

    if (GTK_IS_LIST_BOX_ROW(selected_row)) {
        index = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(selected_row));

        GtkWidget* menu = gtk_menu_new();
        gtk::menu_push(menu, "Remove object", signals::remove_object, index);
        gtk::menu_push(menu, "Translate object", signals::translate_object, index);
        gtk::menu_push(menu, "Scale object", signals::scale_object, index);
        gtk::menu_push(menu, "Rotate object", signals::rotate_object, index);

        gtk_widget_show_all(menu);
        gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
        (event != NULL) ? event->button : 0,
        gdk_event_get_time((GdkEvent*) event));
    }
}

void Interface::addShape(const std::string& name) {
    GtkWidget* label = gtk_label_new(name.c_str());
    gtk_label_set_xalign(GTK_LABEL(label), 0);
    gtk_widget_show(label);
    gtk_list_box_insert(GTK_LIST_BOX(objList), label, -1);
}

void Interface::removeShape(long index) {
    GtkListBoxRow* selected_row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(objList), index);
    gtk_container_remove(GTK_CONTAINER(objList), GTK_WIDGET(selected_row));
}

void Interface::queueDraw() {
    gtk::queue_draw(canvas);
}

void Interface::show() {
    gtk_widget_show_all(window);
}
