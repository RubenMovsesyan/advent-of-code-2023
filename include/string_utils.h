#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <vector>

// Splits the string based on a delimeter
std::vector<std::string> split_string(std::string_view str, char delimeter);

// Trims all the `delimeter` characters from the front and end of the string
std::string_view trim(const std::string_view str, const char delimeter);

#endif
