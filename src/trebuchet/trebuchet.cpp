#include "trebuchet.h"
#include "file_reader.h"
#include <cctype>
#include <cstdio>
#include <optional>

int get_calibration_value(const std::string_view line) {
    int left_digit = -1;
    int right_digit = -1;
    // Get the first digit
    for (size_t i = 0; i < line.size(); i++) {
        if (isdigit(line[i])) {
            left_digit = line[i] - '0';
            break;
        }
    }

    // Get the last digit
    for (size_t i = line.size() - 1; i >= 0; i--) {
        if (isdigit(line[i])) {
            right_digit = line[i] - '0';
            break;
        }
    }

    return left_digit * 10 + right_digit;
}

void trebuchet_part1() {
    auto reader =
        FileReader::try_make((char *)"../test_files/trebuchet/trebuchet.txt");
    uint64_t total = 0;

    if (reader) {
        while (auto line = reader->next_line()) {
            total += get_calibration_value(*line);
        }
    } else {
        printf("Error: %s\n", reader.error().c_str());
    }

    printf("Total: %llu\n", total);
}

// Requirements for trebuchet part 2

// Number of possible ascii characters
const int NUM_OF_CHARS = 256;

void bad_character_heuristic(const std::string_view str, const size_t size,
                             int bad_chars[NUM_OF_CHARS]) {
    int i;
    // Initialize all occurences as -1
    for (i = 0; i < NUM_OF_CHARS; i++) {
        bad_chars[i] = -1;
    }

    // Fill the actual value of last occurence
    for (i = 0; i < size; i++) {
        bad_chars[(int)str[i]] = i;
    }
}

std::vector<int> search(const std::string_view str, const std::string pat) {
    std::vector<int> occurences;

    int m = pat.size();
    int n = str.size();

    int bad_chars[NUM_OF_CHARS];
    // Fill in the bad characters
    bad_character_heuristic(pat, m, bad_chars);

    // s is shift of the pattern with respect to text
    int s = 0;

    while (s <= (n - m)) {
        int j = m - 1;

        while (j >= 0 && pat[j] == str[s + j]) {
            j--;
        }

        if (j < 0) {
            occurences.push_back(s);
            s += (s + m < n) ? m - bad_chars[str[s + m]] : 1;
        } else {
            s += std::max(1, j - bad_chars[str[s + j]]);
        }
    }

    return occurences;
}

std::optional<int> get_min(const std::vector<int> &vec) {
    if (vec.empty()) {
        return std::nullopt;
    } else {
        int min = vec[0];

        for (int i = 0; i < vec.size(); i++) {
            if (vec[i] < min) {
                min = vec[i];
            }
        }

        return min;
    }
}

std::optional<int> get_max(const std::vector<int> &vec) {
    if (vec.empty()) {
        return std::nullopt;
    } else {
        int max = vec[0];

        for (int i = 0; i < vec.size(); i++) {
            if (vec[i] > max) {
                max = vec[i];
            }
        }

        return max;
    }
}

const std::string values[] = {
    "zero", "0", "one", "1", "two",   "2", "three", "3", "four", "4",
    "five", "5", "six", "6", "seven", "7", "eight", "8", "nine", "9",
};

int get_line_calibration_value(const std::string_view line) {
    int min_occurence = line.size();
    int min_value = 0;

    int max_occurence = -1;
    int max_value = 0;

    for (int i = 2; i < 20; i++) {
        std::vector<int> occurences = search(line, values[i]);

        auto value_min = get_min(occurences);
        auto value_max = get_max(occurences);

        if (value_min) {
            if (*value_min < min_occurence) {
                min_occurence = *value_min;
                min_value = i / 2;
            }
        }

        if (value_max) {
            if (*value_max > max_occurence) {
                max_occurence = *value_max;
                max_value = i / 2;
            }
        }
    }

    return min_value * 10 + max_value;
}

void trebuchet_part2() {
    auto reader =
        FileReader::try_make((char *)"../test_files/trebuchet/trebuchet.txt");
    uint64_t total = 0;

    if (reader) {
        while (auto line = reader->next_line()) {
            total += get_line_calibration_value(*line);
        }
    } else {
        printf("Error: %s\n", reader.error().c_str());
    }

    printf("Total: %llu\n", total);
}
