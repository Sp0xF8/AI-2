#pragma once





// #define FIND_BEST



#define ELITEISM
// #define HEAVY_ELITEISM
// #define SIMPLE_PASSDOWN




#define RANDOM_SEED 1699118515

#define POPULATION_SIZE 50
#define NUMBER_OF_GENES 10
#define NUMBER_OF_GENERATIONS 100






#define GENE_HEIGHT 1
#define GENE_BASE 0



#define TOURNAMENT_SIZE 3


#ifdef FIND_BEST

    #define MUTATION_RATE 0.03
    #define MUTATION_HEIGHT 0.1
#else

    #define MUTATION_RATE 0.03
    #define MUTATION_HEIGHT 0.2
#endif






// #define _DEBUG_GENES
// #define _DEBUG_FITNESS
// #define _DEBUG_INDIVIDUAL
// #define _DEBUG_GENERATION

// #define _DEBUG_POPULATION
// #define _DEBUG_GLADIATOR
// #define _DEBUG_CROSSPOINT
// #define _DEBUG_MUTATION


#define _PLOT_GRAPHS

#define _GRAPH_POPULATION
#define _GRAPH_GLADIATOR
#define _GRAPH_CROSSPOINT
#define _GRAPH_MUTATION

// #define _GRAPH_FITNESS
// #define _GRAPH_FITNESS_AVERAGE


