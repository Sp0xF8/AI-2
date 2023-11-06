#include <individual.h>
#include <helper.h>
#include <assignment.h>

#include <iostream>


float fitnesses[4];

void* population[POPULATION_SIZE];
void* gladiators[POPULATION_SIZE];
void* crosspoints[POPULATION_SIZE];
void* mutations[POPULATION_SIZE];

#ifdef FIND_BEST
    float best_fitnesses[NUMBER_OF_GENERATIONS];
#else
    float worst_fitnesses[NUMBER_OF_GENERATIONS];
#endif

float average_fitnesses[NUMBER_OF_GENERATIONS];


bool GeneratePopulatin(){

    for (int i = 0; i < POPULATION_SIZE; i++) {
        Individual *individual = new Individual();
        Helper::generateGenes(individual);              // Populate initial population
        population[i] = individual;

        Helper::calculateFitness(individual);           // Calculate fitness of each individual
    }

    return true;

}


bool GladiatorSelection(){


    for (int i = 0; i < POPULATION_SIZE; i++) {
        Individual selection[TOURNAMENT_SIZE];
        selection[0] = *(Individual*)population[i];
        
        for (int j = 1; j < TOURNAMENT_SIZE; j++) {
            int random_index = rand() % POPULATION_SIZE;
            selection[j] = *(Individual*)population[random_index];
            // Helper::printIndividual(&selection[j]);
        }


        #ifdef FIND_BEST
            //find best individual from selection
            Individual best = selection[0];
            for (int j = 1; j < TOURNAMENT_SIZE; j++) {
                if(selection[j].getFitness() > best.getFitness()){
                    best = selection[j];
                }
            }
        #else
            //find worst individual from selection
            Individual best = selection[0];
            for (int j = 1; j < TOURNAMENT_SIZE; j++) {
                if(selection[j].getFitness() < best.getFitness()){
                    best = selection[j];
                }
            }
        #endif

        //clone best individual to gladiators
        Individual *gladiator = new Individual();
        gladiator->setGenes(best.getGenes());
        gladiator->setFitness(best.getFitness());
        gladiators[i] = gladiator;

        
    

    }

    return true;
}


bool Crosspoint(){
    
        for (int i = 0; i < POPULATION_SIZE; i += 2) {
            Individual *individual1 = new Individual();
            Individual *individual2 = new Individual();

            //get 2 random gladiators
            int random_index1 = rand() % POPULATION_SIZE;
            int random_index2 = rand() % POPULATION_SIZE;

            //get genes from gladiators
            float *genes1 = ((Individual*)gladiators[random_index1])->getGenes();
            float *genes2 = ((Individual*)gladiators[random_index2])->getGenes();

            //get random crosspoint
            int random_crosspoint = rand() % NUMBER_OF_GENES;

            //set genes for new individuals

            float temp_gene;
            
            for (int j = random_crosspoint; j < NUMBER_OF_GENES; j++) {
                temp_gene = genes1[j];
                genes1[j] = genes2[j];
                genes2[j] = temp_gene;
            }

            individual1->setGenes(genes1);
            individual2->setGenes(genes2);

            //calculate fitness
            Helper::calculateFitness(individual1);
            Helper::calculateFitness(individual2);

            //add to crosspoint
            crosspoints[i] = individual1;
            crosspoints[i + 1] = individual2;
        }
    
        return true;
    
    
}


bool Mutation(){

    for (int i = 0; i < POPULATION_SIZE; i++) {
        float* genes = ((Individual*)crosspoints[i])->getGenes();

        for (int j = 0; j < NUMBER_OF_GENES; j++) {
            float random = (float)rand() / RAND_MAX;
            if(random < MUTATION_RATE){
                printf("Mutation!\n");
                
                float alteration = (float)rand() / RAND_MAX * MUTATION_HEIGHT;
                printf("Gene: %f + %f\n", genes[j], alteration);
                genes[j] += alteration;
            }
        }

        Individual *individual = new Individual();
            individual->setGenes(genes);
            Helper::calculateFitness(individual);
            mutations[i] = individual;
        
    }


    return true;

}

bool ClearPopulation(void* population[]){

    for (int i = 0; i < POPULATION_SIZE; i++) {
        delete (Individual*)population[i];
    }

    return true;
}

bool CopyPopulation(void* from[], void* to[]){

    
    
    for (int i = 0; i < POPULATION_SIZE; i++) {
        Individual *individual = new Individual();
        individual->setGenes(((Individual*)from[i])->getGenes());
        individual->setFitness(((Individual*)from[i])->getFitness());
        to[i] = individual;
    }

    return true;
}


bool ReplacePopulation(void* best_population[]){

    printf("Replacing population\n");


    void* temp_population[POPULATION_SIZE];

    if(best_population == population){
        printf("Replacing population with itself\n");
        ClearPopulation(gladiators);
        ClearPopulation(crosspoints);
        ClearPopulation(mutations);
    }

    printf("Copying population\n");

    CopyPopulation(best_population, temp_population);

    printf("Clearing population\n");

    ClearPopulation(population);

    printf("Copying temp population\n");

    CopyPopulation(temp_population, population);

    printf("Clearing temp population\n");

    // ClearPopulation(temp_population);
    ClearPopulation(gladiators);
    ClearPopulation(crosspoints);

    printf("Clearing mutations\n");
    ClearPopulation(mutations);


    


    return true;
}


bool Assignment::runAssignment(){

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //                                              DATA VARIATION
    //
    ///////////////////

    if(RANDOM_SEED != 0){
        srand(RANDOM_SEED);
    }else{
        int seed = time(NULL);
        printf("Random seed: %d\n", seed);
        srand(seed);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //                                          GENERATE INITIAL POPULATION
    //
    ///////////////////


    if(!GeneratePopulatin()){
        printf("Error generating population\n");
        return false;
    }



    Helper::printPopulation(population);


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //                                                  GENERATIONS
    //
    ///////////////////


    for (int g = 0; g < NUMBER_OF_GENERATIONS; g++) {
        /* code */

        printf("\n\nGeneration %d\n", g);


        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //                                          GLADIATOR SELECTION
        //
        ///////////////////

        printf("\n\n Gladiators\n\n");

        if(!GladiatorSelection()){
            printf("Error selecting gladiators\n");
            return false;
        }   

        // printf("\n\n Gladiators\n\n");
        // Helper::printPopulation(gladiators);


        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //                                              CROSSPOINT
        //
        ///////////////////

        printf("\n\n Crosspoints\n\n");

        if(!Crosspoint()){
            printf("Error crosspointing\n");
            return false;
        }

        // printf("\n\n Crosspoints\n\n");
        // Helper::printPopulation(crosspoints);


        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //                                                MUTATION
        //
        ///////////////////

        printf("\n\n Mutations\n\n");

        if(!Mutation()){
            printf("Error mutating\n");
            return false;
        }

        // printf("\n\n Mutations\n\n");
        // Helper::printPopulation(mutations);



        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //                                          REPLACE POPULATION WITH HIGHEST FITNESS
        //
        ///////////////////

        fitnesses[0] = Helper::getPopulationFitness(population);
        fitnesses[1] = Helper::getPopulationFitness(gladiators);
        fitnesses[2] = Helper::getPopulationFitness(crosspoints);
        fitnesses[3] = Helper::getPopulationFitness(mutations);


        printf("\n\nFitnesses:\n");
        printf("Population: %f\n", fitnesses[0]);
        printf("Gladiators: %f\n", fitnesses[1]);
        printf("Crosspoint: %f\n", fitnesses[2]);
        printf("Mutations: %f\n", fitnesses[3]);


        #ifdef FIND_BEST

            if (fitnesses[0] > fitnesses[1]) {
                if(fitnesses[0] > fitnesses[2])
                    if(fitnesses[0] > fitnesses[3])
                        ReplacePopulation(population);
                    else
                        ReplacePopulation(mutations);
                else
                    if(fitnesses[2] > fitnesses[3])
                        ReplacePopulation(crosspoints);
                    else
                        ReplacePopulation(mutations);
            }else{
                if(fitnesses[1] > fitnesses[2])
                    if(fitnesses[1] > fitnesses[3])
                        ReplacePopulation(gladiators);
                    else
                        ReplacePopulation(mutations);
                else
                    if(fitnesses[2] > fitnesses[3])
                        ReplacePopulation(crosspoints);
                    else
                        ReplacePopulation(mutations);
            }
        #else
            if (fitnesses[0] < fitnesses[1]) {
                if(fitnesses[0] < fitnesses[2])
                    if(fitnesses[0] < fitnesses[3])
                        ReplacePopulation(population);
                    else
                        ReplacePopulation(mutations);
                else
                    if(fitnesses[2] < fitnesses[3])
                        ReplacePopulation(crosspoints);
                    else
                        ReplacePopulation(mutations);
            }else{
                if(fitnesses[1] < fitnesses[2])
                    if(fitnesses[1] < fitnesses[3])
                        ReplacePopulation(gladiators);
                    else
                        ReplacePopulation(mutations);
                else
                    if(fitnesses[2] < fitnesses[3])
                        ReplacePopulation(crosspoints);
                    else
                        ReplacePopulation(mutations);
            }
        #endif



        #ifdef FIND_BEST
            best_fitnesses[g] = Helper::getPopulationHeight(population);
        #else
            worst_fitnesses[g] = Helper::getPopulationHeight(population);
        #endif

        average_fitnesses[g] = Helper::getPopulationFitness(population) / POPULATION_SIZE;

        printf("\n\nGeneration %d\n", g);

        #ifdef FIND_BEST
            printf("Best fitness: %f\n", best_fitnesses[g]);
        #else
            printf("Worst fitness: %f\n", worst_fitnesses[g]);
        #endif
        
        printf("Average fitness: %f\n", average_fitnesses[g]);



    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //                                          PRINT RESULTS
    //
    ///////////////////

    #ifdef FIND_BEST
        printf("\n\nBest fitnesses:\n");
        for (int i = 0; i < NUMBER_OF_GENERATIONS; i++) {
            printf("%d:  B:[%f]  A: [%f]\n", i, best_fitnesses[i], average_fitnesses[i]);
        }
    #else
        printf("\n\nWorst fitnesses:\n");
        for (int i = 0; i < NUMBER_OF_GENERATIONS; i++) {
            printf("%d:  W:[%f]  A: [%f]\n", i, worst_fitnesses[i], average_fitnesses[i]);
        }
    #endif

    return true;
}