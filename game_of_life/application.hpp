#pragma once
#include <random>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include <game_of_life/game_state.hpp>

namespace game_of_life {

class application {
 public:
  application(int rows, int cols, const std::string& option = {});
  ~application() = default;

  void execute();

 private:
  void compute_view();
  void compute_adaptive_view();
  void random_init();
  void render();

 private:
  game_state game;

  sf::RenderWindow window{sf::VideoMode(100, 100), "Conway's Game of Life"};
  sf::View view;
  float size = 10.0f;

  static constexpr int max_width = 1200;
  static constexpr int min_width = 300;
  static constexpr int max_height = 800;
  static constexpr int min_height = 300;
  static constexpr float border = 10;
};

}  // namespace game_of_life