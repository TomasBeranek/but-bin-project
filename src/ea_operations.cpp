#include <vector>
#include <algorithm>
#include <iostream>

#include "ea_operations.h"

using namespace std;


void randRectangle(int *startX, int *endX, int *startY, int *endY, int size) {
  int p1X = rand() % size;
  int p1Y = rand() % size;
  int p2X = rand() % size;
  int p2Y = rand() % size;

  if (p1X < p2X) {
    *startX = p1X;
    *endX = p2X;
  } else {
    *startX = p2X;
    *endX = p1X;
  }

  if (p1Y < p2Y) {
    *startY = p1Y;
    *endY = p2Y;
  } else {
    *startY = p2Y;
    *endY = p1Y;
  }
}

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

  // if there is 0, do not change anything, if there is 1 take this item from B
  bool* mask = new bool[size*size];

  for (int i = 0; i < size * size; i++)
    mask[i] = 0;

  for (int i = 0; i < crossoverType; i++) {
    // for each spatial crossover generate two points -- rectangle
    int startX, endX, startY, endY;

    randRectangle(&startX, &endX, &startY, &endY, size);

    // negate values in masks in generated rectangle
    for (int x = startX; x <= endX; x++) {
      for (int y = startY; y <= endY; y++) {
        mask[x*size + y] = !mask[x*size + y];
      }
    }
  }

  // create new population according to mask
  for (int i = 0; i < size * size; i++) {
    if (mask[i]) {
      dAmap[i] = pBmap[i];
      dBmap[i] = pAmap[i];
    } else {
      dAmap[i] = pAmap[i];
      dBmap[i] = pBmap[i];
    }
  }

  dA->loadMap(dAmap);
  dB->loadMap(dBmap);
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
