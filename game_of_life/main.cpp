#include <iostream>
#include <sstream>
#include <string>

#include <game_of_life/application.hpp>

using namespace std;

int main(int argc, char** argv) {
  int rows = 50;
  int cols = 50;
  string option{};

  if (argc >= 3) {
    stringstream input{argv[1]};
    input >> rows;
    input = stringstream{argv[2]};
    input >> cols;
    if (argc == 4) option = argv[3];
  }

  cout << "Game of Life resolution set to " << rows << " x " << cols << " !\n"
       << "Given option was '" << option << "'.\n";
  cout << "usage: " << argv[0] << " [<rows> <cols> [<option>]]\n";

  game_of_life::application app{rows, cols, option};
  app.execute();
}