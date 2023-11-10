#pragma once

#include <individual.h>

#include <vector>


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

struct GenerationHistory {
    void* population[POPULATION_SIZE];
    void* gladiators[POPULATION_SIZE];
    void* crosspoints[POPULATION_SIZE];
    void* mutations[POPULATION_SIZE];

};


struct MetaData {
    float mutation_rate;
    float mutation_height;
    float tournament_size;

    float ending_fitness_height;

    #ifdef PLATO_HEIGHT
        int plato_confirmed;
    #endif
};


namespace Helper {

    #ifdef FIND_BEST
        extern float best_fitnesses[NUMBER_OF_GENERATIONS];
    #else
        extern float worst_fitnesses[NUMBER_OF_GENERATIONS];
    #endif

    extern float average_fitnesses[NUMBER_OF_GENERATIONS];


    extern Generation generations[NUMBER_OF_GENERATIONS];

    extern GenerationHistory generation_history[NUMBER_OF_GENERATIONS];
    extern int generation_index;
    extern int generation_type_index;

    extern bool cycle;
    extern int cycle_speed;
    extern int cycle_timer;

    extern std::vector<MetaData> meta_data;

    void generateGenes(Individual *individual);
    void calculateFitness(Individual *individual);
    void printGenes(Individual *individual);
    void printFitness(Individual *individual);
    void printIndividual(Individual *individual);

    void printPopulation(void* population[]);
    float getPopulationFitness(void* population[]);
    float getPopulationHeight(void* population[]);

    bool ClearPopulation(void* population[]);
	
	bool CopyPopulation(void* from[], void* to[]);

}
