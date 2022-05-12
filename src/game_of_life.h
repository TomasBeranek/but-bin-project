// File    game_of_life.h
// Author  Tomas Beranek <xberan46@stud.fit.vutbr.cz>
// Brief   Using EA to find the initial configuration of a Game of Life 2D cellular
//         automaton that leads in 1 to N steps to the desired configuration
// Date    8.5.2022
// Up2date sources can be found at: https://github.com/TomasBeranek/but-bin-project

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
    void saveBestMap(std::string outputFileName, int steps);
    void saveCurrentMap(std::string outputFileName);
    void printMap();
    size_t getSize();
    void resetToStart();

  protected:
    bool *targetMap = NULL;
    bool *map = NULL;
    bool *oldMap = NULL;
    bool *startingMap = NULL;
    size_t size = 0;

    size_t neighboursCnt(size_t i, size_t j);
};

#endif // GAME_OF_LIFE_H
