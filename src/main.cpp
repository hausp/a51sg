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

int main(int argc, char** argv) {
 	// Init GTK+ with command line options
    gtk::init(argc, argv);

    unsigned canvasWidth = 500;
    unsigned canvasHeight = 500;

    // Graphical Interface - managing ugly buttons and boxes
    Interface gui(canvasWidth, canvasHeight);

    // BaseDrawer - the cool stuff goes here
    // Default values to window - length(600), height(600)
    Drawer BaseDrawer(canvasWidth, canvasHeight, 20);
    
    // Controller - being God
    Controller controller(gui, BaseDrawer);

    // Set controller to receive GTK+ signals
    signals::set_receiver(&controller);
	
    // Build graphical environment
    gui.build();

    // Show GUI
    gui.show();

    // Start GTK+
    gtk::main();

//     Point<3> scientist{6, 6, 6};
//     Point<2> sinner = scientist;
//     std::cout << "sinner = {" << sinner[0] << ", " << sinner[1] << "}" << std::endl;
//     scientist = sinner;
//     std::cout << "scientist = {" << scientist[0] << ", " << scientist[1] << ", "
//     << scientist[2] << "}" << std::endl;
}
