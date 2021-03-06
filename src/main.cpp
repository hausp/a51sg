/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "gtk.hpp"
#include "signals.hpp"
#include "Controller.hpp"
#include "Interface.hpp"
#include "BaseDrawer.hpp"
#include "Drawer.hpp"
#include "Matrix.hpp"
#include "Point.hpp"
#include "Line.hpp"
#include "Wireframe.hpp"

int main(int argc, char** argv) {
     // Init GTK+ with command line options
    gtk::init(argc, argv);

    unsigned canvasWidth = 500;
    unsigned canvasHeight = 500;

    // Graphical Interface - managing ugly buttons and boxes
    Interface gui(canvasWidth, canvasHeight);

    // Drawer - the cool stuff goes here
    Drawer drawer(canvasWidth, canvasHeight, 20);
    
    // Controller - being God
    Controller controller(gui, drawer);

    // Set controller to receive GTK+ signals
    signals::set_receiver(&controller);

    // Build graphical environment
    gui.build();

    // Show GUI
    gui.show();

    // Start GTK+
    gtk::main();
}
