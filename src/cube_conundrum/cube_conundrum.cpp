#include "cube_conundrum.h"
#include "file_reader.h"
#include "string_utils.h"
#include <optional>
#include <string>

const char GAME_SPLIT_CHAR = ':';
const char GAME_ID_SPLIT_CHAR = ' ';
const char SET_SPLIT_CHAR = ';';

enum class Color : std::uint8_t {
    Red,
    Green,
    Blue,
    None,
};

inline Color get_color(const std::string_view &color_str) {
    if (color_str == "red") {
        return Color::Red;
    } else if (color_str == "green") {
        return Color::Green;
    } else if (color_str == "blue") {
        return Color::Blue;
    }

    return Color::None;
}

inline bool is_digit(char c) { return c <= '9' && c >= '0'; }

GameSet::GameSet(const std::string_view &parseable) {
    std::vector<std::string> comma_split = split_string(parseable, ',');

    for (auto cs : comma_split) {
        std::vector<std::string> value_color = split_string(trim(cs, ' '), ' ');

        switch (get_color(value_color[1])) {
        case Color::Red:
            red = std::stoi(value_color[0]);
            break;

        case Color::Green:
            green = std::stoi(value_color[0]);
            break;

        case Color::Blue:
            blue = std::stoi(value_color[0]);
            break;

        default:
            break;
        }
    }
}

GameSet::~GameSet() {}

// Class Definitions
Game::Game(const std::string_view &line) {
    std::vector<std::string> game_split = split_string(line, GAME_SPLIT_CHAR);

    // Split into Game and id
    std::vector<std::string> game_id =
        split_string(game_split[0], GAME_ID_SPLIT_CHAR);

    id = std::stoi(game_id[1]);

    // Split the sets of the game
    std::vector<std::string> game_sets_str =
        split_string(game_split[1], SET_SPLIT_CHAR);

    for (auto set : game_sets_str) {
        game_sets.emplace_back(set);
    }
}

std::optional<int> Game::game_possible(const int red, const int green,
                                       const int blue) {
    for (auto game_set : game_sets) {
        if (red < game_set.red || green < game_set.green ||
            blue < game_set.blue) {
            return std::nullopt;
        }
    }

    return id;
}

Game::~Game() {}

void Game::print_game() {
    printf("Game Id: %d\n", id);
    for (GameSet game_set : game_sets) {
        printf("	Red: %d, Green: %d, Blue: %d\n", game_set.red,
               game_set.green, game_set.blue);
    }
}

int Game::get_power() {
    // Find the minimum number of red green and blue required
    int min_red = 0;
    int min_green = 0;
    int min_blue = 0;

    for (auto game_set : game_sets) {
        min_red = std::max(min_red, game_set.red);
        min_green = std::max(min_green, game_set.green);
        min_blue = std::max(min_blue, game_set.blue);
    }

    return min_red * min_green * min_blue;
}

const int GAME_PARAMS[3] = {12, 13, 14};

void cube_conundrum_part1() {
    auto reader = FileReader::try_make(
        (char *)"../test_files/cube_conundrum/cube_conundrum.txt");

    int ids_sum = 0;
    if (reader) {
        while (auto line = reader->next_line()) {
            Game g(*line);
            if (auto id = g.game_possible(GAME_PARAMS[0], GAME_PARAMS[1],
                                          GAME_PARAMS[2])) {
                ids_sum += *id;
            }
        }
    } else {
        printf("Error: %s\n", reader.error().c_str());
    }

    printf("Sum Ids: %d\n", ids_sum);
}

void cube_conundrum_part2() {
    auto reader = FileReader::try_make(
        (char *)"../test_files/cube_conundrum/cube_conundrum.txt");

    int total_power = 0;
    if (reader) {
        while (auto line = reader->next_line()) {
            Game g(*line);
            total_power += g.get_power();
        }
    } else {
        printf("Error: %s\n", reader.error().c_str());
    }

    printf("Total Power: %d\n", total_power);
}
