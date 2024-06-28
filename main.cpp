#include <iostream>
#include "Kromek.hpp"
#include "Helpers.hpp"

// Serial numbers
const unsigned GR1 = 1819;
const unsigned SIGMA50 = 8155;

void menu() {
    std::cout << "FIU-ARC" << std::endl
              << "Menu:" << std::endl
              << "  start" << std::endl
              << "  clear" << std::endl
              << "  read" << std::endl
              << "  bye" << std::endl;
}

int main(int argc, char **argv) {
    Kromek::Driver kromek;
    menu();
    while(true) {
       auto command = input("?>");
        if (command == "bye")
            break;
        else if (command == "start") {
            auto sensor = kromek.device(SIGMA50);
            if (sensor)
                sensor->start();
            else
                std::cout << "Ops, senor " << SIGMA50 << " not found!" << std::endl;
        }
        else if (command == "clear") {
            auto sensor = kromek.device(SIGMA50);
            if (sensor)
                sensor->clear();
             else
                std::cout << "Ops, senor " << SIGMA50 << " not found!" << std::endl;
        }
        else if (command == "read") {
            auto sensor = kromek.device(SIGMA50);
            if (sensor)
                std::cout << sensor->counts() << std::endl;
             else
                std::cout << "Ops, senor " << SIGMA50 << " not found!" << std::endl;
        }
    }
    return 0;
}