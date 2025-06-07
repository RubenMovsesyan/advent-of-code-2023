#ifndef FILE_READER_H
#define FILE_READER_H

#include <expected>
#include <fstream>
#include <vector>

class FileReader {
  public:
    static std::expected<FileReader, std::string> try_make(char *path);

    std::string get_contents();

    std::optional<std::string_view> next_line();

    ~FileReader();

  private:
    FileReader(std::ifstream *file);
    std::string contents;
    std::vector<std::string> lines;
    size_t lines_iter;
};

#endif
