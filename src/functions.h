#ifndef FUNCTIONS_H
#define FUNCTIONS_H
// functions.h
// Utility functions

// C++ Standard Library
#include <vector>
#include <string>

std::vector<char> splitToChar(std::string str);
std::vector<std::string> split(std::string str, char splitChar);
std::vector<std::string> splitToLines(std::string str, unsigned int lineWidth);

#endif // FUNCTIONS_H
