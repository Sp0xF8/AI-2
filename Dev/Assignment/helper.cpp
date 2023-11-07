#include <helper.h>
#include <defines.h>

#include <iostream>

namespace Helper {
    
        #ifdef FIND_BEST
            float best_fitnesses[NUMBER_OF_GENERATIONS];
        #else
            float worst_fitnesses[NUMBER_OF_GENERATIONS];
        #endif
    
        float average_fitnesses[NUMBER_OF_GENERATIONS];

        Generation generations[NUMBER_OF_GENERATIONS];
}


void Helper::generateGenes(Individual *individual) {

    
    float temp_genes[NUMBER_OF_GENES];

    //generate random genes
    for (int i = 0; i < NUMBER_OF_GENES; i++) {
        temp_genes[i] = static_cast<float>(std::rand()) / RAND_MAX;
    }

    //set genes
    individual->setGenes(temp_genes);
}

void Helper::calculateFitness(Individual *individual) {
    //calculate fitness aka test function
    float fitness = 0;
    float *genes = individual->getGenes();


    for (int i = 0; i < NUMBER_OF_GENES; i++) {
        fitness += genes[i];
    }
    individual->setFitness(fitness);
}


void Helper::printGenes(Individual *individual) {
    //print genes

    float *genes = individual->getGenes();

    std::cout << "Genes: ";

    for (int i = 0; i < NUMBER_OF_GENES; i++) {
        std::cout << genes[i] << " ";
    }
    std::cout << ";" << std::endl;
}

void Helper::printFitness(Individual *individual) {
    //print fitness
    float fitness = individual->getFitness();
    std::cout << "Fitness: " << fitness << std::endl;

}

void Helper::printIndividual(Individual *individual) {

    std::cout << "\n\nIndividual: " << std::endl;
    //print genes and fitness
    printGenes(individual);
    printFitness(individual);

}


void Helper::printPopulation(void* population[]) {

    std::cout << "Population: " << std::endl;
    //print population
    for (int i = 0; i < POPULATION_SIZE; i++) {
        Individual *individual = (Individual*)population[i];
        printIndividual(individual);
    }
}


float Helper::getPopulationFitness(void* population[]) {
    //calculate fitness of population
    float fitness = 0;

    for (int i = 0; i < POPULATION_SIZE; i++) {
        Individual *individual = (Individual*)population[i];
        fitness += individual->getFitness();
    }

    return fitness;
}

float Helper::getPopulationHeight(void* population[]) {
    //calculate fitness of population
    float height;
    Individual *individual = (Individual*)population[0];

    height = individual->getFitness();

    #ifdef FIND_BEST

        for (int i = 1; i < POPULATION_SIZE; i++) {
            Individual *individual = (Individual*)population[i];
            if (individual->getFitness() > height) {
                height = individual->getFitness();
            }
            
        }

    #else
    
            for (int i = 1; i < POPULATION_SIZE; i++) {
                Individual *individual = (Individual*)population[i];
                if (individual->getFitness() < height) {
                    height = individual->getFitness();
                }
                
            }
    #endif

    return height;
}
