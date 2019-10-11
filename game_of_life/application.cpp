#include <game_of_life/application.hpp>

#include <cmath>
#include <iostream>
#include <stdexcept>

namespace game_of_life {

application::application(int rows, int cols, const std::string& option)
    : game{rows, cols} {
  const float expected_height = rows * size + 2 * border;
  const float expected_width = cols * size + 2 * border;

  if ((expected_width < max_width) && (expected_height < max_height)) {
    if ((expected_width > min_width) && (expected_height > min_height)) {
      window.setSize({expected_width, expected_height});
      view.setCenter(0.5f * game.cols, 0.5f * game.rows);
    } else {
      if (static_cast<float>(game.cols) / game.rows <
          static_cast<float>(min_width) / min_height) {
        size = (min_height - 2 * border) / game.rows;
        window.setSize({game.cols * size + 2 * border, min_height});
      } else {
        size = (min_width - 2 * border) / game.cols;
        window.setSize({min_width, game.rows * size + 2 * border});
      }
      view.setCenter(0.5f * game.cols, 0.5f * game.rows);
    }
  } else {
    if (static_cast<float>(game.cols) / game.rows <
        static_cast<float>(max_width) / max_height) {
      size = (max_height - 2 * border) / game.rows;
      window.setSize({game.cols * size + 2 * border, max_height});
    } else {
      size = (max_width - 2 * border) / game.cols;
      window.setSize({max_width, game.rows * size + 2 * border});
    }
    view.setCenter(0.5f * game.cols, 0.5f * game.rows);
  }

  window.setVerticalSyncEnabled(true);

  if (option == "") {
  } else if (option == "random") {
    random_init();
  } else {
    throw std::runtime_error(
        "game_of_life::application could not interpret given option '" +
        option + "'!");
  }
}

void application::compute_adaptive_view() {
  if (static_cast<float>(game.cols) / game.rows <
      static_cast<float>(window.getSize().x) / window.getSize().y) {
    size = (window.getSize().y - 2 * border) / game.rows;
  } else {
    size = (window.getSize().x - 2 * border) / game.cols;
  }
  view.setCenter(0.5f * game.cols, 0.5f * game.rows);
}

void application::compute_view() {
  view.setSize(window.getSize().x, window.getSize().y);
  view.zoom(1 / size);
  window.setView(view);
}

void application::random_init() {
  static std::mt19937 rng{std::random_device{}()};
  for (int i = 0; i < game.rows; ++i) {
    for (int j = 0; j < game.cols; ++j) {
      game(i, j) = rng() % 2;
    }
  }
}

void application::render() {
  sf::RectangleShape boundary{{game.cols, game.rows}};
  boundary.setOutlineThickness(0.1);
  boundary.setOutlineColor(sf::Color::Black);
  window.draw(boundary);

  for (int i = 0; i < game.rows; ++i) {
    for (int j = 0; j < game.cols; ++j) {
      const sf::Color color_table[2] = {sf::Color{220, 220, 250},
                                        sf::Color::Black};
      sf::RectangleShape rect{{0.9f, 0.9f}};
      rect.setOrigin(-0.05f, -0.05f);
      rect.setPosition(j * 1.0f, i * 1.0f);
      rect.setFillColor(color_table[game(i, j)]);
      // rect.setOutlineThickness(0.2f * 1.0f);
      // rect.setOutlineColor({200, 200, 200});
      window.draw(rect);
    }
  }
}

void application::execute() {
  bool mouse_moved_while_pressed = false;
  int old_mouse_x = 0;
  int old_mouse_y = 0;
  bool update = true;

  while (window.isOpen()) {
    // Get new mouse position and compute movement in space.
    const auto mouse_pos = sf::Mouse::getPosition(window);
    const int mouse_x = mouse_pos.x;
    const int mouse_y = mouse_pos.y;
    const int mouse_diff_x = old_mouse_x - mouse_x;
    const int mouse_diff_y = old_mouse_y - mouse_y;
    const float mouse_move_x =
        view.getSize().x / window.getSize().x * mouse_diff_x;
    const float mouse_move_y =
        view.getSize().y / window.getSize().y * mouse_diff_y;

    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;

        case sf::Event::MouseWheelMoved:
          size *= exp(-event.mouseWheel.delta * 0.05f);
          break;

        case sf::Event::MouseMoved:
          mouse_moved_while_pressed =
              sf::Mouse::isButtonPressed(sf::Mouse::Left);
          break;

        case sf::Event::MouseButtonPressed:
          break;

        case sf::Event::MouseButtonReleased:
          if (!mouse_moved_while_pressed) {
            const auto pixel =
                window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if ((pixel.x >= 0) && (pixel.x < game.cols) && (pixel.y >= 0) &&
                (pixel.y < game.rows))
              game(pixel.y, pixel.x) = !game(pixel.y, pixel.x);
          }
          mouse_moved_while_pressed = false;
          break;

        case sf::Event::KeyPressed:
          switch (event.key.code) {
            case sf::Keyboard::Escape:
              window.close();
              break;

            case sf::Keyboard::Space:
              update = true;
              break;

            case sf::Keyboard::V:
              compute_adaptive_view();
              break;

            case sf::Keyboard::R:
              random_init();
              break;

            case sf::Keyboard::C:
              game.clear();
              break;
          }
      }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      view.move(mouse_move_x, mouse_move_y);
    }

    if (update) {
      advance(game);
      update = false;
    }

    compute_view();
    window.clear(sf::Color::White);
    render();
    window.display();

    old_mouse_x = mouse_x;
    old_mouse_y = mouse_y;
  }
}

}  // namespace game_of_life