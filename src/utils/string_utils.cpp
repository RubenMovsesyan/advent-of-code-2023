#include "string_utils.h"

std::vector<std::string> split_string(std::string_view str, char delimeter) {
    std::vector<std::string> lines;

    std::string current_line = "";

    for (size_t i = 0; i < str.length(); i++) {
        char current_character = str[i];
        if (current_character == delimeter) {
            lines.push_back(current_line);
            current_line = "";
        } else {
            current_line.push_back(current_character);
        }
    }

    if (!current_line.empty()) {
        lines.push_back(current_line);
    }

    return lines;
}

std::string_view trim(const std::string_view str, const char delimeter) {
    size_t index = 0;
    size_t size = str.size();

    while (str[index] == delimeter) {
        index++;
    }

    while (str[size] == delimeter) {
        size--;
    }

    return str.substr(index, size);
}
