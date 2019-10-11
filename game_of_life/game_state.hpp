#pragma once
#include <algorithm>
#include <vector>

namespace game_of_life {

struct game_state {
  // enum type : int { dead = 0, alive = 1 };
  using type = int;

  game_state() = default;
  game_state(const game_state&) = default;
  game_state& operator=(const game_state&) = default;
  game_state(game_state&&) = default;
  game_state& operator=(game_state&&) = default;
  ~game_state() = default;

  game_state(int r, int c) : rows{r}, cols{c}, cells(r * c, 0), buffer(r * c) {}

  constexpr auto map(int i, int j) const noexcept { return i * cols + j; }
  constexpr auto periodic_map(int i, int j) const noexcept {
    return ((i + rows) % rows) * cols + ((j + cols) % cols);
  }
  auto& operator()(int i, int j) noexcept { return cells[map(i, j)]; }
  const auto& operator()(int i, int j) const noexcept {
    return cells[map(i, j)];
  }
  auto& get(int i, int j) noexcept { return cells[periodic_map(i, j)]; }
  const auto& get(int i, int j) const noexcept {
    return cells[periodic_map(i, j)];
  }
  void clear() noexcept { std::fill(cells.begin(), cells.end(), 0); }

  int rows{};
  int cols{};
  std::vector<type> cells{};
  decltype(cells) buffer{};
};

inline void advance(game_state& game) noexcept {
  constexpr auto is_alive = [](game_state::type alive, int neighbors) {
    return ((!static_cast<bool>(alive) && (neighbors == 3)) ||
            (static_cast<bool>(alive) &&
             ((neighbors == 2) || (neighbors == 3)))) &&
           !(static_cast<bool>(alive) && ((neighbors < 2) || (neighbors > 3)));
  };

  // left-upper
  int neighbors = game.get(-1, -1) + game.get(-1, 0) + game.get(-1, 1) +
                  game.get(0, -1) + game(0, 1) + game.get(1, -1) + game(1, 0) +
                  game(1, 1);
  game.buffer[game.map(0, 0)] = is_alive(game(0, 0), neighbors);
  // right-upper
  neighbors = game.get(-1, -2) + game.get(-1, -1) + game.get(-1, 0) +
              game.get(0, -2) + game.get(0, 0) + game.get(1, -2) +
              game.get(1, -1) + game.get(1, 0);
  game.buffer[game.periodic_map(0, -1)] = is_alive(game.get(0, -1), neighbors);
  // left-lower
  neighbors = game.get(-2, -1) + game.get(-2, 0) + game.get(-2, 1) +
              game.get(-1, -1) + game.get(-1, 0) + game.get(0, -1) +
              game.get(0, 0) + game.get(0, 1);
  game.buffer[game.periodic_map(-1, 0)] = is_alive(game.get(-1, 0), neighbors);
  // right-lower
  neighbors = game.get(-2, -2) + game.get(-2, -1) + game.get(-2, 0) +
              game.get(-1, -2) + game.get(-1, 0) + game.get(0, -2) +
              game.get(0, -1) + game.get(0, 0);
  game.buffer[game.periodic_map(-1, -1)] =
      is_alive(game.get(-1, -1), neighbors);

  for (int i = 1; i < game.cols - 1; ++i) {
    {
      const int down = game.cols + i;
      const int index = i;
      const int up = (game.rows - 1) * game.cols + i;
      const int neighbors = game.cells[up - 1] + game.cells[up] +
                            game.cells[up + 1] + game.cells[index - 1] +
                            game.cells[index + 1] + game.cells[down - 1] +
                            game.cells[down] + game.cells[down + 1];
      game.buffer[index] = is_alive(game.cells[index], neighbors);
    }
    {
      const int down = i;
      const int index = (game.rows - 1) * game.cols + i;
      const int up = (game.rows - 2) * game.cols + i;
      const int neighbors = game.cells[up - 1] + game.cells[up] +
                            game.cells[up + 1] + game.cells[index - 1] +
                            game.cells[index + 1] + game.cells[down - 1] +
                            game.cells[down] + game.cells[down + 1];
      game.buffer[index] = is_alive(game.cells[index], neighbors);
    }
  }
  for (int j = 1; j < game.rows - 1; ++j) {
    {
      const int index = j * game.cols;
      const int neighbors =
          game.cells[j * game.cols - 1] + game.cells[(j - 1) * game.cols] +
          game.cells[(j - 1) * game.cols + 1] +
          game.cells[index + game.cols - 1] + game.cells[index + 1] +
          game.cells[(j + 2) * game.cols - 1] +
          game.cells[(j + 1) * game.cols] + game.cells[(j + 1) * game.cols + 1];
      game.buffer[index] = is_alive(game.cells[index], neighbors);
    }
    {
      const int index = (j + 1) * game.cols - 1;
      const int neighbors =
          game.cells[j * game.cols - 2] + game.cells[j * game.cols - 1] +
          game.cells[(j - 1) * game.cols] + game.cells[index - 1] +
          game.cells[index - game.cols + 1] +
          game.cells[(j + 2) * game.cols - 2] +
          game.cells[(j + 2) * game.cols - 1] + game.cells[(j + 1) * game.cols];
      game.buffer[index] = is_alive(game.cells[index], neighbors);
    }
  }
  for (int i = 1; i < game.rows - 1; ++i) {
    for (int j = 1; j < game.cols - 1; ++j) {
      const int neighbors = game(i - 1, j - 1) + game(i - 1, j) +
                            game(i - 1, j + 1) + game(i, j - 1) +
                            game(i, j + 1) + game(i + 1, j - 1) +
                            game(i + 1, j) + game(i + 1, j + 1);
      game.buffer[game.map(i, j)] = is_alive(game(i, j), neighbors);
    }
  }
  swap(game.buffer, game.cells);
}

}  // namespace game_of_life