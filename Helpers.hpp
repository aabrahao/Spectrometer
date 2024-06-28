#ifndef __AA__HELPER_HPP__
#define __AA__HELPER_HPP__

#include <iostream>
#include <string_view>

inline std::ostream &log() { return std::cout; }
inline std::ostream &err() { return std::cout; }

std::string input(std::string_view prompt);

#endif