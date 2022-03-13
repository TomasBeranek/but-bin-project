#include <string>

#include "game_of_life.h"

using namespace std;

// argv[1] -- size of map
// argv[2] -- output file with map configuration
int main(int argc, char const *argv[]) {
  size_t gameSize = stoi(argv[1]);
  string outputFileName = argv[2];

  GameOfLife* gol = new GameOfLife (gameSize);

  gol->makeStep(1000);

  gol->saveMap(outputFileName);

  delete gol;

  return 0;
}
