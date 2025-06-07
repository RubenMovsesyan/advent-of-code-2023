#ifndef CUBE_CONUNDRUM_H
#define CUBE_CONUNDRUM_H

#include <optional>
#include <vector>

struct GameSet {
    int red{};
    int green{};
    int blue{};

    GameSet(const std::string_view &parseable);
    ~GameSet();
};

class Game {
  public:
    Game(const std::string_view &line); // Parses the string to create a game

    void print_game();
    std::optional<int> game_possible(const int red, const int green,
                                     const int blue);

    int get_power();

    ~Game();

  private:
    int id;
    std::vector<GameSet> game_sets;
};

void cube_conundrum_part1();
void cube_conundrum_part2();

#endif
