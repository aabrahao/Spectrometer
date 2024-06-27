#include <iostream>
#include "Kromek.hpp"

int main(int argc, char **argv) {

    auto devices = Kromek::list();
    if (devices.empty()) {
        std::cout << "Ops, no spectrometer found!" << std::end;
        return 0;
    } 

    Kromek sigma50(8155);
    Kromek gr1(1819);




    return 0;
}