#include <individual.h>
#include <helper.h>
#include <assignment.h>

#include <iostream>


#ifdef _PLOT_GRAPHS
    
#else
    float fitnesses[4];
#endif

int g = 0;


#ifdef _META_AI

    #include <vector>

    float mutation_rate = 0.0;
    float mutation_height = 0.0;
    int tournament_size = 2;

    float best_mutation_rate = 0.0;
    float best_mutation_height = 0.0;
    int best_tournament_size = 2;

    #ifdef FIND_BEST 
        float ending_fitness_height = 0.0;
    #else
        float ending_fitness_height = 1.0;
    #endif
#endif

void* population[POPULATION_SIZE];
void* gladiators[POPULATION_SIZE];
void* crosspoints[POPULATION_SIZE];
void* mutations[POPULATION_SIZE];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 SIMPLE FUNCTION TO GENERATE A POPULATION
//      TAKES ARGUMENTS:
//          #NONE - USES GLOBAL VARIABLES EXISTING INSIDE THIS CPP FILE
//
//
//      CYCLES THROUGH EACH INDIVIDUAL IN THE POPULATION
//          GENERATES GENES FOR EACH INDIVIDUAL
//          CALCULATES THE FITNESS OF EACH INDIVIDUAL
//          ADDS THE INDIVIDUAL TO THE POPULATION
//
//      RETURNS:
//          true : IF THE POPULATION WAS GENERATED
//          false : IF THE POPULATION WAS NOT GENERATED
//
///////////////////

bool GeneratePopulatin(){

    for (int i = 0; i < POPULATION_SIZE; i++) {
        Individual *individual = new Individual();
        Helper::generateGenes(individual);              // Populate initial population
        population[i] = individual;

        Helper::calculateFitness(individual);           // Calculate fitness of each individual
    }

    return true;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                          GLADIATOR SELECTION FUNCTION
//      TAKES ARGUMENTS:
//          #NONE - USES GLOBAL VARIABLES EXISTING INSIDE THIS CPP FILE
//
//
//      CYCLES THROUGH EACH INDIVIDUAL IN THE POPULATION
//          GETS x AMOUNT OF RANDOM INDIVIDUALS FROM THE POPULATION
//          FINDS THE BEST INDIVIDUAL FROM THE RANDOM INDIVIDUALS
//          CLONES THE BEST INDIVIDUAL TO THE GLADIATORS POPULATION
//
//      RETURNS:
//          true : IF THE POPULATION WAS GLADIATOR SELECTED
//          false : IF THE POPULATION WAS NOT GLADIATOR SELECTED
//
///////////////////

bool GladiatorSelection(){

    #ifdef _META_AI

        for(int i = 0; i < POPULATION_SIZE; i++){
            std::vector<Individual> selection;
            selection.push_back(*(Individual*)population[i]);

            for(int j = 1; j < tournament_size; j++){
                int random_index = rand() % POPULATION_SIZE;
                selection.push_back(*(Individual*)population[random_index]);
            }

            Individual best = selection[0];

            #ifdef FIND_BEST
                for(int j = 1; j < tournament_size; j++){
                    if(selection[j].getFitness() > best.getFitness()){
                        best = selection[j];
                    }
                }
            #else
                for(int j = 1; j < tournament_size; j++){
                    if(selection[j].getFitness() < best.getFitness()){
                        best = selection[j];
                    }
                }
            #endif

            Individual *gladiator = new Individual();
            gladiator->setGenes(best.getGenes());
            gladiator->setFitness(best.getFitness());
            gladiators[i] = gladiator;



        }

    #else

        for (int i = 0; i < POPULATION_SIZE; i++) {
            Individual selection[TOURNAMENT_SIZE];
            selection[0] = *(Individual*)population[i];
            
            for (int j = 1; j < TOURNAMENT_SIZE; j++) {
                int random_index = rand() % POPULATION_SIZE;
                selection[j] = *(Individual*)population[random_index];
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

    #endif

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                          Crosspoint Function
//      TAKES ARGUMENTS:
//          #NONE - USES GLOBAL VARIABLES EXISTING INSIDE THIS CPP FILE
//
//
//      CYCLES THROUGH EACH INDIVIDUAL IN THE POPULATION
//          GETS 2  RANDOM INDIVIDUALS FROM THE POPULATION
//          GETS A RANDOM CROSSPOINT
//              SETS THE GENES OF THE NEW INDIVIDUALS TO THE GENES OF THE RANDOM INDIVIDUALS
//              SWAPS THE GENES OF THE NEW INDIVIDUALS AT THE CROSSPOINT
//              CALCULATES THE FITNESS OF THE NEW INDIVIDUALS
//              ADDS THE NEW INDIVIDUALS TO THE CROSSPOINT POPULATION
//
//      RETURNS:
//          true : IF THE POPULATION WAS CROSSPOINTED
//          false : IF THE POPULATION WAS NOT CROSSPOINTED
//
///////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                          Mutation Function
//      TAKES ARGUMENTS:
//          #NONE - USES GLOBAL VARIABLES EXISTING INSIDE THIS CPP FILE
//  
//
//      CYCLES THROUGH EACH INDIVIDUAL IN THE POPULATION 
//          AND CYCLES THROUGH EACH GENE IN THE INDIVIDUAL
//          GENERATES A RANDOM FLOAT BETWEEN 0 AND 1
//          IF THE RANDOM FLOAT IS LESS THAN THE MUTATION RATE
//              THEN GENERATE A RANDOM FLOAT BETWEEN 0 AND THE MUTATION HEIGHT
//              AND ADD IT TO THE GENE
//
//      RETURNS:
//          true : IF THE POPULATION WAS MUTATED
//          false : IF THE POPULATION WAS NOT MUTATED
//
///////////////////

bool Mutation(){

    #ifdef _META_AI

        for(int i = 0; i < POPULATION_SIZE; i++){
            float* genes = ((Individual*)crosspoints[i])->getGenes();

            for (int j = 0; j < NUMBER_OF_GENES; j++){
                float random = (float)rand() / RAND_MAX;
                if(random < mutation_rate){
                    
                    float alteration = (float)rand() / RAND_MAX * mutation_height;
                    float negative = (float)rand() / RAND_MAX;

                    #ifdef _DEBUG_MUTATION
                        printf("Mutation!\n");
                        printf("Gene: %f + %f\n", genes[j], alteration);
                    #endif

                    if (negative < 0.5) {
                        genes[j] += alteration;
                    } else {
                        genes[j] -= alteration;
                    }

                    if(genes[j] > 1)
                        genes[j] = 1;
                    if(genes[j] < 0)
                        genes[j] = 0;
                    
                }
            }

            Individual *individual = new Individual();
            individual->setGenes(genes);
            Helper::calculateFitness(individual);
            mutations[i] = individual;
        }

    #else

        for (int i = 0; i < POPULATION_SIZE; i++) {
            float* genes = ((Individual*)crosspoints[i])->getGenes();

            for (int j = 0; j < NUMBER_OF_GENES; j++) {
                float random = (float)rand() / RAND_MAX;
                if(random < MUTATION_RATE){
                    
                    float alteration = (float)rand() / RAND_MAX * MUTATION_HEIGHT;
                    float negative = (float)rand() / RAND_MAX;

                    #ifdef _DEBUG_MUTATION
                        printf("Mutation!\n");
                        printf("Gene: %f + %f\n", genes[j], alteration);
                    #endif

                    if (negative < 0.5) {
                        genes[j] += alteration;
                    } else {
                        genes[j] -= alteration;
                    }

                    if(genes[j] > 1)
                        genes[j] = 1;
                    if(genes[j] < 0)
                        genes[j] = 0;
                    
                }
            }

            Individual *individual = new Individual();
            individual->setGenes(genes);
            Helper::calculateFitness(individual);
            mutations[i] = individual;
            
        }

    #endif


    return true;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    REPLACE POPULATION WITH BEST POPULATION
//      TAKES ARGUMENTS:
//          best_population : THE POPULATION TO REPLACE THE CURRENT POPULATION WITH
//
//
//      DELETES THE CURRENT POPULATION AND REPLACES IT WITH THE BEST POPULATION
//          IF THE BEST POPULATION IS THE SAME AS THE CURRENT POPULATION, IT WILL DELETE OTHER POPULATIONS
//          OTHERWISE, IT WILL DELETE THE CURRENT POPULATION AND REPLACE IT WITH THE BEST POPULATION
//      
//      RETURNS: 
//          true : IF THE POPULATION WAS REPLACED
//          false : IF THE POPULATION WAS NOT REPLACED
//
///////////////////

bool ReplacePopulation(void* best_population[]){

    #ifdef _GRAPH_GENERATION_HISTORY
        Helper::CopyPopulation(population, Helper::generation_history[g].population);
        Helper::CopyPopulation(gladiators, Helper::generation_history[g].gladiators);
        Helper::CopyPopulation(crosspoints, Helper::generation_history[g].crosspoints);
        Helper::CopyPopulation(mutations, Helper::generation_history[g].mutations);
    #endif

    void* temp_population[POPULATION_SIZE];

    if(best_population == population){
        printf("Replacing population with itself\n");

    } else {

        Helper::CopyPopulation(best_population, temp_population);

        Helper::ClearPopulation(population);

        Helper::CopyPopulation(temp_population, population);

    // ClearPopulation(temp_population);
    }

    Helper::ClearPopulation(gladiators);
    Helper::ClearPopulation(crosspoints);
    Helper::ClearPopulation(mutations);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 THIS IS THE PART OF THE PROGRAM WHICH IS CALLED FROM MAIN
//      TAKES ARGUMENTS:
//          #NONE
//
//
//      THIS FUNCTION RUNS THE ENTIRE PROGRAM. IT GENERATES THE INITIAL POPULATION, THEN RUNS THE GENERATIONS LOOP
//         
//                                                   THIS FUNCTION IS CALLED EXTERNALLY, INSTEAD OF MAIN TO ALLOW FOR
//                                                   EASY TESTING OF THE PROGRAM AND BETTER READABILITY
//
//      RETURNS:
//          true : IF THE PROGRAM WAS RUN SUCCESSFULLY
//          false : IF THE PROGRAM WAS NOT RUN SUCCESSFULLY
//
///////////////////

bool Assignment::runAssignment(){

    #ifdef _META_AI
    
    for (tournament_size = 2; tournament_size < 10; tournament_size++) {
        for ( mutation_rate = 0.0; mutation_rate < 0.3; mutation_rate += 0.01) {
            for ( mutation_height = 0.0; mutation_height < 0.5; mutation_height += 0.01) {

                // printf("Mutation rate: %f\n", mutation_rate);
                // printf("Mutation height: %f\n", mutation_height);
                // printf("Tournament size: %d\n", tournament_size);


               
    #endif

                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //
                //                                              DATA VARIATION
                //
                ///////////////////

                #ifdef RANDOM_SEED
                    
                    if(RANDOM_SEED != 0){
                        srand(RANDOM_SEED);
                    }else{
                        printf("Seed is 0, using time as seed\n");
                        int seed = time(NULL);
                        printf("Random seed: %d\n", seed);
                        srand(seed);
                    }
                #else
                    int seed = time(NULL);
                    printf("Random seed: %d\n", seed);
                    srand(seed);
                #endif

                

                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //
                //                                          GENERATE INITIAL POPULATION
                //
                ///////////////////




                if(!GeneratePopulatin()){
                    printf("Error generating population\n");
                    return false;
                }

                #ifdef _DEBUG_POPULATION
                    printf("\n\nInitial population\n\n");
                    Helper::printPopulation(population);
                #endif

                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //
                //                                                  GENERATIONS
                //
                ///////////////////


                for (g = 0; g < NUMBER_OF_GENERATIONS; g++) {
                    /* code */

                    #ifdef _DEBUG_GENERATION
                        printf("\n\nGeneration %d\n", g);
                    #endif

                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //
                    //                                          GLADIATOR SELECTION
                    //
                    ///////////////////

                    if(!GladiatorSelection()){
                        printf("Error selecting gladiators\n");
                        return false;
                    }   

                    #ifdef _DEBUG_GLADIATOR
                        printf("\n\n Gladiators\n\n");
                        Helper::printPopulation(gladiators);
                    #endif

                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //
                    //                                              CROSSPOINT
                    //
                    ///////////////////

                    if(!Crosspoint()){
                        printf("Error crosspointing\n");
                        return false;
                    }

                    #ifdef _DEBUG_CROSSPOINT
                        printf("\n\n Crosspoints\n\n");
                        Helper::printPopulation(crosspoints);
                    #endif

                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //
                    //                                                MUTATION
                    //
                    ///////////////////

                    if(!Mutation()){
                        printf("Error mutating\n");
                        return false;
                    }

                    #ifdef _DEBUG_MUTATION
                        printf("\n\n Mutations\n\n");
                        Helper::printPopulation(mutations);
                    #endif

                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //
                    //                              CALCULATE FITNESS FOR EACH POPULATION AND STORE FOR GRAPHING
                    //                                              IF GRAPHING IS ENABLED
                    //                              OTHERWISE, STORE FITNESS FOR EACH POPULATION TEMPORARILY
                    //
                    ///////////////////

                    #ifdef _PLOT_GRAPHS

                        #ifdef FIND_BEST 
                            Helper::generations[g].population.best = Helper::getPopulationHeight(population);
                            Helper::generations[g].gladiator.best = Helper::getPopulationHeight(gladiators);
                            Helper::generations[g].crosspoint.best = Helper::getPopulationHeight(crosspoints);
                            Helper::generations[g].mutation.best = Helper::getPopulationHeight(mutations);
                        #else
                            Helper::generations[g].population.worst = Helper::getPopulationHeight(population);
                            Helper::generations[g].gladiator.worst = Helper::getPopulationHeight(gladiators);
                            Helper::generations[g].crosspoint.worst = Helper::getPopulationHeight(crosspoints);
                            Helper::generations[g].mutation.worst = Helper::getPopulationHeight(mutations);
                        #endif

                        Helper::generations[g].population.total = Helper::getPopulationFitness(population);
                        Helper::generations[g].population.average = Helper::generations[g].population.total / POPULATION_SIZE;

                        Helper::generations[g].gladiator.total = Helper::getPopulationFitness(gladiators);
                        Helper::generations[g].gladiator.average = Helper::generations[g].gladiator.total / POPULATION_SIZE;

                        Helper::generations[g].crosspoint.total = Helper::getPopulationFitness(crosspoints);
                        Helper::generations[g].crosspoint.average = Helper::generations[g].crosspoint.total / POPULATION_SIZE;

                        Helper::generations[g].mutation.total = Helper::getPopulationFitness(mutations);
                        Helper::generations[g].mutation.average = Helper::generations[g].mutation.total / POPULATION_SIZE;

                    #else 

                        fitnesses[0] = Helper::getPopulationFitness(population);
                        fitnesses[1] = Helper::getPopulationFitness(gladiators);
                        fitnesses[2] = Helper::getPopulationFitness(crosspoints);
                        fitnesses[3] = Helper::getPopulationFitness(mutations);

                    #endif

                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //
                    //                                      PRINT CURRENT GENERATION'S POPULATIONS' FITNESSES
                    //
                    ///////////////////

                    #ifdef _PLOT_GRAPHS

                    #else

                        #ifdef _DEBUG_GENERATION

                            printf("\n\nFitnesses:\n");
                            printf("Population: %f\n", fitnesses[0]);
                            printf("Gladiators: %f\n", fitnesses[1]);
                            printf("Crosspoint: %f\n", fitnesses[2]);
                            printf("Mutations: %f\n", fitnesses[3]);

                        #endif
                        

                    #endif

                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //
                    //                                          REPLACE POPULATION WITH HIGHEST FITNESS
                    //
                    ///////////////////

                    #ifdef HEAVY_ELITEISM
                        #ifdef _PLOT_GRAPHS

                            #ifdef FIND_BEST

                                if (Helper::generations[g].population.best > Helper::generations[g].gladiator.best) {
                                    if(Helper::generations[g].population.best > Helper::generations[g].crosspoint.best)
                                        if(Helper::generations[g].population.best > Helper::generations[g].mutation.best)
                                            ReplacePopulation(population);
                                        else
                                            ReplacePopulation(mutations);
                                    else
                                        if(Helper::generations[g].crosspoint.best > Helper::generations[g].mutation.best)
                                            ReplacePopulation(crosspoints);
                                        else
                                            ReplacePopulation(mutations);
                                }else{
                                    if(Helper::generations[g].gladiator.best > Helper::generations[g].crosspoint.best)
                                        if(Helper::generations[g].gladiator.best > Helper::generations[g].mutation.best)
                                            ReplacePopulation(gladiators);
                                        else
                                            ReplacePopulation(mutations);
                                    else
                                        if(Helper::generations[g].crosspoint.best > Helper::generations[g].mutation.best)
                                            ReplacePopulation(crosspoints);
                                        else
                                            ReplacePopulation(mutations);
                                }
                            #else

                                if (Helper::generations[g].population.worst < Helper::generations[g].gladiator.worst) {
                                    if(Helper::generations[g].population.worst < Helper::generations[g].crosspoint.worst)
                                        if(Helper::generations[g].population.worst < Helper::generations[g].mutation.worst)
                                            ReplacePopulation(population);
                                        else
                                            ReplacePopulation(mutations);
                                    else
                                        if(Helper::generations[g].crosspoint.worst < Helper::generations[g].mutation.worst)
                                            ReplacePopulation(crosspoints);
                                        else
                                            ReplacePopulation(mutations);
                                }else{
                                    if(Helper::generations[g].gladiator.worst < Helper::generations[g].crosspoint.worst)
                                        if(Helper::generations[g].gladiator.worst < Helper::generations[g].mutation.worst)
                                            ReplacePopulation(gladiators);
                                        else
                                            ReplacePopulation(mutations);
                                    else
                                        if(Helper::generations[g].crosspoint.worst < Helper::generations[g].mutation.worst)
                                            ReplacePopulation(crosspoints);
                                        else
                                            ReplacePopulation(mutations);
                                }
                            #endif

                        #else
                        
                            #ifdef FIND_BEST

                                if(fitnesses[0] > fitnesses[1]){
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
                                } else {
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

                                if(fitnesses[0] < fitnesses[1]){
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
                                } else {
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
                        #endif
                    #endif

                    #ifdef ELITEISM 
                        #ifdef _PLOT_GRAPHS
                            #ifdef FIND_BEST 
                                if (Helper::generations[g].population.best > Helper::generations[g].mutation.best) {
                                    ReplacePopulation(population);
                                }else{
                                    ReplacePopulation(mutations);
                                }
                            #else
                                if (Helper::generations[g].population.worst < Helper::generations[g].mutation.worst) {
                                    ReplacePopulation(population);
                                }else{
                                    ReplacePopulation(mutations);
                                }
                            #endif
                        #else
                            #ifdef FIND_BEST 
                                if(fitnesses[0] > fitnesses[3]){
                                    ReplacePopulation(population);
                                }else{
                                    ReplacePopulation(mutations);
                                }
                            #else
                                if(fitnesses[0] < fitnesses[3]){
                                    ReplacePopulation(population);
                                }else{
                                    ReplacePopulation(mutations);
                                }
                            #endif
                        #endif
                    #endif

                    #ifdef SIMPLE_PASSDOWN
                        ReplacePopulation(mutations);
                    #endif

                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //
                    //                                          CALCULATE GENERATIONAL FITNESS AND HEIGHT
                    //
                    ///////////////////

                    #ifdef FIND_BEST
                        Helper::best_fitnesses[g] = Helper::getPopulationHeight(population);
                    #else
                        Helper::worst_fitnesses[g] = Helper::getPopulationHeight(population);
                    #endif

                    Helper::average_fitnesses[g] = Helper::getPopulationFitness(population) / POPULATION_SIZE;

                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //
                    //                                          PRINT GENERATIONAL FITNESS AND HEIGHT
                    //
                    ///////////////////

                    #ifdef _DEBUG_GENERATION

                        printf("\n\nGeneration %d\n", g);

                        #ifdef FIND_BEST
                            printf("Best fitness: %f\n", Helper::best_fitnesses[g]);
                        #else
                            printf("Worst fitness: %f\n", Helper::worst_fitnesses[g]);
                        #endif
                        
                        printf("Average fitness: %f\n", Helper::average_fitnesses[g]);
                    
                    #endif


                }
                    #ifdef _META_AI

                        printf("Average fitness: %f\n", Helper::average_fitnesses[NUMBER_OF_GENERATIONS - 1]);

                    


   
                #ifdef FIND_BEST
                    if(Helper::average_fitnesses[NUMBER_OF_GENERATIONS - 1] > ending_fitness_height){

                        if(Helper::average_fitnesses[NUMBER_OF_GENERATIONS - 1] < 10.0){
                            ending_fitness_height = Helper::average_fitnesses[NUMBER_OF_GENERATIONS - 1];
                            best_mutation_height = mutation_height;
                            best_mutation_rate = mutation_rate;
                            best_tournament_size = tournament_size;
                        }

                        
                    }
                #else
                    if(Helper::average_fitnesses[NUMBER_OF_GENERATIONS - 1] < ending_fitness_height){
                        ending_fitness_height = Helper::average_fitnesses[NUMBER_OF_GENERATIONS - 1];
                        best_mutation_height = mutation_height;
                        best_mutation_rate = mutation_rate;
                        best_tournament_size = tournament_size;
                    }
                #endif
            }   


            #ifdef FIND_BEST
                if(Helper::average_fitnesses[NUMBER_OF_GENERATIONS - 1] > ending_fitness_height){
                    if(Helper::average_fitnesses[NUMBER_OF_GENERATIONS - 1] < 10.0){
                        ending_fitness_height = Helper::average_fitnesses[NUMBER_OF_GENERATIONS - 1];
                        best_mutation_height = mutation_height;
                        best_mutation_rate = mutation_rate;
                        best_tournament_size = tournament_size;
                    }
                }
            #else
                if(Helper::average_fitnesses[NUMBER_OF_GENERATIONS - 1] < ending_fitness_height){
                    ending_fitness_height = Helper::average_fitnesses[NUMBER_OF_GENERATIONS - 1];
                    best_mutation_height = mutation_height;
                    best_mutation_rate = mutation_rate;
                    best_tournament_size = tournament_size;
                }
            #endif
            
        }

        #ifdef FIND_BEST
            if(Helper::average_fitnesses[NUMBER_OF_GENERATIONS - 1] > ending_fitness_height){
                if(Helper::average_fitnesses[NUMBER_OF_GENERATIONS - 1] < 10.0){
                    ending_fitness_height = Helper::average_fitnesses[NUMBER_OF_GENERATIONS - 1];
                    best_mutation_height = mutation_height;
                    best_mutation_rate = mutation_rate;
                    best_tournament_size = tournament_size;
                }
            }
        #else
            if(Helper::average_fitnesses[NUMBER_OF_GENERATIONS - 1] < ending_fitness_height){
                ending_fitness_height = Helper::average_fitnesses[NUMBER_OF_GENERATIONS - 1];
                best_mutation_height = mutation_height;
                best_mutation_rate = mutation_rate;
                best_tournament_size = tournament_size;
            }
        #endif
    }
#endif
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //                                          PRINT RESULTS
    //
    ///////////////////



    #ifdef _META_AI
        printf("\n\nBest mutation rate: %f\n", best_mutation_rate);
        printf("Best mutation height: %f\n", best_mutation_height);
        printf("Best tournament size: %d\n", best_tournament_size);
        printf("Ending fitness height: %f\n", ending_fitness_height);
    #endif


    #ifdef _DEBUG_FITNESS
        #ifdef FIND_BEST
            printf("\n\nBest fitnesses:\n");
            for (int i = 0; i < NUMBER_OF_GENERATIONS; i++) {
                printf("%d:  B:[%f]  A: [%f]\n", i, Helper::best_fitnesses[i], Helper::average_fitnesses[i]);
            }
        #else
            printf("\n\nWorst fitnesses:\n");
            for (int i = 0; i < NUMBER_OF_GENERATIONS; i++) {
                printf("%d:  W:[%f]  A: [%f]\n", i, Helper::worst_fitnesses[i], Helper::average_fitnesses[i]);
            }
        #endif
    #endif

    return true;
}