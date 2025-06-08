#ifndef GEAR_RATIOS_H
#define GEAR_RATIOS_H

#include <cstddef>
#include <cstdint>
#include <unordered_set>
#include <vector>

struct PartNumber {
    int32_t number{};
    int32_t y{};
    int32_t x_range[2]{};

    PartNumber(int32_t part_num, int32_t part_y, int32_t part_x_range[2]);
    ~PartNumber();
};

struct Position {
    int32_t x, y;

    Position(int32_t x, int32_t y);
    ~Position();

    bool operator==(const Position &other) const {
        return x == other.x && y == other.y;
    }
};

struct PositionHash {
    size_t operator()(const Position &position) const {
        return position.x + position.y;
    }
};

class EngineSchematic {
  public:
    EngineSchematic();
    ~EngineSchematic();

    void print_engine_description();

    void parse_line(const std::string_view &line);

    int32_t get_parts_sum();

  private:
    int32_t y_count;
    std::vector<PartNumber> part_numbers;
    std::unordered_set<Position, PositionHash> symbols;
};

void gear_ratios_part1();

#endif
