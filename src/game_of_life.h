#include <string>

#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

class GameOfLife{
  public:
    GameOfLife(size_t size, std::string inputFileName);
    GameOfLife(size_t size);
    ~GameOfLife();
    void makeStep(size_t steps);
    void loadMap(bool *newMap);
    bool *getMap(std::string outputFileName);
    void saveMap(std::string outputFileName);
    void printMap();

  protected:
    bool *map = NULL;
    bool *oldMap = NULL;
    size_t size = 0;

    size_t neighboursCnt(size_t i, size_t j);
};

#endif // GAME_OF_LIFE_H
