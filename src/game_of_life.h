#include <string>

#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

class GameOfLife{
  public:
    GameOfLife(size_t size, std::string inputFileName);
    GameOfLife(size_t size, bool* targetMap, bool randomInit);
    ~GameOfLife();
    int makeStep(size_t steps);
    void loadMap(bool *newMap);
    bool *getMap();
    void saveMap(std::string outputFileName);
    void printMap();

  protected:
    bool *targetMap = NULL;
    bool *map = NULL;
    bool *oldMap = NULL;
    bool *startingMap = NULL;
    size_t size = 0;

    size_t neighboursCnt(size_t i, size_t j);
};

#endif // GAME_OF_LIFE_H
