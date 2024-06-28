#include <iostream>
#include "Kromek.hpp"
#include "Helpers.hpp"

const unsigned GR1 = 1819;
const unsigned SIGMA50 = 8115; 

int main(int argc, char **argv) {
    Kromek::Driver kromek;
    
    while(true) {
        auto cmd = input("?>");
        if (cmd == "bye")
            break;
    }

    return 0;
}