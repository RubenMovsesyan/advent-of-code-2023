#include "file_reader.h"
#include <sstream>

std::vector<std::string> split_string(std::string str, char delimeter) {
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

    return lines;
}

std::expected<FileReader, std::string> FileReader::try_make(char *path) {
    std::ifstream file(path);

    if (file.good()) {
        return FileReader(&file);
    } else {
        return std::unexpected("Could Not Read File");
    }
}

FileReader::FileReader(std::ifstream *file) {
    std::stringstream str_stream;
    str_stream << file->rdbuf();
    contents = str_stream.str();
    lines = split_string(contents, '\n');

    // for (auto line : lines) {
    // 	printf("Line!: %s\n", line.c_str());
    // }

    // lines_iter = lines.begin();
    lines_iter = 0;
}

std::string FileReader::get_contents() { return contents; }

std::optional<std::string_view> FileReader::next_line() {
    if (lines_iter == lines.size()) {
        return std::nullopt;
    } else {
        // std::string line = *lines_iter;
        // lines_iter++;
        // return line;
        std::string_view view{lines[lines_iter++]};
        return view;
    }
}

FileReader::~FileReader() {}
