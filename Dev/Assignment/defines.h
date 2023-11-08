#pragma once




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   SETTINGS FOR FINDING THE BEST OR WORST INDIVIDUAL
//     - FIND_BEST : FINDS THE BEST INDIVIDUAL
//     
//     OTHERWISE THE PROGRAM WILL FIND THE WORST INDIVIDUAL BY DEFAULT
//
///////////////////

#define FIND_BEST

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   SELECT A SPECIFIC TEST FUNCTION
//     ONLY ONE OF THE FOLLOWING CAN BE DEFINED AT A TIME
//
//     - SIMPLE_TEST : SIMPLE TEST FUNCTION
//     - ADVANCED_TEST : ADVANCED TEST FUNCTION [MINIMISATION AND MAXIMISATION]
//
///////////////////

// #define SIMPLE_TEST

#define ADVANCED_TEST

// #define COMPLEX_TEST


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ONLY COMMENT ONE OF THE FOLLOWING:
//      - ELITEISM - HEAVY_ELITEISM - SIMPLE_PASSDOWN
//          OTHERWISE THE PROGRAM WILL CRASH
//      
//      ELITEISM : CHECKS IF ANY PROGRESSION WAS MADE FROM THE PREVIOUS GENERATION
//      HEAVY_ELITEISM : FORCES THE BEST OF ALL 4 POPULATIONS INTO THE NEXT GENERATION
//      SIMPLE_PASSDOWN : FORCES THE MUTATED POPULATION ONTO THE NEXT GENERATION
//
///////////////////

// #define ELITEISM
#define HEAVY_ELITEISM
// #define SIMPLE_PASSDOWN



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    RANDOM_SEED : SEED FOR RANDOM NUMBER GENERATOR - 
//      RAMDOM SEED IS OUTPUTTED TO THE CONSOLE WHEN THE SEED == 0
//
///////////////////
#define RANDOM_SEED 1699118515

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   POPULATION_SIZE : NUMBER OF INDIVIDUALS IN A POPULATION
//   NUMBER_OF_GENES : NUMBER OF GENES IN AN INDIVIDUAL
//   NUMBER_OF_GENERATIONS : NUMBER OF GENERATIONS TO RUN THE PROGRAM FOR
//
///////////////////

#define POPULATION_SIZE 50
#define NUMBER_OF_GENES 10
#define NUMBER_OF_GENERATIONS 100


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   GENE_BASE : BASE OF THE GENE
//   GENE_HEIGHT : HEIGHT OF THE GENE
//
///////////////////

#define GENE_BASE 0
#define GENE_HEIGHT 1


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   TOURNAMENT_SIZE : NUMBER OF INDIVIDUALS IN A GLADIATOR TOURNAMENT
//
///////////////////
#ifdef FIND_BEST

    #define TOURNAMENT_SIZE 3

#else

    #define TOURNAMENT_SIZE 2

#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   MUTATION_RATE : THE PROBABILITY OF A MUTATION
//   MUTATION_HEIGHT : THE UPPER AND LOWER BOUND OF THE MUTATION
//
//      DIFFERENT SETTINGS ARE USED FOR FINDING BEST OR WORST
//
///////////////////


#ifdef FIND_BEST

    #define MUTATION_RATE 0.28
    #define MUTATION_HEIGHT 0.02
#else

    #define MUTATION_RATE 0.11
    #define MUTATION_HEIGHT 0.28
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   DIFFERENT DEBUGGING SETTINGS
//
//   _DEBUG_GENES : PRINTS THE GENES OF AN INDIVIDUAL
//   _DEBUG_FITNESS : PRINTS THE FITNESS OF AN INDIVIDUAL
//   _DEBUG_INDIVIDUAL : PRINTS THE GENES AND FITNESS OF AN INDIVIDUAL
//   _DEBUG_GENERATION : PRINTS INFORMATION ABOUT THE CURRENT GENERATIONS PROGRESSION
//
//   _DEBUG_POPULATION : PRINTS THE GENES AND FITNESS OF A POPULATION
//   _DEBUG_GLADIATOR : PRINTS THE GENES AND FITNESS OF A GLADIATOR
//   _DEBUG_CROSSPOINT : PRINTS THE GENES AND FITNESS OF A CROSSPOINT
//   _DEBUG_MUTATION : PRINTS THE GENES AND FITNESS OF A MUTATION
//
///////////////////



// #define _DEBUG_GENES
// #define _DEBUG_FITNESS
// #define _DEBUG_INDIVIDUAL
// #define _DEBUG_GENERATION

// #define _DEBUG_POPULATION
// #define _DEBUG_GLADIATOR
// #define _DEBUG_CROSSPOINT
// #define _DEBUG_MUTATION



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   DIFFERENT GRAPHING SETTINGS
//
//   _PLOT_GRAPHS : THIS IS REQUIRED FOR ANY OF THE FOLLOWING TO WORK
//        IF THIS IS NOT DEFINED, THE PROGRAM WILL NOT CALL THE FUNCTIONS TO PLOT DATA
//
//   _GRAPH_POPULATION : PLOTS THE FITNESS OF THE POPULATION
//   _GRAPH_GLADIATOR : PLOTS THE FITNESS OF THE GLADIATOR
//   _GRAPH_CROSSPOINT : PLOTS THE FITNESS OF THE CROSSPOINT
//   _GRAPH_MUTATION : PLOTS THE FITNESS OF THE MUTATION
//
///////////////////


#define _PLOT_GRAPHS

#ifdef _PLOT_GRAPHS

    #define _GRAPH_POPULATION
    #define _GRAPH_GLADIATOR
    #define _GRAPH_CROSSPOINT
    #define _GRAPH_MUTATION
    #define _GRAPH_GENERATION_HISTORY

#endif

    // #define _GRAPH_FITNESS
    // #define _GRAPH_FITNESS_AVERAGE



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   TOGGLE META AI
//
//   _META_AI : TOGGLES META AI
//
///////////////////

// #define _META_AI