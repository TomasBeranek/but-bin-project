#include <iostream>
#include <string>
#include <random>
#include <functional>
#include <cstring>
#include <fstream>
#include <climits>

#include "game_of_life.h"

using namespace std;


GameOfLife::GameOfLife(const size_t size, bool* targetMap, bool randomInit){
  this->size = size;
  this->map = new bool[size*size];
  this->oldMap = new bool[size*size];
  this->targetMap = targetMap;
  this->startingMap = new bool[size*size];

  if (randomInit){
    for (size_t i = 0; i < size * size; i++){
      this->startingMap[i] = rand() % 2;
    }

    memcpy(this->oldMap, this->startingMap, (this->size * this->size) * sizeof(bool));
  }
}

GameOfLife::~GameOfLife(){
  delete this->map;
  delete this->oldMap;
  delete this->startingMap;
}

inline size_t GameOfLife::neighboursCnt(const size_t i, const size_t j){
  size_t cnt = 0;
  size_t lowerI = i - 1;
  size_t lowerJ = j - 1;
  size_t higherI = (i + 1) % this->size;
  size_t higherJ = (j + 1) % this->size;

  // underflow of rows
  if (lowerI >= this->size)
    lowerI = this->size - 1;

  // underflow of columns
  if (lowerJ >= this->size)
    lowerJ = this->size - 1;

  cnt += this->oldMap[ lowerI * this->size +  lowerJ]; // left up
  cnt += this->oldMap[      i * this->size +  lowerJ]; // left mid
  cnt += this->oldMap[higherI * this->size +  lowerJ]; // left down
  cnt += this->oldMap[higherI * this->size +       j]; // mid down
  cnt += this->oldMap[higherI * this->size + higherJ]; // right down
  cnt += this->oldMap[      i * this->size + higherJ]; // right mid
  cnt += this->oldMap[ lowerI * this->size + higherJ]; // right up
  cnt += this->oldMap[ lowerI * this->size +       j]; // mid up

  return cnt;
}

int GameOfLife::makeStep(const size_t steps){
  int bestFitness = INT_MAX;
  int fitness;

  for (size_t n = 0; n < steps; n++){
    fitness = 0;

    for (size_t i = 0; i < this->size; i++){
      for (size_t j = 0; j < this->size; j++){
        if (this->oldMap[i*this->size + j]) {
          // cell is alive
          switch (this->neighboursCnt(i,j)) {
            case 2:
            case 3:
              break; // cell stays alive
            default:
              this->map[i*this->size +j] = 0; // cell dies
          }
        } else {
          // cell is dead
          if (this->neighboursCnt(i,j) == 3) {
            this->map[i*this->size +j] = 1; // cell became alive
          }
        }

        // calculate fitness
        if (this->map[i*this->size +j] != this->targetMap[i*this->size +j]) {
          fitness++;
        }
      }
    }

    if (fitness < bestFitness) {
      bestFitness = fitness;
    }

    memcpy(this->oldMap, this->map, (this->size * this->size) * sizeof(bool));
  }

  return bestFitness;
}

void GameOfLife::loadMap(bool *newMap){
  memcpy(this->oldMap, newMap, (this->size * this->size) * sizeof(bool));
  memcpy(this->startingMap, newMap, (this->size * this->size) * sizeof(bool));
}

bool *GameOfLife::getMap(){
  return this->startingMap;
}

void GameOfLife::saveMap(string outputFileName){
  ofstream f;

  f.open(outputFileName);

  for (size_t i = 0; i < this->size; i++){
    for (size_t j = 0; j < this->size; j++){
      f << this->startingMap[i*this->size + j];
    }
    f << "\n";
  }

  f.close();
}

void GameOfLife::saveCurrentMap(string outputFileName){
  ofstream f;

  f.open(outputFileName);

  for (size_t i = 0; i < this->size; i++){
    for (size_t j = 0; j < this->size; j++){
      f << this->map[i*this->size + j];
    }
    f << "\n";
  }

  f.close();
}

void GameOfLife::printMap(){
  for (size_t i = 0; i < this->size; i++){
    for (size_t j = 0; j < this->size; j++){
      cout << this->map[i*this->size + j];
    }
    cout << "\n";
  }
}

size_t GameOfLife::getSize() {
  return this->size;
}

void GameOfLife::resetToStart() {
  memcpy(this->oldMap, this->startingMap, (this->size * this->size) * sizeof(bool));
}

void GameOfLife::saveBestMap(string outputFileName, int steps){
  bool* bestMap = new bool[size*size];
  int bestFitness = INT_MAX;
  int fitness;
  this->resetToStart();

  for (int i = 0; i < steps; i++) {
    fitness = this->makeStep(1);

    if (fitness < bestFitness) {
      memcpy(bestMap, this->map, (this->size * this->size) * sizeof(bool));
    }
  }

  // overwrite starting map and save it to file
  this->startingMap = bestMap;
  this->saveMap(outputFileName);
  return;
}
