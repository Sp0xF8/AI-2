#pragma once

#include <defines.h>

class Individual {
    private:
        float fitness;
        float genes[NUMBER_OF_GENES];

    public:

        //constructor
        Individual();

        //destructor
        ~Individual();

        //getters
        float getFitness();
        float* getGenes();

        //setters
        void setFitness(float fitness);
        void setGenes(float genes[]);
        void setGene(int index, float value);
};