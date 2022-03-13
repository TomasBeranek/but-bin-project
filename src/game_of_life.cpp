#include <iostream>
#include <string>
#include <random>
#include <functional>
#include <cstring>
#include <fstream>

#include "game_of_life.h"

using namespace std;

GameOfLife::GameOfLife(size_t size, string inputFileName){
  this->size = size;
  std::cout << "constructor with file\n";
}

GameOfLife::GameOfLife(size_t size){
  this->size = size;
  this->map = new bool[size*size];
  this->oldMap = new bool[size*size];

  auto gen = std::bind(std::uniform_int_distribution<>(0,1),std::default_random_engine());

  for (size_t i = 0; i < size * size; i++){
    this->map[i] = gen();
  }

  memcpy(this->oldMap, this->map, (this->size * this->size) * sizeof(bool));
}

GameOfLife::~GameOfLife(){
  delete this->map;
  delete this->oldMap;
}

size_t GameOfLife::neighboursCnt(size_t i, size_t j){
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

void GameOfLife::makeStep(size_t steps){
  for (size_t n = 0;  n < steps; n++){
    this->printMap();
    cout << "\n\n";

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
      }
    }

    memcpy(this->oldMap, this->map, (this->size * this->size) * sizeof(bool));
  }
}

void GameOfLife::loadMap(bool *newMap){
  memcpy(this->map, newMap, (this->size * this->size) * sizeof(bool));
}

bool *GameOfLife::getMap(string outputFileName){
  return this->map;
}

void GameOfLife::saveMap(string outputFileName){
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
