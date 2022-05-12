# File    show_graphs.py
# Author  Tomas Beranek <xberan46@stud.fit.vutbr.cz>
# Brief   Using EA to find the initial configuration of a Game of Life 2D cellular
#         automaton that leads in 1 to N steps to the desired configuration
# Date    8.5.2022
# Up2date sources can be found at: https://github.com/TomasBeranek/but-bin-project

import matplotlib.pyplot as plt
import sys
import numpy as np
import pandas as pd

def zAxis(X, Y, data):
    Z = []

    for rowX, rowY in zip(X, Y):
        rowZ = []

        for x, y in zip(rowX, rowY):
            z = data.loc[(data['Crossover'] == x) & (data['MaxMutationPercetange'] == y)].iloc[0]['BestFitness']
            rowZ += [z]

        Z += [rowZ]

    return np.array(Z)


if __name__ == '__main__':
    file = sys.argv[1]
    file2 = sys.argv[2]

    df = pd.read_csv(file)
    df2 = pd.read_csv(file2)

    X = list(df2['Generations'])
    X = [str(x) for x in X]
    P = list(df2['Population'])
    P = [str(p) for p in P]
    Y = list(df2['BestFitness'])

    fig, ax = plt.subplots(1, 1)

    for x, y in zip(X, Y):
        plt.annotate(str(y), (x, y))

    ax.plot(X, Y)
    ax.xaxis.set_ticks(range(len(X)))
    X = [f"{x}\n{p}" for x,p in zip(X,P)]

    ax.xaxis.set_ticklabels(X)

    plt.ylabel('Average best fitness')
    plt.xlabel('Generations\nPopulation')
    plt.title('Generations vs population')
    plt.savefig('graphs/generations.png')
    plt.subplots_adjust(bottom=0.2)
    plt.show()

    df['Crossover'] = df['Crossover'].abs()
    df['MaxMutationPercetange'] = df['MaxMutationPercetange'].abs()

    normal_data = df.loc[df['Type'] == 'normal']

    X_list = sorted(normal_data['Crossover'].unique())
    Y_list = sorted(normal_data['MaxMutationPercetange'].unique())
    X_list = list(map(abs, X_list))

    X, Y = np.meshgrid(X_list, Y_list)
    Z = zAxis(X, Y, normal_data)

    ax = plt.axes(projection='3d')
    ax.set_zlim([820, 850])
    ax.set_xlabel('Crossover points')
    ax.set_ylabel('Max mutation percentage')
    ax.set_zlabel('Fitness')
    ax.invert_xaxis()
    ax.set_xticks(X_list)
    ax.set_yticks(Y_list)
    ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap='viridis', edgecolor='none');

    plt.title('Normal operations')
    plt.savefig('graphs/normal.png')
    plt.show()


    spatial_data = df.loc[(df['Type'] == "spatial") & (df['ConcentratedMutation'] == 30)]
    X_list = sorted(spatial_data['Crossover'].unique())
    Y_list = sorted(spatial_data['MaxMutationPercetange'].unique())
    Y_list = list(map(abs, Y_list))

    X, Y = np.meshgrid(X_list, Y_list)
    Z = zAxis(X, Y, spatial_data)

    ax = plt.axes(projection='3d')
    ax.set_zlim([820, 850])
    ax.set_xlabel('Crossover rectangles')
    ax.set_ylabel('Max mutation points')
    ax.set_zlabel('Fitness')
    ax.invert_xaxis()
    ax.set_xticks(X_list)
    ax.set_yticks(Y_list)
    ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap='viridis', edgecolor='none');

    plt.title('Spatial operations (mutation 30 %)')
    plt.savefig('graphs/spatial30.png')
    plt.show()


    spatial_data = df.loc[(df['Type'] == "spatial") & (df['ConcentratedMutation'] == 50)]
    X_list = sorted(spatial_data['Crossover'].unique())
    Y_list = sorted(spatial_data['MaxMutationPercetange'].unique())
    Y_list = list(map(abs, Y_list))

    X, Y = np.meshgrid(X_list, Y_list)
    Z = zAxis(X, Y, spatial_data)

    ax = plt.axes(projection='3d')
    ax.set_zlim([820, 850])
    ax.set_xlabel('Crossover rectangles')
    ax.set_ylabel('Max mutation points')
    ax.set_zlabel('Fitness')
    ax.invert_xaxis()
    ax.set_xticks(X_list)
    ax.set_yticks(Y_list)
    ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap='viridis', edgecolor='none');

    plt.title('Spatial operations (mutation 50 %)')
    plt.savefig('graphs/spatial50.png')
    plt.show()


    spatial_data = df.loc[(df['Type'] == "spatial") & (df['ConcentratedMutation'] == 70)]
    X_list = sorted(spatial_data['Crossover'].unique())
    Y_list = sorted(spatial_data['MaxMutationPercetange'].unique())
    Y_list = list(map(abs, Y_list))

    X, Y = np.meshgrid(X_list, Y_list)
    Z = zAxis(X, Y, spatial_data)

    ax = plt.axes(projection='3d')
    ax.set_zlim([820, 850])
    ax.set_xlabel('Crossover rectangles')
    ax.set_ylabel('Max mutation points')
    ax.set_zlabel('Fitness')
    ax.invert_xaxis()
    ax.set_xticks(X_list)
    ax.set_yticks(Y_list)
    ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap='viridis', edgecolor='none');

    plt.title('Spatial operations (mutation 70 %)')
    plt.savefig('graphs/spatial70.png')
    plt.show()

    # show optimalization graph
    baseline = 243299
    y = [243299, 380090, 358114, 632769, 684318, 1074342, 1085623, 1110159, 1114071, 9690340]
    y = [sol / baseline for sol in y]
    x = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

    for xp, yp in zip(x, y):
        plt.annotate('%.2fx' % yp, (xp - 0.3, yp + 0.5))

    plt.plot(x, y)
    plt.xticks(x)
    plt.ylabel('Speed up')
    plt.xlabel('Phase')
    plt.title('Optimization progress')
    plt.savefig('graphs/optimization_progress.png')
    plt.show()
