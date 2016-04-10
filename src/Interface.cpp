/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "Interface.hpp"

#include <iostream>
#include <string>
#include "gtk.hpp"
#include "signals.hpp"

Interface::Interface() { }

void Interface::build() {
    window = gtk::new_window("Area51 - Interactive Graphical System");

    // Workaround for i3's inconveniency
    gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_DIALOG);

    g_signal_connect(window, "delete-event", G_CALLBACK(signals::close), NULL);

    auto mainbox = gtk::new_box(window, GTK_ORIENTATION_VERTICAL);
    buildMenubar(mainbox);

    auto outerbox = gtk::new_box(mainbox, GTK_ORIENTATION_HORIZONTAL, 10);
    gtk::set_margins(outerbox, 10, 0, 10, 10);
    buildSidebar(outerbox);
    buildMainbar(outerbox);
}

void Interface::buildMenubar(GtkWidget* const box) {
    std::vector<GtkWidget*> menus;
    auto menubar = gtk::new_menubar(menus, "_File", "_Objects");
    gtk::box_push_back(box, menubar);
    gtk::new_submenu(menus[0], "_Open", signals::open_file_dialog,
                                "_Save", signals::save_file_dialog);
    gtk::new_submenu(menus[1], "_Clear", signals::clear_objects);
}

void Interface::buildSidebar(GtkWidget* const outerbox) {
    auto sidebox   = gtk::new_box(NULL, GTK_ORIENTATION_VERTICAL, 3);
    auto scwin     = gtk::new_scrolled_window(NULL, NULL, GTK_SHADOW_IN, 200, 200);
    auto objFrame  = gtk::new_frame("Create Object", 0.5);
    auto winFrame  = gtk::new_frame("Window", 0.5);
    auto navFrame  = gtk::new_frame("Navigation", 0.5);
    auto zoomFrame = gtk::new_frame("Zoom", 0.5);
    auto objgrid   = gtk::new_grid(objFrame, 3, 3, true, true, 5);
    auto navGrid   = gtk::new_grid(navFrame, 3, 3, true, true, 5);
    auto winbox    = gtk::new_box(winFrame, GTK_ORIENTATION_VERTICAL, 0, false, 3);
    auto zoombox   = gtk::new_box(zoomFrame, GTK_ORIENTATION_HORIZONTAL, 1, false, 3);
    objList        = gtk_list_box_new();
    auto set       = gtk::new_button("Set", NULL, signals::set_zoom);
    auto zoomIn    = gtk::new_button("+", NULL, signals::zoom_in);
    auto zoomOut   = gtk::new_button("-", NULL, signals::zoom_out);
    auto point     = gtk::new_button("_Point", NULL, signals::point_pressed);
    auto line      = gtk::new_button("_Line", NULL, signals::line_pressed);
    auto polygon   = gtk::new_button("P_olygon", NULL, signals::polygon_pressed);
    auto up        = gtk::new_button("\u25B2", NULL, signals::up);
    auto left      = gtk::new_button("\u25C0", NULL, signals::left);
    auto right     = gtk::new_button("\u25B6", NULL, signals::right);
    auto down      = gtk::new_button("\u25BC", NULL, signals::down);
    zoomLevel      = gtk::new_entry("5", 1, 3, 3);
    auto objLabel  = gtk_label_new("Objects list");
    auto percent   = gtk_label_new("%");

    g_signal_connect(objList, "popup-menu",
                     G_CALLBACK(signals::object_options), NULL);
    g_signal_connect(objList, "row-selected",
                     G_CALLBACK(signals::row_selected), NULL);
    g_signal_connect(objList, "button-press-event",
                     G_CALLBACK(signals::object_click), NULL);
    
    gtk_container_add(GTK_CONTAINER(scwin), objList);

    gtk::box_push_back(outerbox, sidebox);
    gtk::box_push_back(winbox, navFrame, zoomFrame);
    gtk::box_push_back(sidebox, objLabel, scwin, objFrame, winFrame);
    gtk::box_push_back(zoombox, zoomIn, true, true,  zoomOut, true, true,
                                zoomLevel, percent, 3, set, true, true);

    gtk_grid_attach(GTK_GRID(objgrid), point, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(objgrid), line, 2, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(objgrid), polygon, 1, 1, 2, 1);

    gtk_grid_attach(GTK_GRID(navGrid), up, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(navGrid), left, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(navGrid), right, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(navGrid), down, 1, 2, 1, 1);
}

void Interface::buildMainbar(GtkWidget* const outerbox) {
    auto centerbox = gtk::new_box(NULL, GTK_ORIENTATION_VERTICAL, 3);
    canvas = gtk_drawing_area_new();
    gtk_widget_set_size_request(canvas, 500, 500);
    gtk::box_push_back(outerbox, centerbox);
    gtk::box_push_back(centerbox, gtk_label_new("viewport"), canvas);
    g_signal_connect(canvas, "configure-event",
                     G_CALLBACK(signals::configure_event), NULL);
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
    if (utils::regex_match(value, "^\\d+")) {
        n = stoi(value);
    }
    if (n < 3) return;
    gtk_widget_destroy(dialog);
    buildCreationWindow("Create Polygon", n, 3, signals::polygon_ok);
}

void Interface::buildVertexWindow() {
    dialog         = gtk::new_dialog(window, "Create Polygon", 10);
    auto mainbox   = gtk::new_box(dialog, GTK_ORIENTATION_VERTICAL, 10);
    auto vertexbox = gtk::new_box(NULL, GTK_ORIENTATION_HORIZONTAL, 3);
    auto buttonbox = gtk::new_button_box();
    numVertices    = gtk::new_entry("", 0, 3, 3);

    gtk::box_push_back(mainbox, vertexbox, buttonbox);
    gtk::box_push_back(vertexbox, gtk_label_new("Number of vertices:"), numVertices);

    gtk::new_button("Ok", buttonbox, signals::vertex_ok);
    gtk::new_button("Cancel", buttonbox, gtk_widget_destroy, dialog);

    gtk_widget_show_all(dialog);
}

void Interface::buildCreationWindow(const char* name, int points,
                                    int m, void (*ok)()) {
    dialog         = gtk::new_dialog(window, name, 6);
    auto mainbox   = gtk::new_box(dialog, GTK_ORIENTATION_VERTICAL, 3);
    auto namebox   = gtk::new_box(NULL, GTK_ORIENTATION_HORIZONTAL);
    auto buttonbox = gtk::new_button_box();
    auto scwin     = gtk::new_scrolled_window();
    auto pointbox  = gtk::new_box(scwin, GTK_ORIENTATION_VERTICAL, 0, true);
    shapeName      = gtk::new_entry("", 0, 255, 10);
    std::string frameName("Point");
    const char* labels[2] = {"x:", "y:"};
    std::vector<GtkWidget*> frames;
    std::vector<GtkWidget*> pointboxes;

    gtk::box_push_back(mainbox, namebox);
    gtk::box_push_back(mainbox, scwin, true, true);
    gtk::box_push_back(mainbox, buttonbox, 3);
    gtk::box_push_back(namebox, gtk_label_new("Name:"), shapeName, true, true);

    auto okButton = gtk::new_button("Ok", buttonbox, ok);
    gtk_widget_set_can_default(okButton, true);
    gtk_window_set_default(GTK_WINDOW(dialog), okButton);
    gtk::new_button("Cancel", buttonbox, gtk_widget_destroy, dialog);

    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scwin),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    entries.clear();

    if (points > 1) frameName += " 1";
    for (int i = 0; i < points; i++) {
        frames.push_back(gtk::new_frame(frameName.c_str()));
        pointboxes.push_back(gtk::new_box(frames.back(), GTK_ORIENTATION_HORIZONTAL,
                                          2, false, 3));

        gtk_box_pack_start(GTK_BOX(pointbox), frames.back(), false, false, 0);

        for (int j = 0; j < 2; j++) {
            entries.push_back(gtk::new_entry("", 1, 5, 5));
            gtk_entry_set_activates_default(GTK_ENTRY(entries.back()), true);
            gtk::box_push_back(pointboxes.back(), gtk_label_new(labels[j]));
            gtk::box_push_back(pointboxes.back(), entries.back(), true, true, 5);
        }
        frameName = "Point " + std::to_string(i+2);
    }

    gtk_widget_show_all(dialog);

    int size = m * (gtk_widget_get_allocated_height(frames.back()) + 6);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scwin), size);
}

void Interface::buildPointWindow(GtkWidget* const parent, const std::string title, 
                                 const std::string label, void (*ok)()) {
    dialog             = gtk::new_dialog(parent, title.c_str(), 10);
    auto mainbox       = gtk::new_box(dialog, GTK_ORIENTATION_VERTICAL, 3);
    auto buttonbox     = gtk::new_button_box();
    auto xbox          = gtk::new_box(NULL, GTK_ORIENTATION_HORIZONTAL);
    auto ybox          = gtk::new_box(NULL, GTK_ORIENTATION_HORIZONTAL);
    auto labelX        = "X " + label + ":";
    auto labelY        = "Y " + label + ":";

    gtk::box_push_back(mainbox, xbox, ybox, buttonbox);

    entries.clear();

    entries.push_back(gtk::new_entry("", 0, 5, 5));

    gtk::box_push_back(xbox, gtk_label_new(labelX.c_str()));
    gtk::box_push_back(xbox, entries.back(), true, true);

    entries.push_back(gtk::new_entry("", 0, 5, 5));

    gtk::box_push_back(ybox, gtk_label_new(labelY.c_str()));
    gtk::box_push_back(ybox, entries.back(), true, true);

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
    auto optFrame  = gtk::new_frame("Rotation Axis", 0, 0.5);
    auto ptFrame   = gtk::new_frame("Point", 0, 0.5);
    auto mainbox   = gtk::new_box(dialog, GTK_ORIENTATION_VERTICAL, 3);
    auto topbox    = gtk::new_box(NULL, GTK_ORIENTATION_HORIZONTAL, 3);
    auto optbox    = gtk::new_box(optFrame, GTK_ORIENTATION_VERTICAL, 3, true, 5);
    auto pointbox  = gtk::new_box(ptFrame, GTK_ORIENTATION_HORIZONTAL, 3, false, 5);
    auto buttonbox = gtk::new_button_box();
    radioButtons   = gtk::new_radio_group("Object Center", "World Center", 
                                          "Arbitrary Point");

    entries.assign({gtk::new_entry("", 1, 5, 5), gtk::new_entry("", 1, 5, 5),
                    gtk::new_entry("", 0, 5, 5)});
    
    gtk::new_button("Ok", buttonbox, signals::finish_rotation);
    gtk::new_button("Cancel", buttonbox, gtk_widget_destroy, dialog);

    gtk_widget_set_sensitive(entries[0], false);
    gtk_widget_set_sensitive(entries[1], false);

    gtk::box_push_back(mainbox, topbox, optFrame, ptFrame, buttonbox);
    gtk::box_push_back(topbox, gtk_label_new("Rotation Degree:"), entries[2]);
    gtk::box_push_back(optbox, radioButtons[0], radioButtons[1], radioButtons[2]);
    gtk::box_push_back(pointbox, gtk_label_new("X:"), 3, entries[0], true,
                                 gtk_label_new("Y:"), 3, entries[1], true);

    for (auto radio : radioButtons) {
        g_signal_connect(radio, "toggled", G_CALLBACK(signals::update_entries), NULL);
    }

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

void Interface::showObjectOptions(GtkWidget* const objList, GdkEventButton* event) {
    GtkListBoxRow* row;
    if (event) {
        row = gtk_list_box_get_row_at_y(GTK_LIST_BOX(objList), event->y);
    } else {
        row = gtk_list_box_get_selected_row(GTK_LIST_BOX(objList));
    }

    if (row) {
        #if RECENT_COMPILER
        if (!gtk_list_box_row_is_selected(row)) {
        #endif

            gtk_list_box_select_row(GTK_LIST_BOX(objList), row);

        #if RECENT_COMPILER
        }
        #endif

        long index = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(row));

        auto menu = gtk_menu_new();
        gtk::menu_push(menu, "Translate object", signals::translate_object, index);
        gtk::menu_push(menu, "Scale object", signals::scale_object, index);
        gtk::menu_push(menu, "Rotate object", signals::rotate_object, index);
        gtk::menu_push(menu, "Remove object", signals::remove_object, index);

        gtk_widget_show_all(menu);
        gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
        (event != NULL) ? event->button : 0,
        gdk_event_get_time((GdkEvent*) event));
    }
}

void Interface::addShape(const std::string& name) {
    auto label = gtk_label_new(name.c_str());
    //gtk_label_set_xalign(GTK_LABEL(label), 0);
    gtk_list_box_insert(GTK_LIST_BOX(objList), label, -1);
    gtk_widget_show(label);
}

void Interface::removeShape(const long index) {
    auto selected_row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(objList), index);
    gtk_container_remove(GTK_CONTAINER(objList), GTK_WIDGET(selected_row));
}

void Interface::clearObjects(long numChildren) {
    while (--numChildren >= 0) {
        auto row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(objList), 0);
        gtk_container_remove(GTK_CONTAINER(objList), GTK_WIDGET(row));        
    }
}

void Interface::buildFileDialog(const GtkFileChooserAction& action,
                                const std::string& title,
                                const std::string& okLabel,
                                void (*ok)(const std::string&)) {
    auto dialog = gtk_file_chooser_dialog_new(title.c_str(), GTK_WINDOW(window),
        action, "Cancel", GTK_RESPONSE_CANCEL, okLabel.c_str(), GTK_RESPONSE_ACCEPT, NULL);

    if (action == GTK_FILE_CHOOSER_ACTION_SAVE) {
        gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), true);
    }
    
    auto response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        ok(filename);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);    
}

void Interface::openFileDialog() {
    buildFileDialog(GTK_FILE_CHOOSER_ACTION_OPEN, "Open File", "Open", signals::open_file);
}

void Interface::saveFileDialog() {
    buildFileDialog(GTK_FILE_CHOOSER_ACTION_SAVE, "Save File", "Save", signals::save_file);
}

void Interface::queueDraw() {
    gtk::queue_draw(canvas);
}

void Interface::show() {
    gtk_widget_show_all(window);
}
