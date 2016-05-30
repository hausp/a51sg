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

#include "BaseVector.hpp"
#include "BaseMatrix.hpp"
#include "BaseTransformation.hpp"
#include "Transformation.hpp"

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

    // BaseVector v1 {0, 1, 2, 3};
    // BaseVector v2 {3, 2, 1, 0};
    // BaseVector v3 = v1 + v2;
    // std::cout << v3 << std::endl;

    // Matrix<3,3> m1 {{3,3,3},{2,2,2},{1,1,1}};
    // Matrix<3,3> m2 {{2,2,2},{1,1,1},{0,0,0}};
    // Matrix<3,4> m2 {{2,2,2,2},{1,1,1,1},{0,0,0,0}};
    // m2[2] = {2,2,2,2};
    // BaseMatrix m3 = m1 * m2;
    // std::cout << m3 << std::endl;
}
