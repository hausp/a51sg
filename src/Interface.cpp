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

    GtkWidget* outerbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
    gtk_container_add(GTK_CONTAINER(window), outerbox);

    buildSidebar(outerbox);
    buildMainbar(outerbox);
}

void Interface::buildSidebar(const GtkWidget* outerbox) {
    auto sidebox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_margin_top(sidebox, 5);
    gtk_box_pack_start(GTK_BOX(outerbox), sidebox, false, false, 0);

    auto listFrame = gtk_frame_new("Objects list");
    gtk_frame_set_label_align(GTK_FRAME(listFrame), 0.5, 1);
    gtk_frame_set_shadow_type(GTK_FRAME(listFrame), GTK_SHADOW_OUT);
    gtk_box_pack_start(GTK_BOX(sidebox), listFrame, false, false, 0);

    objList = gtk_list_box_new();
    auto scwin = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scwin), 200);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scwin), 300);
    gtk_widget_set_margin_top(scwin, 3);
    gtk_container_add(GTK_CONTAINER(scwin), objList);
    gtk_container_add(GTK_CONTAINER(listFrame), scwin);

    g_signal_connect(objList, "button-press-event", G_CALLBACK(signals::button_press), NULL);
    g_signal_connect(objList, "popup-menu", G_CALLBACK(signals::popup_menu), NULL);

    auto objFrame = gtk_frame_new("Create Object");
    gtk_frame_set_label_align(GTK_FRAME(objFrame), 0.5, 0.5);
    gtk_box_pack_start(GTK_BOX(sidebox), objFrame, false, false, 2);

    auto objgrid = gtk_grid_new();
    gtk_container_set_border_width(GTK_CONTAINER(objgrid), 5);
    gtk_grid_set_column_homogeneous(GTK_GRID(objgrid), true);
    gtk_container_add(GTK_CONTAINER(objFrame), objgrid);

    auto pointButton   = gtk::new_button("Point", NULL, signals::point_pressed);
    auto lineButton    = gtk::new_button("Line", NULL, signals::line_pressed);
    auto polygonButton = gtk::new_button("Polygon", NULL, signals::polygon_pressed);

    gtk_grid_attach(GTK_GRID(objgrid), pointButton, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(objgrid), lineButton, 2, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(objgrid), polygonButton, 1, 1, 2, 1);

    auto winFrame  = gtk_frame_new("Window");
    auto navFrame  = gtk_frame_new("Navigation");
    auto zoomFrame = gtk_frame_new("Zoom");
    auto innerBox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    auto navGrid   = gtk_grid_new();

    gtk_container_set_border_width(GTK_CONTAINER(innerBox), 5);
    gtk_container_set_border_width(GTK_CONTAINER(navGrid), 3);
    gtk_frame_set_label_align(GTK_FRAME(winFrame), 0.5, 0.5);
    gtk_frame_set_label_align(GTK_FRAME(navFrame), 0.5, 0.5);
    gtk_grid_get_column_homogeneous(GTK_GRID(navGrid));

    gtk_box_pack_start(GTK_BOX(sidebox), winFrame, false, false, 2);
    gtk_container_add(GTK_CONTAINER(winFrame), innerBox);
    gtk_box_pack_start(GTK_BOX(innerBox), navFrame, false, false, 2);
    gtk_container_add(GTK_CONTAINER(navFrame), navGrid);

    auto upButton    = gtk::new_button("\u25B2", NULL, signals::up);
    auto leftButton  = gtk::new_button("\u25C0", NULL, signals::left);
    auto rightButton = gtk::new_button("\u25B6", NULL, signals::right);
    auto downButton  = gtk::new_button("\u25BC", NULL, signals::down);

    gtk_grid_attach(GTK_GRID(navGrid), upButton, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(navGrid), leftButton, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(navGrid), rightButton, 3, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(navGrid), downButton, 1, 2, 1, 1);

    auto zoombox   = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);

    gtk_frame_set_label_align(GTK_FRAME(zoomFrame), 0.5, 0.5);
    gtk_container_set_border_width(GTK_CONTAINER(zoombox), 3);
    gtk_box_pack_start(GTK_BOX(innerBox), zoomFrame, false, false, 0);
    gtk_container_add(GTK_CONTAINER(zoomFrame), zoombox);

    auto zoomIn    = gtk::new_button("+", NULL, signals::zoom_in);
    auto zoomOut   = gtk::new_button("-", NULL, signals::zoom_out);
    auto setButton = gtk::new_button("Set", NULL, signals::set_zoom);
    zoomLevel      = gtk_entry_new();

    gtk::set_entry_max_length(zoomLevel, 3);
    gtk_entry_set_text(GTK_ENTRY(zoomLevel), "5");
    gtk_entry_set_alignment(GTK_ENTRY(zoomLevel), 1);
    
    gtk::box_push_back(zoombox, {{zoomIn}, {zoomOut}, {zoomLevel}, 
                                {gtk_label_new("%"), 3}, {setButton}});
}

void Interface::buildMainbar(const GtkWidget* outerbox) {
    auto centerbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    canvas = gtk_drawing_area_new();
    gtk_widget_set_size_request(canvas, 600, 600);
    gtk_widget_set_margin_top(centerbox, 5);
    gtk_box_pack_start(GTK_BOX(outerbox), centerbox, false, false, 0);
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
        frames.push_back(gtk_frame_new(frameName.c_str()));
        pointboxes.push_back(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2));

        gtk_container_set_border_width(GTK_CONTAINER(frames.back()), 3);
        gtk_container_set_border_width(GTK_CONTAINER(pointboxes.back()), 3);

        gtk_box_pack_start(GTK_BOX(pointbox), frames.back(), false, false, 0);
        gtk_container_add(GTK_CONTAINER(frames.back()), pointboxes.back());

        for (int j = 0; j < 2; j++) {
            entries.push_back(gtk_entry_new());
            gtk::set_entry_max_length(entries.back(), 5);
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
    GtkWidget* angle       = gtk_entry_new();
    const unsigned numRows = 4;
    std::vector<GtkWidget*> rows(numRows);

    gtk_container_add(GTK_CONTAINER(dialog), mainbox);
    for (unsigned i = 0; i < numRows; i++) {
        rows[i] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
        gtk_box_pack_start(GTK_BOX(mainbox), rows[i], true, false, 0);
    }
    gtk_box_pack_end(GTK_BOX(mainbox), buttonbox, false, false, 0);
    
    gtk::set_entry_max_length(angle, 5);
    gtk::box_push_back(rows[0], {{gtk_label_new("Angle:")}, {angle, true}});

    GtkWidget* r1 = gtk_radio_button_new_with_label(NULL, "object center");
    GtkWidget* r2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(r1), "world center");
    GtkWidget* r3 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(r1), "arbitrary point");
    GtkWidget* i1 = gtk_entry_new();
    GtkWidget* i2 = gtk_entry_new();
    radioButtons.assign({r1, r2, r3});
    entries.assign({i1, i2, angle});
    gtk::set_entry_max_length(i1, 5);
    gtk::set_entry_max_length(i2, 5);
    gtk_widget_set_sensitive(i1, false);
    gtk_widget_set_sensitive(i2, false);
    gtk::box_push_back(rows[1], {{r1, true}, {r2, true}});
    gtk::box_push_back(rows[2], {{r3, true}});
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
    GtkWidget* menu = gtk_menu_new();
    GtkListBoxRow* selected_row = gtk_list_box_get_selected_row(GTK_LIST_BOX(objList));
    long index = -1;

    if (GTK_IS_LIST_BOX_ROW(selected_row)) {
        index = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(selected_row));

        GtkWidget* item1 = gtk_menu_item_new_with_label("Remove object");
        g_signal_connect_swapped(item1, "activate", G_CALLBACK(signals::remove_object),(gpointer)index);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), item1);

        GtkWidget* item2 = gtk_menu_item_new_with_label("Translate object");
        g_signal_connect_swapped(item2, "activate", G_CALLBACK(signals::translate_object),(gpointer)index);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), item2); 

        GtkWidget* item3 = gtk_menu_item_new_with_label("Scale object");
        g_signal_connect_swapped(item3, "activate", G_CALLBACK(signals::scale_object),(gpointer)index);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), item3);

        GtkWidget* item4 = gtk_menu_item_new_with_label("Rotate object");
        g_signal_connect_swapped(item4, "activate", G_CALLBACK(signals::rotate_object),(gpointer)index);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), item4);

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
