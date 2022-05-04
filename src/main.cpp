#include <string>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>
#include <fstream>
#include <iostream>

#include "game_of_life.h"

using namespace std;

int main(int argc, char const *argv[]) {
  ifstream configFile("config.json");
  Json::Reader reader;
  Json::Value config;
  reader.parse(configFile, config);

  string targetMapFile =    config["TargetMap"].asString();
  string outputMapFile =    config["OutputMap"].asString();
  int mapSize =             config["MapSize"].asInt();
  int steps =               config["Steps"].asInt();
  int populationSize =      config["Population"].asInt();
  int elitism =             config["Elitism"].asInt();
  int parentPopulation =    config["ParentPopulation"].asInt();
  int mutationPercetange =  config["MutationPercetange"].asInt();
  int generations =         config["Generations"].asInt();

  vector<GameOfLife> population;

  GameOfLife* gol = new GameOfLife (mapSize);

  gol->makeStep(1000);

  gol->saveMap(outputMapFile);

  delete gol;

  return 0;
}
