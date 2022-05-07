#include <vector>
#include <algorithm>
#include <iostream>

#include "ea_operations.h"

using namespace std;

void crossover(GameOfLife* pA, GameOfLife* pB, GameOfLife* dA, GameOfLife* dB, int crossoverType) {
  bool *pAmap = pA->getMap();
  bool *pBmap = pB->getMap();
  bool *dAmap = dA->getMap();
  bool *dBmap = dB->getMap();
  int size = pA->getSize();

  // uniform crossover
  if (crossoverType == 0) {

    for (int i = 0; i < size * size; i++) {
      if (rand() % 2 == 0) {
        dAmap[i] = pAmap[i];
        dBmap[i] = pBmap[i];
      } else {
        dAmap[i] = pBmap[i];
        dBmap[i] = pAmap[i];
      }
    }
    dA->resetToStart();
    dB->resetToStart();
    return;
  }

  // points crossover
  if (crossoverType < 0) {
    int pointsNum = -crossoverType;
    int flip = 1;
    vector<int> points;

    // generate points for crossover
    for (int i = 0; i < pointsNum; i++) {
      points.push_back(rand() % (size * size));
    }

    for (int i = 0; i < size * size; i++) {
      // flip on points
      if (std::count(points.begin(), points.end(), i)) {
        flip = !flip;
      }

      if (flip) {
        dAmap[i] = pAmap[i];
        dBmap[i] = pBmap[i];
      } else {
        dAmap[i] = pBmap[i];
        dBmap[i] = pAmap[i];
      }
    }

    dA->resetToStart();
    dB->resetToStart();
    return;
  }

  // we are using spatial crossover with rectangles
  dA->loadMap(pA->getMap());
  dB->loadMap(pB->getMap());

  for (int i = 0; i < crossoverType; i++) {
    exit(1);
  }
}


void mutate(GameOfLife* x, int p) {
  bool* map = x->getMap();
  int size = x->getSize();
  int cellsToMutate = (int)(((size*size) / 100.0) * p);

  // randomly flip 'cellsToMutate' pixels
  for (int i = 0; i < cellsToMutate; i++) {
    int index = rand() % (size * size);
    map[index] = !map[index];
  }

  x->resetToStart();
  return;
}
