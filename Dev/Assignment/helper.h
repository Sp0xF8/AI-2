#pragma once

#include <individual.h>


namespace Helper {

    void generateGenes(Individual *individual);
    void calculateFitness(Individual *individual);
    void printGenes(Individual *individual);
    void printFitness(Individual *individual);
    void printIndividual(Individual *individual);

    void printPopulation(void* population[]);
    float getPopulationFitness(void* population[]);
    float getPopulationHeight(void* population[]);

}
