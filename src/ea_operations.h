#include <string>

#include "game_of_life.h"

#ifndef EA_OPERATIONS_H
#define EA_OPERATIONS_H

void crossover(GameOfLife* pA, GameOfLife* pB, GameOfLife* dA, GameOfLife* dB, int crossoverType);
void mutate(GameOfLife* x, int p, int concentratedP);

#endif // EA_OPERATIONS_H
