#include <iostream>
#include "Kromek.hpp"

int main(int argc, char **argv) {

    auto devices = Kromek::list();
    if (devices.empty()) {
        std::cout << "Ops, no spectrometer found!" << std::end;
        return 0;
    } 

    auto sigma50 = kromek::


    return 0;
}