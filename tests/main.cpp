#include <string>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <climits>

#include "game_of_life.h"
#include "ea_operations.h"

using namespace std;

int main(int argc, char const *argv[]) {
  ifstream configFile("config.json");
  Json::Reader reader;
  Json::Value config;
  reader.parse(configFile, config);

  string targetMapFile =    config["TargetMap"].asString();
  string outputMapFile =    config["OutputMap"].asString();
  int mapSize =             config["MapSize"].asInt();
  int steps =               config["StepsGOL"].asInt();
  int populationSize =      config["Population"].asInt();
  int elitism =             config["Elitism"].asInt();
  int parentPopulation =    config["ParentPopulation"].asInt();
  int mutationPercetange =  config["MaxMutationPercetange"].asInt();
  int generations =         config["Generations"].asInt();
  // 0 for uniform, otherwise it says how many squares are combined
  int crossoverType =       config["Crossover"].asInt();
  int concentratedMutation =config["ConcentratedMutation"].asInt();

  // load target map
  bool* targetMap = new bool[mapSize*mapSize];
  FILE* f = fopen(targetMapFile.c_str(), "r");

  for (int i = 0; i < mapSize * mapSize; i++){
    targetMap[i] = getc(f) == '1';
  }

  vector<GameOfLife*> population;
  vector<GameOfLife*> parents;
  vector<tuple<int, int>> fitness(populationSize); // number of different fields
  tuple<int, int> bestFitness = {INT_MAX, -1};

  // TODO: just init parents and then combine them
  // random population initialization
  srand(time(NULL));
  for (int i = 0; i < populationSize; i++) {
    population.push_back(new GameOfLife (mapSize, targetMap, true));
  }

  // init parents
  for (int i = 0; i < parentPopulation; i++) {
    parents.push_back(new GameOfLife (mapSize, targetMap, false));
  }

  // load empty map
  bool* emptyMap = new bool[mapSize*mapSize];
  FILE* f2 = fopen("patterns/empty64.gol", "r");
  int c;

  for (int i = 0; i < mapSize * mapSize; i++){
    c = getc(f2);

    if (c == '\n'){
      i--;
      continue;
    }

    emptyMap[i] = c == '1';
  }

  // load full map
  bool* fullMap = new bool[mapSize*mapSize];
  f2 = fopen("patterns/full64.gol", "r");

  for (int i = 0; i < mapSize * mapSize; i++){
    c = getc(f2);

    if (c == '\n'){
      i--;
      continue;
    }

    fullMap[i] = c == '1';
  }

  // DEBUG ////////////////////////////////////////////

  parents[0]->loadMap(emptyMap);
  parents[1]->loadMap(fullMap);

  int mutation;

  if (mutationPercetange > 0)
    mutation = rand() % (mutationPercetange + 1);
  else
    mutation = -(rand() % (-mutationPercetange + 1));

  crossover(parents[0], parents[1], population[0], population[1], crossoverType);

  mutate(population[0], mutation, concentratedMutation);
  mutate(population[1], mutation, concentratedMutation);

  population[0]->printMap();
  cout << endl;
  population[1]->printMap();

  // DEBUG ////////////////////////////////////////////

  // free GOL objects
  for (int i = 0; i < populationSize; i++) {
    delete population[i];
  }

  return 0;
}
