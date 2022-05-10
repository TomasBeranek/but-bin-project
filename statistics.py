import subprocess as subprocess
import sys
import json
from parse import parse

def changeConfig(original_config, target=None, mutation=None, crossover=None, concentration=None):
    config = original_config.copy()

    config["Generations"] = 500

    if target:
        config["TargetMap"] = target

    if mutation:
        config["MaxMutationPercetange"] = mutation

    if crossover:
        config["Crossover"] = crossover

    if concentration:
        config["ConcentratedMutation"] = concentration

    with open('config.json', 'w') as f:
        json.dump(config, f, indent = 2)

def runWithConfig(command):
    o, e = subprocess.Popen([command], stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()

    bin_output = o.decode('utf-8')

    # take only the best solution
    bin_output = bin_output.split('\n')[-2]

    best_fitness, in_generation = parse("New best fitness: {}	Generation: {}", bin_output)
    return int(best_fitness), int(in_generation)

if __name__ == '__main__':
    command = sys.argv[1]

    with open('config.json', 'r') as f:
        original_config = json.load(f)

    # print CSV header
    print("Type,Crossover,MaxMutationPercetange,ConcentratedMutation,BestFitness,Generation")

    with open('statistics.csv', 'a') as outf:
        outf.write("Type,Crossover,MaxMutationPercetange,ConcentratedMutation,BestFitness,Generation\n")

    files = ["patterns/1random64.gol", "patterns/2random64.gol", "patterns/3random64.gol"]
    runs = 5

    normal crossover and normal mutation
    crossover = [-1, -2, -5, -10, -15, -20, -40, -50]
    mutation  = [ 1,  2,  3,   4,   5,  10,  15,  20]

    for c in crossover:
        for m in mutation:
                avg_best_fitness = 0
                avg_generation = 0

                for file in files:
                    for run in range(0,runs):
                        changeConfig(original_config, target=file, mutation=m, crossover=c)
                        best_fitness, generation = runWithConfig(command)

                        avg_best_fitness += best_fitness
                        avg_generation += generation

                avg_best_fitness /= runs*len(files)
                avg_generation /= runs*len(files)
                print(f"normal,{c},{m},0,{round(avg_best_fitness)},{round(avg_generation)}")

                with open('statistics.csv', 'a') as outf:
                    outf.write(f"normal,{c},{m},0,{round(avg_best_fitness)},{round(avg_generation)}\n")

    # spatial crossover and spatial mutation
    crossover = [ 1,  2, 5, 10, 15,  20,  40,  50]
    mutation  = [-1, -2,-3, -4, -5, -10, -15, -20]
    concentration = [50, 70, 30]

    for con in concentration:
        for c in crossover:
            for m in mutation:
                    avg_best_fitness = 0
                    avg_generation = 0

                    for file in files:
                        for run in range(0,runs):
                            changeConfig(original_config, target=file, mutation=m, crossover=c)
                            best_fitness, generation = runWithConfig(command)

                            avg_best_fitness += best_fitness
                            avg_generation += generation

                    avg_best_fitness /= runs*len(files)
                    avg_generation /= runs*len(files)
                    print(f"spatial,{c},{m},{con},{round(avg_best_fitness)},{round(avg_generation)}")

                    with open('statistics.csv', 'a') as outf:
                        outf.write(f"spatial,{c},{m},{con},{round(avg_best_fitness)},{round(avg_generation)}\n")

    # restore original config
    with open('config.json', 'w') as f:
        json.dump(original_config, f, indent = 2)
