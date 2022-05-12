// File    main.cpp
// Author  Tomas Beranek <xberan46@stud.fit.vutbr.cz>
// Brief   Using EA to find the initial configuration of a Game of Life 2D cellular
//         automaton that leads in 1 to N steps to the desired configuration
// Date    8.5.2022
// Up2date sources can be found at: https://github.com/TomasBeranek/but-bin-project

#include <string>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <climits>
#include <thread>

#include "game_of_life.h"
#include "ea_operations.h"

using namespace std;


void getFitness(vector<tuple<int, int>> *fitness, vector<GameOfLife*> *population, int tID, int steps, int populationSize) {
  for (int i = tID; i < populationSize; i += 12)
    (*fitness)[i] = tuple<int, int>((*population)[i]->makeStep(steps), i);
}

// picks random individual based on relative rank in fitness
int getRandomParent(vector<tuple<int, int>> fitness) {
  int n = fitness.size();
  int sum = (1 / 2.0) * n * (n + 1);
  int x = (rand() % sum) + 1; // <1,sum>
  int tmp = 0;

  for (int i = n; i >= 1; --i) {
    tmp += i;

    if (x <= tmp) {
      return get<1>(fitness[n - i]);
    }
  }

  return 0;
}

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
    array<thread, 12> threads;
    int tID = 0;

    for( auto & t : threads )
    {
    	t = thread(getFitness, &fitness, &population, tID, steps, populationSize);
      tID++;
    }

    for( auto & t : threads )
    {
    	t.join();
    }

    // for (int i = 0; i < populationSize; i++) {
    //   thread t1(empty);
    //   // thread thread_obj(getFitness, fitness, population, i, steps);
    //   fitness[i] = tuple<int, int>(population[i]->makeStep(steps), i);
    // }

    // sort population by fitness -- lower better -> ascending sort
    std::sort(fitness.begin(), fitness.end(),
              [](std::tuple<int, int> &a, std::tuple<int, int> &b)
              { return std::get<0>(a) < std::get<0>(b); });


    // copy best individuals to parents
    for (int i = 0; i < parentPopulation; i++) {
      GameOfLife* individual = population[get<1>(fitness[i])];
      parents[i]->loadMap(individual->getMap());
    }

    // TODO: if there are two individuals with same fitness pick the new one
    // automatically add best individual to next generation -- elitism
    GameOfLife* individual = population[get<1>(fitness[0])];
    population[0]->loadMap(individual->getMap());

    // crossing of parents
    for (int i = elitism; i < (populationSize-1); i+=2) {
      GameOfLife* parentA = parents[rand() % parentPopulation];
      GameOfLife* parentB = parents[rand() % parentPopulation];
      GameOfLife* descentantA = population[i];
      GameOfLife* descentantB = population[i+1];
      crossover(parentA, parentB, descentantA, descentantB, crossoverType);
    }

    // apply mutation to all population (except the first one)
    for (int i = elitism; i < populationSize; i++) {
      if (mutationPercetange > 0)
        mutate(population[i], rand() % (mutationPercetange + 1), concentratedMutation);
      else
        mutate(population[i], -(rand() % (-mutationPercetange + 1)), concentratedMutation);
    }

    // save and print the best individual so far
    if (get<0>(fitness[0]) < get<0>(bestFitness)) {
      bestFitness = {get<0>(fitness[0]), 0};
      cout << "New best fitness: " << get<0>(bestFitness) << "\tGeneration: " << g << endl;

      // save the best solution
      population[0]->saveBestMap(outputMapFile, steps);
    }
  } // for each generation

  // free GOL objects
  for (int i = 0; i < populationSize; i++)
    delete population[i];

  for (int i = 0; i < parentPopulation; i++)
    delete parents[i];

  delete targetMap;

  return 0;
}
