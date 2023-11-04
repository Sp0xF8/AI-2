import random
import numpy as np
import matplotlib.pyplot as plt
import copy
import math


N = 50 #number of genes
P = 50 #population size
G = 250 #number of generations
M = 0.03 #mutation rate #0.03 for high 0.33 for low


MIN_GENE = 0.0
MAX_GENE = 1.0
MUTESTEP = 0.1



population = []
offspring = []

crossover = []
mutate = []


total_utilities = {
    "origonal": 0.0,
    "gladiator": 0.0,
    "crosspoint": 0.0,
    "mutate": 0.0
}


best_fitness = np.zeros(G)
average_fitness = np.zeros(G)


class individual:
    def __init__(self):
        self.gene = [0]*N
        self.fitness = 0 


def test_function( ind ):
    utility= 0.0
    for i in range(N):

        minimisation = pow(ind.gene[i], 2) - (10 * math.cos((2*3.1415) * ind.gene[i]))
        utility = utility + minimisation



    return (N*10 + utility)

def test_all(pop):
    total_utility = 0
    for i in pop:
        total_utility = total_utility + test_function(i)

    average_utility = total_utility/len(pop)
    
    return average_utility


def graph_population(pop, name):
    data = np.array([])
    
    for po in pop:
        data = np.append(data, po.fitness)


    plt.plot(data)
    plt.title(name)
    plt.xlabel("Individual")
    plt.ylabel("Fitness")
    plt.show()
    plt.close()


def graph_averages():
    height = np.array([])
    average = np.array([])
    
    for i in best_fitness:
        height = np.append(height, i)
    
    for i in average_fitness:
        average = np.append(average, i)


    plt.plot(height, label="Best")
    plt.plot(average, label="Average")
    plt.legend()
    plt.title("Generational Fitness")


    plt.xlabel("Generation")
    plt.ylabel("Fitness")
    plt.show()
    plt.close()

for x in range (0, P):
    tempgene=[]
    for y in range (0, N):
        tempgene.append( random.uniform(MIN_GENE, MAX_GENE) )
    newind = individual()
    newind.gene = tempgene.copy()
    population.append(newind)


def get_height(pop):
    height = pop[0].fitness
    for i in pop:
        if i.fitness < height:
            height = i.fitness

    return height



def test_population(population, name):
    for pop in population:
        pop.fitness = test_function(pop)
        # print(pop.gene, " ", pop.fitness)
        total_utilities[name] = total_utilities[name] + pop.fitness




for g in range (0, G):



    print("Generation ", g)

## Test origonal population

    test_population(population, "origonal")
    
    
    
    



## init gladiator population
    
    for i in range (0, P):
        parent1 = random.randint( 0, P-1 )
        off1 = copy.deepcopy(population[parent1])
        parent2 = random.randint( 0, P-1 )
        off2 = copy.deepcopy(population[parent2])
        if off1.fitness < off2.fitness:
            offspring.append( off1 )
        else:
            offspring.append( off2 )

    ## test gladiator population
    
    test_population(offspring, "gladiator")



## create new copies to allow for easy transfer of data

    crossover = copy.deepcopy(offspring) 
    # mutate = copy.deepcopy(offspring)

## init crossover population

    toff1 = individual()
    toff2 = individual()
    temp = individual()
    for i in range( 0, P, 2 ):
        toff1 = copy.deepcopy(crossover[i])
        toff2 = copy.deepcopy(crossover[i+1])
        temp = copy.deepcopy(crossover[i])
        crosspoint = random.randint(1,N)

        for j in range (crosspoint, N):
            toff1.gene[j] = toff2.gene[j]
            toff2.gene[j] = temp.gene[j]
            
        crossover[i] = copy.deepcopy(toff1)
        crossover[i+1] = copy.deepcopy(toff2)

    ## test crossover population

    test_population(crossover, "crosspoint")




## init mutate population

    for i in range( 0, P ):
        newind = individual()
        newind.gene = []
        for j in range( 0, N ):
            gene = crossover[i].gene[j]
            mutprob = random.random()
            # print("muteprob: ", mutprob)
            if mutprob < M:
                alter = random.uniform(-MUTESTEP,MUTESTEP)
                gene = gene + alter
                if gene > MAX_GENE:
                    gene = MAX_GENE
                if gene < MIN_GENE:
                    gene = MIN_GENE

            newind.gene.append(gene)
            
        mutate.append(newind)

    ## test mutate population

    test_population(mutate, "mutate")

    


 ## calculate average fitness

    # test_population(population, "origonal")

    average_fitness[g] = total_utilities["origonal"] / P
    best_fitness[g] = get_height(population)



## DEBUGGING OPTIONS

    # graph_population(population, "Origonal")
    # graph_population(offspring, "Offspring Gladiator")
    # graph_population(crossover, "Offspring Crossover")
    # graph_population(mutate, "Offspring Mutation")

    print("Best fitness: ", best_fitness[g-1])
    print("Average fitness: ", average_fitness[g-1])
    print("Total utilities: ", total_utilities)

## copy most fit population to next generation


    # if (total_utilities["gladiator"] > total_utilities["crosspoint"]):
    #     if (total_utilities["gladiator"] > total_utilities["mutate"]):

    #         if (total_utilities["origonal"] > total_utilities["gladiator"]):
    #             population = copy.deepcopy(population)
    #             print("Origonal wins")
    #         else:
    #             population = copy.deepcopy(offspring)
    #             print("Gladiator wins")
    #     else:

    #         if (total_utilities["origonal"] > total_utilities["mutate"]):
    #             population = copy.deepcopy(population)
    #             print("Origonal wins")
    #         else:
    #             population = copy.deepcopy(mutate)
    #             print("Mutate wins")
    # else:
    #     if (total_utilities["crosspoint"] > total_utilities["mutate"]):
    #         if(total_utilities["origonal"] > total_utilities["crosspoint"]):
    #             population = copy.deepcopy(population)
    #             print("Origonal wins")
    #         else:
    #             population = copy.deepcopy(crossover)
    #             print("Crossover wins")
    #     else:
    #         if(total_utilities["origonal"] > total_utilities["mutate"]):
    #             population = copy.deepcopy(population)
    #             print("Origonal wins")
    #         else:
    #             population = copy.deepcopy(mutate)
    #             print("Mutate wins")



    if (total_utilities["mutate"] < total_utilities["origonal"]):
        population = copy.deepcopy(mutate)
        print("Mutate wins")
 
    ## calculate average fitness
    best_fitness[g] = get_height(population)
    average_fitness[g] = test_all(population)

   
    ## reset total utilities

    total_utilities = {
        "origonal": 0,
        "gladiator": 0,
        "crosspoint": 0,
        "mutate": 0
    }

    ## reset populations

    offspring = []
    crossover = []
    mutate = []



## graph average fitness

print("Best fitness: ", best_fitness)
print("Average fitness: ", average_fitness)
graph_averages()
    