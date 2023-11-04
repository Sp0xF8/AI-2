#pragma once

#include <defines.h>

class Individual {
    //define private float vars for fitness and array of genes
    private:
        float fitness;
        float genes[NUMBER_OF_GENES];

    //define public functions
    public:

        //constructor
        Individual();
        // Individual(float genes[]);

        //destructor
        ~Individual();

        //getters
        float getFitness();
        float* getGenes();

        //setters
        void setFitness(float fitness);
        void setGenes(float genes[]);
        void setGene(int index, float value);

        //other functions
        
};