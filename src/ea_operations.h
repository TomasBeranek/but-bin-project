// File    ea_operations.h
// Author  Tomas Beranek <xberan46@stud.fit.vutbr.cz>
// Brief   Using EA to find the initial configuration of a Game of Life 2D cellular
//         automaton that leads in 1 to N steps to the desired configuration
// Date    8.5.2022
// Up2date sources can be found at: https://github.com/TomasBeranek/but-bin-project

#include <string>

#include "game_of_life.h"

#ifndef EA_OPERATIONS_H
#define EA_OPERATIONS_H

void crossover(GameOfLife* pA, GameOfLife* pB, GameOfLife* dA, GameOfLife* dB, int crossoverType);
void mutate(GameOfLife* x, int p, int concentratedP);

#endif // EA_OPERATIONS_H
