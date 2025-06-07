#include "file_reader.h"
#include "string_utils.h"
#include <sstream>

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
    //     printf("Line!: %s\n", line.c_str());
    // }

    // for (auto lines_it = lines.begin(); lines_it != lines.end(); lines_it++)
    // {
    //     printf("%s\n", lines_it->c_str());
    // }

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
