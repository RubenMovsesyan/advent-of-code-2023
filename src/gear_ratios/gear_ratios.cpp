#include "gear_ratios.h"
#include "file_reader.h"

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <unordered_set>

PartNumber::PartNumber(int32_t part_num, int32_t part_y,
                       int32_t part_x_range[2]) {
    number = part_num;
    y = part_y;
    x_range[0] = part_x_range[0];
    x_range[1] = part_x_range[1];
}

PartNumber::~PartNumber() {}

void add_gear(GearType &gear_type) {
    if (gear_type == GearType::None) {
        gear_type = GearType::Single;
    } else if (gear_type == GearType::Single) {
        gear_type = GearType::Double;
    }
}

Symbol::Symbol(int32_t x, int32_t y) {
    this->x = x;
    this->y = y;
}

Symbol::Symbol(int32_t x, int32_t y, char symbol) {
    this->x = x;
    this->y = y;
    this->symbol = symbol;
}

Symbol::~Symbol() {}

EngineSchematic::EngineSchematic() { y_count = 0; }
EngineSchematic::~EngineSchematic() {}

void EngineSchematic::parse_line(const std::string_view &line) {
    int32_t current_number = 0;
    int32_t current_range_start = 0;
    bool in_number = false;
    for (int32_t i = 0; i < line.size(); i++) {
        if (isdigit(line[i])) {
            current_number = current_number * 10 + (line[i] - '0');

            if (!in_number) {
                current_range_start = i;
                in_number = true;
            }
        } else {
            in_number = false;

            // Only add the part number if it is not 0
            if (current_number) {
                int32_t range[2] = {current_range_start, i};
                part_numbers.emplace_back(current_number, y_count, range);
                current_number = 0;
            }

            // Add the symbol position to the set of symbols if there is one
            if (line[i] != '.') {
                symbols.insert(Symbol(i, y_count, line[i]));
            }
        }
    }

    // If there is a current number still in the queue then add it to the part
    // numbers
    if (current_number) {
        int32_t range[2] = {current_range_start, (int32_t)line.size()};
        part_numbers.emplace_back(current_number, y_count, range);
        current_number = 0;
    }

    y_count++;
}

void EngineSchematic::print_engine_description() {
    for (auto part_number : part_numbers) {
        printf("Part Number: %d\n", part_number.number);
        printf("	Row: %d\n", part_number.y);
        printf("	Col Range: %d - %d\n", part_number.x_range[0],
               part_number.x_range[1]);
    }

    for (auto symbol : symbols) {
        printf("Symbol\n");
        printf("	x: %d\n", symbol.x);
        printf("	y: %d\n", symbol.y);
    }
}

int32_t EngineSchematic::get_parts_sum() {
    int32_t total_parts_sum = 0;

    for (auto part_num : part_numbers) {
        // Horrible lambda function for breaking out of outer loop
        // without carrying booleans
        [&]() {
            for (int32_t i = part_num.x_range[0]; i <= part_num.x_range[1];
                 i++) {
                for (int32_t j = i - 1; j < i + 1; j++) {
                    for (int32_t k = part_num.y - 1; k <= part_num.y + 1; k++) {
                        if (symbols.contains(Symbol(j, k))) {
                            total_parts_sum += part_num.number;
                            return;
                        }
                    }
                }
            }
        }();
    }

    return total_parts_sum;
}

int32_t EngineSchematic::get_gear_ratios() {
    for (auto part_num : part_numbers) {
        [&]() {
            for (int32_t i = part_num.x_range[0]; i <= part_num.x_range[1];
                 i++) {
                for (int32_t j = i - 1; j < i + 1; j++) {
                    for (int32_t k = part_num.y - 1; k <= part_num.y + 1; k++) {
                        auto symbol_handle = symbols.extract(Symbol(j, k));
                        bool ret = false;
                        if (!symbol_handle.empty()) {
                            Symbol symbol = symbol_handle.value();
                            if (symbol.symbol == '*') {
                                // printf("Found Symbol: %d %c\n",
                                // part_num.number,
                                //        symbol.symbol);

                                // Only multiply if the number is not 0
                                // Otherwise set it to that number
                                if (symbol.ratio) {
                                    symbol.ratio *= part_num.number;
                                } else {
                                    symbol.ratio = part_num.number;
                                }

                                add_gear(symbol.gear_type);

                                ret = true;
                            }

                            symbols.insert(std::move(symbol));

                            if (ret) {
                                return;
                            }
                        }
                    }
                }
            }
        }();
    }

    int32_t total_gear_ratios = 0;

    for (auto gear : symbols) {
        if (gear.gear_type == GearType::Double) {
            total_gear_ratios += gear.ratio;
        }
    }

    return total_gear_ratios;
}

void gear_ratios_part1() {
    auto reader =
        FileReader::try_make((char *)"test_files/gear_ratios/gear_ratios.txt");

    if (reader) {
        EngineSchematic engine;

        while (auto line = reader->next_line()) {
            engine.parse_line(*line);
        }
        printf("Engine Parts Sum: %d\n", engine.get_parts_sum());

    } else {
        printf("Error: %s\n", reader.error().c_str());
    }
}

void gear_ratios_part2() {
    auto reader =
        FileReader::try_make((char *)"test_files/gear_ratios/gear_ratios.txt");

    if (reader) {
        EngineSchematic engine;

        while (auto line = reader->next_line()) {
            engine.parse_line(*line);
        }
        printf("Engine Gear Ratios: %d\n", engine.get_gear_ratios());

    } else {
        printf("Error: %s\n", reader.error().c_str());
    }
}
