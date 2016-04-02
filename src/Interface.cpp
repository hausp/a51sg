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
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Area51 - Interactive Graphical System");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);

    // Workaround for i3's inconveniency
    gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_DIALOG);

    g_signal_connect(window, "delete-event", G_CALLBACK(signals::close), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    GtkWidget* outerbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_add(GTK_CONTAINER(window), outerbox);

    buildSidebar(outerbox);
    buildMainbar(outerbox);
}

void Interface::buildSidebar(const GtkWidget* outerbox) {
    GtkWidget* sidebox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(outerbox), sidebox, false, false, 0);

    GtkWidget* listFrame = gtk_frame_new("Objects list");
    gtk_frame_set_label_align(GTK_FRAME(listFrame), 0.5, 0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(listFrame), GTK_SHADOW_OUT);
    gtk_box_pack_start(GTK_BOX(sidebox), listFrame, false, false, 0);

    objList = gtk_list_box_new();
    GtkWidget *scwin = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scwin), 200);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scwin), 300);
    gtk_container_add(GTK_CONTAINER(scwin), objList);
    gtk_container_add(GTK_CONTAINER(listFrame), scwin);

    g_signal_connect(objList, "button-press-event", G_CALLBACK(signals::button_press), NULL);
    g_signal_connect(objList, "popup-menu", G_CALLBACK(signals::popup_menu), NULL);

    GtkWidget* objFrame = gtk_frame_new("Create Object");
    gtk_frame_set_label_align(GTK_FRAME(objFrame), 0.5, 0.5);
    gtk_box_pack_start(GTK_BOX(sidebox), objFrame, false, false, 2);

    GtkWidget* objgrid = gtk_grid_new();
    gtk_container_set_border_width(GTK_CONTAINER(objgrid), 5);
    gtk_grid_set_column_homogeneous(GTK_GRID(objgrid), true);
    gtk_container_add(GTK_CONTAINER(objFrame), objgrid);

    GtkWidget* pointButton = gtk_button_new_with_label("Point");
    GtkWidget* lineButton = gtk_button_new_with_label("Line");
    GtkWidget* polygonButton = gtk_button_new_with_label("Polygon");
    
    g_signal_connect(pointButton, "clicked", G_CALLBACK(signals::point_pressed), NULL);
    g_signal_connect(lineButton, "clicked", G_CALLBACK(signals::line_pressed), NULL);
    g_signal_connect(polygonButton, "clicked", G_CALLBACK(signals::polygon_pressed), NULL);

    gtk_grid_attach(GTK_GRID(objgrid), pointButton, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(objgrid), lineButton, 2, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(objgrid), polygonButton, 1, 1, 2, 1);

    GtkWidget* winFrame = gtk_frame_new("Window");
    gtk_frame_set_label_align(GTK_FRAME(winFrame), 0.5, 0.5);
    gtk_box_pack_start(GTK_BOX(sidebox), winFrame, false, false, 2);

    GtkWidget* innerBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_container_set_border_width(GTK_CONTAINER(innerBox), 5);
    gtk_container_add(GTK_CONTAINER(winFrame), innerBox);

    GtkWidget* navFrame = gtk_frame_new("Navigation");
    gtk_frame_set_label_align(GTK_FRAME(navFrame), 0.5, 0.5);
    gtk_box_pack_start(GTK_BOX(innerBox), navFrame, false, false, 2);

    GtkWidget* navGrid = gtk_grid_new();
    gtk_grid_get_column_homogeneous(GTK_GRID(navGrid));
    gtk_container_set_border_width(GTK_CONTAINER(navGrid), 3);
    gtk_container_add(GTK_CONTAINER(navFrame), navGrid);

    GtkWidget* upButton = gtk_button_new_with_label("\u25B2");
    g_signal_connect(upButton, "clicked", G_CALLBACK(signals::up), NULL);
    gtk_grid_attach(GTK_GRID(navGrid), upButton, 1, 0, 1, 1);

    GtkWidget* leftButton = gtk_button_new_with_label("\u25C0");
    g_signal_connect(leftButton, "clicked", G_CALLBACK(signals::left), NULL);
    gtk_grid_attach(GTK_GRID(navGrid), leftButton, 0, 1, 1, 1);

    GtkWidget* rightButton = gtk_button_new_with_label("\u25B6");
    g_signal_connect(rightButton, "clicked", G_CALLBACK(signals::right), NULL);
    gtk_grid_attach(GTK_GRID(navGrid), rightButton, 3, 1, 1, 1);

    GtkWidget* downButton = gtk_button_new_with_label("\u25BC");
    g_signal_connect(downButton, "clicked", G_CALLBACK(signals::down), NULL);
    gtk_grid_attach(GTK_GRID(navGrid), downButton, 1, 2, 1, 1);

    GtkWidget* zoomFrame = gtk_frame_new("Zoom");
    gtk_frame_set_label_align(GTK_FRAME(zoomFrame), 0.5, 0.5);
    gtk_box_pack_start(GTK_BOX(innerBox), zoomFrame, false, false, 0);

    GtkWidget* zoombox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_set_border_width(GTK_CONTAINER(zoombox), 3);
    gtk_container_add(GTK_CONTAINER(zoomFrame), zoombox);

    GtkWidget* line1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_box_pack_start(GTK_BOX(zoombox), line1, false, false, 0);

    GtkWidget* zoomIn = gtk_button_new_with_label("+");
    g_signal_connect(zoomIn, "clicked", G_CALLBACK(signals::zoom_in), NULL);
    gtk_box_pack_start(GTK_BOX(line1), zoomIn, false, false, 0);

    GtkWidget* zoomOut = gtk_button_new_with_label("-");
    g_signal_connect(zoomOut, "clicked", G_CALLBACK(signals::zoom_out), NULL);
    gtk_box_pack_start(GTK_BOX(line1), zoomOut, false, false, 0);

    zoomLevel = gtk_entry_new();
    gtk::set_entry_max_length(zoomLevel, 3);
    gtk_entry_set_text(GTK_ENTRY(zoomLevel), "5");
    gtk_entry_set_alignment(GTK_ENTRY(zoomLevel), 1);
    gtk_box_pack_start(GTK_BOX(line1), zoomLevel, false, false, 0);

    GtkWidget* label2 = gtk_label_new("%");
    gtk_box_pack_start(GTK_BOX(line1), label2, false, false, 3);

    GtkWidget* setButton = gtk_button_new_with_label("Set");
    g_signal_connect(setButton, "clicked", G_CALLBACK(signals::set_zoom), NULL);
    gtk_box_pack_start(GTK_BOX(line1), setButton, false, false, 0);

    GtkWidget* line2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_box_pack_start(GTK_BOX(zoombox), line2, false, false, 0);
}

void Interface::buildMainbar(const GtkWidget* outerbox) {
    GtkWidget* centerbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_box_pack_start(GTK_BOX(outerbox), centerbox, false, false, 0);

    canvas = gtk_drawing_area_new();
    gtk_widget_set_size_request(canvas, 800, 600);
    gtk_box_pack_start(GTK_BOX(centerbox), canvas, false, false, 0);

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
    dialog               = gtk::new_dialog(window, "Create Polygon", 10);
    GtkWidget* mainbox   = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget* vertexbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    GtkWidget* buttonbox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    numVertices          = gtk_entry_new();

    gtk_container_add(GTK_CONTAINER(dialog), mainbox);
    gtk_box_pack_start(GTK_BOX(mainbox), vertexbox, false, false, 0);
    gtk_box_pack_start(GTK_BOX(mainbox), buttonbox, false, false, 0);
    
    gtk::set_entry_max_length(numVertices, 3);
    gtk_box_pack_start(GTK_BOX(vertexbox), gtk_label_new("Number of vertices:"), false, false, 0);
    gtk_box_pack_start(GTK_BOX(vertexbox), numVertices, false, false, 0);

    gtk_button_box_set_layout(GTK_BUTTON_BOX(buttonbox), GTK_BUTTONBOX_EXPAND);
    gtk::new_button("Ok", buttonbox, signals::vertex_ok);
    gtk::new_button("Cancel", buttonbox, gtk_widget_destroy, dialog);

    gtk_widget_show_all(dialog);
}

void Interface::buildCreationWindow(const char* name, int points, int m, void (*ok)()) {
    dialog                = gtk::new_dialog(window, name, 6);
    GtkWidget* mainbox    = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    GtkWidget* namebox    = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget* buttonbox  = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    GtkWidget* pointbox   = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget* scwin      = gtk_scrolled_window_new(NULL, NULL);
    shapeName             = gtk_entry_new();
    std::vector<GtkWidget*> point_frames;
    std::vector<GtkWidget*> point_grids;
    GtkWidget* labelX;
    GtkWidget* labelY;

    // Window group
    gtk_container_add(GTK_CONTAINER(dialog), mainbox);
    gtk_box_pack_start(GTK_BOX(mainbox), namebox, false, false, 0);
    gtk_box_pack_start(GTK_BOX(mainbox), scwin, true, false, 0);
    gtk_box_pack_start(GTK_BOX(mainbox), buttonbox, false, false, 3);

    // Name group
    gtk_box_pack_start(GTK_BOX(namebox), gtk_label_new("Name:"), false, false, 0);
    gtk_box_pack_start(GTK_BOX(namebox), shapeName, true, true, 0);

    // Buttons group
    gtk_button_box_set_layout(GTK_BUTTON_BOX(buttonbox), GTK_BUTTONBOX_EXPAND);
    gtk_widget_set_receives_default(gtk::new_button("Ok", buttonbox, ok), true);
    gtk::new_button("Cancel", buttonbox, gtk_widget_destroy, dialog);

    // Points group
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scwin), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scwin), pointbox);
    entries.clear();

    for (int i = 0; i < points; i++) {
        point_frames.push_back(gtk_frame_new("Coordinates"));
        point_grids.push_back(gtk_grid_new());

        labelX = gtk_label_new("x:");
        labelY = gtk_label_new("y:");

        entries.push_back(gtk_entry_new());
        entries.push_back(gtk_entry_new());

        gtk_container_set_border_width(GTK_CONTAINER(point_frames.back()), 3);
        gtk_container_set_border_width(GTK_CONTAINER(point_grids.back()), 3);

        gtk_box_pack_start(GTK_BOX(pointbox), point_frames.back(), false, false, 0);
        gtk_container_add(GTK_CONTAINER(point_frames.back()), point_grids.back());

        gtk::set_entry_max_length(entries[entries.size()-2], 5);
        gtk::set_entry_max_length(entries.back(), 5);

        gtk_grid_set_column_homogeneous(GTK_GRID(point_grids.back()), true);
        gtk_grid_attach(GTK_GRID(point_grids.back()), labelX, 0, 0, 2, 1);
        gtk_grid_attach(GTK_GRID(point_grids.back()), entries[entries.size()-2], 2, 0, 4, 1);
        gtk_grid_attach(GTK_GRID(point_grids.back()), labelY, 6, 0, 2, 1);
        gtk_grid_attach(GTK_GRID(point_grids.back()), entries.back(), 8, 0, 4, 1);
    }

    gtk_entry_set_activates_default(GTK_ENTRY(entries.back()), true);
    gtk_widget_show_all(dialog);

    int size = m * (gtk_widget_get_allocated_height(point_frames.back()));
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scwin), size);
}

void Interface::buildPointWindow(const GtkWidget* parent, const std::string title, const std::string label, void (*ok)()) {
    dialog                 = gtk::new_dialog(parent, title.c_str(), 10);
    GtkWidget* mainbox     = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    GtkWidget* buttonbox   = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    GtkWidget* entriesGrid = gtk_grid_new();
    const char* labelX     = std::string("X " + label + ":").c_str();
    const char* labelY     = std::string("Y " + label + ":").c_str();
    
    gtk_container_add(GTK_CONTAINER(dialog), mainbox);
    gtk_box_pack_start(GTK_BOX(mainbox), entriesGrid, true, false, 0);
    gtk_box_pack_end(GTK_BOX(mainbox), buttonbox, false, false, 0);
    
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
    dialog                 = gtk::new_dialog(window, "Rotate object", 10);
    GtkWidget* mainbox     = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    GtkWidget* buttonbox   = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    GtkWidget* line1       = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    GtkWidget* line2       = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    GtkWidget* line3       = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    GtkWidget* line4       = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    GtkWidget* angle       = gtk_entry_new();
    
    gtk_container_add(GTK_CONTAINER(dialog), mainbox);
    gtk_box_pack_start(GTK_BOX(mainbox), line1, true, false, 0);
    gtk_box_pack_start(GTK_BOX(mainbox), line2, true, false, 0);
    gtk_box_pack_start(GTK_BOX(mainbox), line3, true, false, 0);
    gtk_box_pack_start(GTK_BOX(mainbox), line4, true, false, 0);
    gtk_box_pack_end(GTK_BOX(mainbox), buttonbox, false, false, 0);
    
    gtk::set_entry_max_length(angle, 5);
    gtk_box_pack_start(GTK_BOX(line1), gtk_label_new("Angle:"), false, false, 0);
    gtk_box_pack_start(GTK_BOX(line1), angle, true, false, 0);

    GtkWidget* r1 = gtk_radio_button_new_with_label(NULL, "object center");
    GtkWidget* r2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(r1), "world center");
    GtkWidget* r3 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(r1), "arbitrary point");
    GtkWidget* i1 = gtk_entry_new();
    GtkWidget* i2 = gtk_entry_new();
    radioButtons.clear();
    radioButtons.push_back(r1);
    radioButtons.push_back(r2);
    radioButtons.push_back(r3);
    entries.clear();
    entries.push_back(i1);
    entries.push_back(i2);
    entries.push_back(angle);
    gtk::set_entry_max_length(i1, 5);
    gtk::set_entry_max_length(i2, 5);
    gtk_widget_set_sensitive(i1, false);
    gtk_widget_set_sensitive(i2, false);
    gtk_box_pack_start(GTK_BOX(line2), r1, true, false, 0);
    gtk_box_pack_start(GTK_BOX(line2), r2, true, false, 0);
    gtk_box_pack_start(GTK_BOX(line3), r3, true, false, 0);
    gtk_box_pack_start(GTK_BOX(line4), gtk_label_new("X:"), true, false, 0);
    gtk_box_pack_start(GTK_BOX(line4), i1, true, false, 0);
    gtk_box_pack_start(GTK_BOX(line4), gtk_label_new("Y:"), true, false, 0);
    gtk_box_pack_start(GTK_BOX(line4), i2, true, false, 0);

    for (auto radio : radioButtons) {
        g_signal_connect(radio, "toggled", G_CALLBACK(signals::update_entries), NULL);
    }
    gtk::new_button("Ok", buttonbox, signals::finish_rotation);
    gtk::new_button("Cancel", buttonbox, gtk_widget_destroy, dialog);

    gtk_widget_show_all(dialog);
}

void Interface::updateEntries() {
    unsigned selected = getSelectedRadio();
    for (unsigned i = 0; i < entries.size() - 1; i++) {
        gtk_widget_set_sensitive(entries[i], selected == 2);
    }
}

void Interface::closeDialog() {
    gtk_widget_destroy(dialog);
}

void Interface::showPopupMenu(GtkWidget* objList, GdkEventButton* event, gpointer data) {
    GtkWidget* menu = gtk_menu_new();
    GtkListBoxRow* selected_row = gtk_list_box_get_selected_row(GTK_LIST_BOX(objList));
    long index = -1;

    if (GTK_IS_LIST_BOX_ROW(selected_row)) {
        index = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(selected_row));

        GtkWidget* item1 = gtk_menu_item_new_with_label("Remover objeto");
        g_signal_connect_swapped(item1, "activate", G_CALLBACK(signals::remove_object),(gpointer)index);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), item1);

        GtkWidget* item2 = gtk_menu_item_new_with_label("Mover objeto");
        g_signal_connect_swapped(item2, "activate", G_CALLBACK(signals::translate_object),(gpointer)index);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), item2); 

        GtkWidget* item3 = gtk_menu_item_new_with_label("Escalonar objeto");
        g_signal_connect_swapped(item3, "activate", G_CALLBACK(signals::scale_object),(gpointer)index);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), item3);

        GtkWidget* item4 = gtk_menu_item_new_with_label("Girar objeto");
        g_signal_connect_swapped(item4, "activate", G_CALLBACK(signals::rotate_object),(gpointer)index);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), item4);

        gtk_widget_show_all(menu);
        gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
        (event != NULL) ? event->button : 0,
        gdk_event_get_time((GdkEvent*) event));
    }
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
