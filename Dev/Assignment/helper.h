#pragma once

#include <individual.h>


struct Stats {
    #ifdef FIND_BEST
        float best;
    #else
        float worst;
    #endif
    float average;
    float total;
};

struct Generation {
    Stats population;
    Stats gladiator;
    Stats crosspoint;
    Stats mutation;
};


namespace Helper {

    #ifdef FIND_BEST
        extern float best_fitnesses[NUMBER_OF_GENERATIONS];
    #else
        extern float worst_fitnesses[NUMBER_OF_GENERATIONS];
    #endif

    extern float average_fitnesses[NUMBER_OF_GENERATIONS];
    extern Generation generations[NUMBER_OF_GENERATIONS];

    void generateGenes(Individual *individual);
    void calculateFitness(Individual *individual);
    void printGenes(Individual *individual);
    void printFitness(Individual *individual);
    void printIndividual(Individual *individual);

    void printPopulation(void* population[]);
    float getPopulationFitness(void* population[]);
    float getPopulationHeight(void* population[]);

}
