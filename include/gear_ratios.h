#ifndef GEAR_RATIOS_H
#define GEAR_RATIOS_H

#include <cstddef>
#include <cstdint>
#include <unordered_set>
#include <vector>

enum class GearType : uint8_t {
    None,
    Single,
    Double,
};

void add_gear(GearType &gear_type);

struct PartNumber {
    int32_t number{};
    int32_t y{};
    int32_t x_range[2]{};

    PartNumber(int32_t part_num, int32_t part_y, int32_t part_x_range[2]);
    ~PartNumber();
};

struct Symbol {
    char symbol;
    int32_t x, y;
    int32_t ratio = 0;
    GearType gear_type = GearType::None;

    Symbol(int32_t x, int32_t y);
    Symbol(int32_t x, int32_t y, char symbol);
    ~Symbol();

    bool operator==(const Symbol &other) const {
        return x == other.x && y == other.y;
    }
};

struct PositionHash {
    size_t operator()(const Symbol &position) const {
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
    int32_t get_gear_ratios();

  private:
    int32_t y_count;
    std::vector<PartNumber> part_numbers;
    std::unordered_set<Symbol, PositionHash> symbols;
};

void gear_ratios_part1();
void gear_ratios_part2();

#endif
