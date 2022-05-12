# File    measure.py
# Author  Tomas Beranek <xberan46@stud.fit.vutbr.cz>
# Brief   Using EA to find the initial configuration of a Game of Life 2D cellular
#         automaton that leads in 1 to N steps to the desired configuration
# Date    8.5.2022
# Up2date sources can be found at: https://github.com/TomasBeranek/but-bin-project

import subprocess as subprocess
import sys
import json

def getUpdatedCells(command):
    o, e = subprocess.Popen(['bash', '-c', command], stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()

    time_output = e.decode('utf-8')
    real_time = time_output.split('\n')[1].split('\t')[1]
    real_m = float(real_time.split('m')[0])
    real_s = float(real_time.split('m')[1][:-1].replace(',', '.'))
    total_s = real_m*60 + real_s

    # For testing:
    # {
    #   "TargetMap": "patterns/1random64.gol",
    #   "OutputMap": "best_solution.gol",
    #   "MapSize": 64,
    #   "StepsGOL": 100,
    #   "Population": 21,
    #   "Elitism": 1,
    #   "ParentPopulation": 9,
    #   "MaxMutationPercetange": -5,
    #   "Generations": 10,
    #   "Crossover": 5,
    #   "ConcentratedMutation": 50
    # }

    with open('config.json', 'r') as f:
        config = json.load(f)

    generations = config["Generations"]
    population = config["Population"]
    steps = config["StepsGOL"]
    map_size = config["MapSize"]

    updated_cells = generations * population * steps * map_size^2
    updated_cells_s = updated_cells / total_s

    return total_s, updated_cells_s

if __name__ == '__main__':
    command = sys.argv[1]

    total_s, updated_cells_s = 0, 0
    runs = 3

    for i in range(0,runs):
        seconds, cells = getUpdatedCells(command)
        total_s += seconds
        updated_cells_s += cells

    total_s_avg = total_s / runs
    updated_cells_s_avg = updated_cells_s / runs

    print(f"Total time: {total_s_avg}s")
    print(f"Updates per second: {int(updated_cells_s_avg)}")
