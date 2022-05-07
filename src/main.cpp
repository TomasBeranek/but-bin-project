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
  int c;

  for (int i = 0; i < mapSize * mapSize; i++){
    c = getc(f);
    if (c == '\n'){
      i--;
      continue;
    }
    targetMap[i] = c == '1';
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

  for (int g = 0; g < generations; g++) {
    // calculate fitness
    // TODO: do it in parallel
    for (int i = 0; i < populationSize; i++) {
      fitness[i] = tuple<int, int>(population[i]->makeStep(steps), i);
    }

    // sort population by fitness -- lower better -> ascending sort
    std::sort(fitness.begin(), fitness.end(),
              [](std::tuple<int, int> &a, std::tuple<int, int> &b)
              { return std::get<0>(a) < std::get<0>(b); });


    // copy best individuals to parents
    // TODO: add more ways of picking parents
    for (int i = 0; i < parentPopulation; i++) {
      GameOfLife* individual = population[get<1>(fitness[i])];
      parents[i]->loadMap(individual->getMap());
    }

    // TODO: if there are two individuals with same fitness pick the new one
    // automatically add best individual to next generation -- elitism
    GameOfLife* individual = population[get<1>(fitness[0])];
    population[0]->loadMap(individual->getMap());

    // crossing of parents
    for (int i = elitism; i < populationSize; i+=2) {
      GameOfLife* parentA = parents[rand() % parentPopulation];
      GameOfLife* parentB = parents[rand() % parentPopulation];
      GameOfLife* descentantA = population[i];
      GameOfLife* descentantB = population[i+1];
      crossover(parentA, parentB, descentantA, descentantB, crossoverType);
    }

    // apply mutation to all population (except the first one)
    for (int i = 1; i < populationSize; i++) {
      if (mutationPercetange > 0)
        mutate(population[i], rand() % (mutationPercetange + 1), concentratedMutation);
      else
        mutate(population[i], -(rand() % (-mutationPercetange + 1)), concentratedMutation);
    }

    cout << g << endl;

    // save and print the best individual so far
    if (get<0>(fitness[0]) < get<0>(bestFitness)) {
      bestFitness = {get<0>(fitness[0]), 0};
      cout << "New best fitness: " << get<0>(bestFitness) << "\tGeneration: " << g << endl;

      // save the best solution
      population[0]->saveBestMap(outputMapFile, steps);
    }
  } // for each generation

  // free GOL objects
  for (int i = 0; i < populationSize; i++) {
    delete population[i];
  }

  return 0;
}
