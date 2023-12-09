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

        std::vector<float> fitnesses;
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

		float temp = 0;

		for (int i = 1; i < NUMBER_OF_GENES; i++) {

			fitness += i * (pow(((2 * pow(genes[i], 2)) - genes[i-1]), 2));
		}

		fitness += pow((genes[0]-1), 2);

	#endif

	#ifdef COMPLEX_TEST

		float temp = 0, temp2 = 0;

		for (int i = 0; i < NUMBER_OF_GENES; i++) {
			temp += pow(genes[i], 2);
			temp2 += 0.5 * (i+1) * genes[i];
		}

		fitness = temp + pow(temp2, 2) + pow(temp2, 4);

	#endif

    #ifdef TRID_FUNCTION

    
    
        float temp = 0, temp2 = 0;

        for (int i = 0; i < NUMBER_OF_GENES; i++) {
            temp += pow(genes[i] - 1, 2);
        }

        for (int i = 1; i < NUMBER_OF_GENES; i++) {
            temp2 += genes[i] * genes[i-1];
        }

        fitness = temp - temp2;

    

        // for (int i = 0; i < NUMBER_OF_GENES; i++) {
        //     fitness += i * pow(genes[i], 2);
        // }
        
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


MetaData* Helper::Top10Gens(){

	MetaData* top10 = new MetaData[NUMBER_OF_RESULTS];

	for (int i = 0; i < NUMBER_OF_RESULTS; i++) {
		top10[i] = meta_data[i];
	}

	// find the best 10 candidates from meta_data and add to top10
	for (int i = NUMBER_OF_RESULTS - 1; i < meta_data.size(); i++) {

        if (meta_data[i].ending_fitness_height == 0) {
            continue;
        }
		for (int j = 0; j < NUMBER_OF_RESULTS; j++) {

			#ifdef FIND_BEST
				if (meta_data[i].ending_fitness_height > top10[j].ending_fitness_height) {
					top10[j] = meta_data[i];
					break;
				}
			#else
				if (meta_data[i].ending_fitness_height < top10[j].ending_fitness_height) {
					top10[j] = meta_data[i];
					break;
				}
			#endif
		}
	}

	return top10;
}

float Helper::GetSolutionFitness(MetaData meta){

	float solution_fitness = 0;

	int gen_loops, glad_loops, cross_loops, mut_loops;

	// if(meta.plato_confirmed != 0){
	// 	gen_loops = meta.plato_confirmed * POPULATION_SIZE;
	// }
	// else{
	// 	gen_loops = NUMBER_OF_GENERATIONS * POPULATION_SIZE;
	// }

	// glad_loops = gen_loops * (TOURNAMENT_SIZE * TOURNAMENT_SIZE - 1);

	// cross_loops = (gen_loops * (POPULATION_SIZE / 2)) * NUMBER_OF_GENES;

	// mut_loops = (gen_loops * POPULATION_SIZE) * NUMBER_OF_GENES;

	// solution_fitness = (gen_loops + glad_loops + cross_loops + mut_loops) * meta.ending_fitness_height;
	

	if(meta.plato_confirmed != 0){
		gen_loops = meta.plato_confirmed;
	}	else	{
		gen_loops = NUMBER_OF_GENERATIONS;
	}

	glad_loops = (TOURNAMENT_SIZE * TOURNAMENT_SIZE) * POPULATION_SIZE;

	cross_loops = (POPULATION_SIZE / 2) * NUMBER_OF_GENES;

	mut_loops = POPULATION_SIZE * NUMBER_OF_GENES;

	solution_fitness = (gen_loops * (glad_loops + cross_loops + mut_loops)) * meta.ending_fitness_height;//+ 0.0000001  sqrt(pow(meta.ending_fitness_height, 2))

	return solution_fitness;
}

MetaData* Helper::GetTopSolutions(){

	MetaData* top10sols = new MetaData[NUMBER_OF_RESULTS];

	for (int i = 0; i < NUMBER_OF_RESULTS; i++) {
		top10sols[i] = meta_data[i];
	}

	// find the best 10 candidates from meta_data and add to top10
	for (int i = NUMBER_OF_RESULTS - 1; i < meta_data.size(); i++) {
        if(meta_data[i].solution_fitness == 0){
            continue;
        }
		for (int j = 0; j < NUMBER_OF_RESULTS; j++) {
 
			#ifdef FIND_BEST
				if (meta_data[i].solution_fitness > top10sols[j].solution_fitness) {
					top10sols[j] = meta_data[i];
					break;
				}
			#else
				if (meta_data[i].solution_fitness < top10sols[j].solution_fitness) {
					top10sols[j] = meta_data[i];
					break;
				}
			#endif
		}
	}

	return top10sols;
}
