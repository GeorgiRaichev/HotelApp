#include "HotelSystem.h"
#include <iostream>

int main() {
    try {
        HotelSystem system;
        system.start(); 
        system.run();   
    }
    catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown error occurred.\n";
    }

    return 0;
}
