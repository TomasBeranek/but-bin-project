import subprocess as subprocess
import sys
import json

if __name__ == '__main__':
    command = sys.argv[1]
    o, e = subprocess.Popen(['bash', '-c', command], stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()

    time_output = e.decode('utf-8')
    user_time = time_output.split('\n')[2].split('\t')[1]
    user_m = float(user_time.split('m')[0])
    user_s = float(user_time.split('m')[1][:-1].replace(',', '.'))
    total_s = user_m*60 + user_s

    # For testing:
    # {
    #   "TargetMap": "patterns/earth.gol",
    #   "OutputMap": "best_solution.gol",
    #   "MapSize": 100,
    #   "StepsGOL": 150,
    #   "Population": 21,
    #   "Elitism": 1,
    #   "ParentPopulation": 9,
    #   "MaxMutationPercetange": 5,
    #   "Generations": 10,
    #   "Crossover": -5
    # }

    with open('config.json', 'r') as f:
        config = json.load(f)

    generations = config["Generations"]
    population = config["Population"]
    steps = config["StepsGOL"]
    map_size = config["MapSize"]

    updated_cells = generations * population * steps * map_size^2
    updated_cells_s = updated_cells / total_s

    print(f"Total time: {total_s}s")
    print(f"Updates per second: {int(updated_cells_s)}")
