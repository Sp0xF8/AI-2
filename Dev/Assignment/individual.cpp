#include <individual.h>

#include <stdio.h>


Individual::Individual() {
    #ifdef _DEBUG_INDIVIDUAL
        printf("Individual constructor\n");
    #endif
}


Individual::~Individual() {
    #ifdef _DEBUG_INDIVIDUAL
        printf("Individual destructor\n");
    #endif

    // delete this;
}


float Individual::getFitness() {
    //return fitness
    return this->fitness;
}

float* Individual::getGenes() {
    //return genes
    return this->genes;
}


void Individual::setFitness(float fitness) {
    //set fitness
    this->fitness = fitness;
}


void Individual::setGenes(float genes[]) {
    //set genes
    for (int i = 0; i < NUMBER_OF_GENES; i++) {
        this->genes[i] = genes[i];
    }
}

void Individual::setGene(int index, float value) {
    //set gene at index
    this->genes[index] = value;
}





