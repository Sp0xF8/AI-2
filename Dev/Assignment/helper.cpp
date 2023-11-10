#include <helper.h>
#include <defines.h>

#include <cmath>

#include <iostream>

#include <time.h>

namespace Helper {
    
        #ifdef FIND_BEST
            float best_fitnesses[NUMBER_OF_GENERATIONS];
        #else
            float worst_fitnesses[NUMBER_OF_GENERATIONS];
        #endif
    
        float average_fitnesses[NUMBER_OF_GENERATIONS];

        Generation generations[NUMBER_OF_GENERATIONS];

        GenerationHistory generation_history[NUMBER_OF_GENERATIONS];
        int generation_index = 0;
        int generation_type_index = 0;

        bool cycle = false;
        int cycle_speed = 100;

        int cycle_timer = 0;

        std::vector<MetaData> meta_data;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 FUNCTION TO GENERATE THE GENES OF AN INDIVIDUAL
//      TAKES ARGUMENTS:
//          individual : THE INDIVIDUAL TO GENERATE THE GENES OF
//
//
//      CYCLES THROUGH EACH GENE IN THE INDIVIDUAL
//          GENERATES A RANDOM FLOAT BETWEEN 0 AND 1
//          SETS THE GENE TO THE RANDOM FLOAT
//
//      RETURNS:
//          NONE
//
///////////////////

void Helper::generateGenes(Individual *individual) {

    
    float temp_genes[NUMBER_OF_GENES];

    for (int i = 0; i < NUMBER_OF_GENES; i++) {
        temp_genes[i] = static_cast<float>(std::rand()) / RAND_MAX;
    }

    individual->setGenes(temp_genes);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 FUNCTION TO CALCULATE THE FITNESS OF AN INDIVIDUAL
//      TAKES ARGUMENTS:
//          individual : THE INDIVIDUAL TO CALCULATE THE FITNESS OF
//
//
//      CYCLES THROUGH EACH GENE IN THE INDIVIDUAL
//          ADDS THE GENE TO THE FITNESS
//
//      RETURNS:
//          NONE
//
///////////////////

void Helper::calculateFitness(Individual *individual) {
    //calculate fitness aka test function
    float fitness = 0;
    float *genes = individual->getGenes();

    #ifdef SIMPLE_TEST

        for (int i = 0; i < NUMBER_OF_GENES; i++) {
            fitness += genes[i];
        }

    #endif

    #ifdef ADVANCED_TEST

        float temp;

        for (int i = 1; i < NUMBER_OF_GENES; i++) {

            //first minimisation problem

            temp += pow((pow(( 2 * genes[i] ), 2) - ( genes[i-1])), 2);

        }

        fitness = temp;

        fitness += pow((genes[0] - 1), 2);

    #endif

    #ifdef COMPLEX_TEST

        float temp, midpoint;


        for(int k = 1; k < NUMBER_OF_GENES; k++){

            midpoint += (0.5 * k) * genes[i];
        }

        temp = pow(midpoint, 2) + pow(midpoint, 4);

        for (int k = 1; k < NUMBER_OF_GENES; k++){
            fitness += pow(genes[k], 2) + temp;
        }



    #endif



    individual->setFitness(fitness);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 FUNCTION TO PRINT THE GENES OF AN INDIVIDUAL
//      TAKES ARGUMENTS:
//          individual : THE INDIVIDUAL TO PRINT THE GENES OF
//
//
//      CYCLES THROUGH EACH GENE IN THE INDIVIDUAL
//          PRINTS THE GENE
//
//      RETURNS:
//          NONE
//
///////////////////

void Helper::printGenes(Individual *individual) {

    float *genes = individual->getGenes();

    std::cout << "Genes: ";

    for (int i = 0; i < NUMBER_OF_GENES; i++) {
        std::cout << genes[i] << " ";
    }
    std::cout << ";" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 FUNCTION TO PRINT THE FITNESS OF AN INDIVIDUAL
//      TAKES ARGUMENTS:
//          individual : THE INDIVIDUAL TO PRINT THE FITNESS OF
//
//
//      PRINTS THE FITNESS OF THE INDIVIDUAL
//
//      RETURNS:
//          NONE
//
///////////////////

void Helper::printFitness(Individual *individual) {
    //print fitness
    float fitness = individual->getFitness();
    std::cout << "Fitness: " << fitness << std::endl;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 FUNCTION TO PRINT AN INDIVIDUAL
//      TAKES ARGUMENTS:
//          individual : THE INDIVIDUAL TO PRINT
//
//
//      PRINTS THE GENES AND FITNESS OF THE INDIVIDUAL
//
//      RETURNS:
//          NONE
//
///////////////////

void Helper::printIndividual(Individual *individual) {

    std::cout << "\n\nIndividual: " << std::endl;
    //print genes and fitness
    printGenes(individual);
    printFitness(individual);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 FUNCTION TO PRINT A POPULATION
//      TAKES ARGUMENTS:
//          population : THE POPULATION TO PRINT
//
//
//      CYCLES THROUGH EACH INDIVIDUAL IN THE POPULATION
//          PRINTS THE INDIVIDUAL
//
//      RETURNS:
//          NONE
//
///////////////////

void Helper::printPopulation(void* population[]) {

    std::cout << "Population: " << std::endl;
    //print population
    for (int i = 0; i < POPULATION_SIZE; i++) {
        Individual *individual = (Individual*)population[i];
        printIndividual(individual);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 FUNCTION TO GET THE FITNESS OF A POPULATION
//      TAKES ARGUMENTS:
//          population : THE POPULATION TO GET THE FITNESS OF
//
//
//      CYCLES THROUGH EACH INDIVIDUAL IN THE POPULATION
//          GETS THE FITNESS OF EACH INDIVIDUAL
//          ADDS THE FITNESS OF EACH INDIVIDUAL TO THE FITNESS OF THE POPULATION
//
//      RETURNS:
//          fitness : THE FITNESS OF THE POPULATION
//
///////////////////

float Helper::getPopulationFitness(void* population[]) {
    //calculate fitness of population
    float fitness = 0;

    for (int i = 0; i < POPULATION_SIZE; i++) {
        Individual *individual = (Individual*)population[i];
        fitness += individual->getFitness();
    }

    return fitness;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 FUNCTION TO GET THE HEIGHT OF A POPULATION
//      TAKES ARGUMENTS:
//          population : THE POPULATION TO GET THE HEIGHT OF
//
//
//      CYCLES THROUGH EACH INDIVIDUAL IN THE POPULATION 
//          GETS THE FITNESS OF EACH INDIVIDUAL
//          
//          IF FINDING BEST
//              IF THE FITNESS IS GREATER THAN THE CURRENT HEIGHT
//                  SET THE HEIGHT TO THE FITNESS
//          IF FINDING WORST
//              IF THE FITNESS IS LESS THAN THE CURRENT HEIGHT
//                  SET THE HEIGHT TO THE FITNESS
//
//      RETURNS:
//          height : THE HEIGHT OF THE POPULATION
//
///////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 SIMPLE FUNCTION TO CLEAR A POPULATION
//      TAKES ARGUMENTS:
//          population : THE POPULATION TO CLEAR
//
//
//      CYCLES THROUGH EACH INDIVIDUAL IN THE POPULATION AND DELETES IT
//          THEN SETS THE POINTER TO NULL
//
//      RETURNS:
//          true : IF THE POPULATION WAS CLEARED
//          false : IF THE POPULATION WAS NOT CLEARED
///////////////////

bool Helper::ClearPopulation(void* population[]){

    for (int i = 0; i < POPULATION_SIZE; i++) {
        delete (Individual*)population[i];
        population[i] = NULL;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 SIMPLE FUNCTION TO COPY A POPULATION TO ANOTHER POPULATION
//      TAKES ARGUMENTS:
//          from : THE POPULATION TO COPY FROM
//          to : THE POPULATION TO COPY TO
//
//
//      COPIES THE POPULATION FROM ONE POPULATION TO ANOTHER BY CYCLING THROUGH EACH INDIVIDUAL
//          AND COPYING THE GENES AND FITNESS OF EACH INDIVIDUAL TO A NEW INSTANCE OF AN INDIVIDUAL
//          AND ADDING A POINTER TO IT TO THE NEW POPULATION ARRAY
//
//      RETURNS:
//          true : IF THE POPULATION WAS COPIED
//          false : IF THE POPULATION WAS NOT COPIED
//
///////////////////

bool Helper::CopyPopulation(void* from[], void* to[]){

    
    
    for (int i = 0; i < POPULATION_SIZE; i++) {
        Individual *individual = new Individual();
        individual->setGenes(((Individual*)from[i])->getGenes());
        individual->setFitness(((Individual*)from[i])->getFitness());
        to[i] = individual;
    }

    return true;
}




