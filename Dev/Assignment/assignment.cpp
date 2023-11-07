#include <individual.h>
#include <helper.h>
#include <assignment.h>

#include <iostream>


float fitnesses[4];

void* population[POPULATION_SIZE];
void* gladiators[POPULATION_SIZE];
void* crosspoints[POPULATION_SIZE];
void* mutations[POPULATION_SIZE];




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
                
                float alteration = (float)rand() / RAND_MAX * MUTATION_HEIGHT;

                #ifdef _DEBUG_MUTATION
                    printf("Mutation!\n");
                    printf("Gene: %f + %f\n", genes[j], alteration);
                #endif
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

    void* temp_population[POPULATION_SIZE];

    if(best_population == population){
        printf("Replacing population with itself\n");
        ClearPopulation(gladiators);
        ClearPopulation(crosspoints);
        ClearPopulation(mutations);
    }

    CopyPopulation(best_population, temp_population);

    ClearPopulation(population);

    CopyPopulation(temp_population, population);

    // ClearPopulation(temp_population);
    ClearPopulation(gladiators);
    ClearPopulation(crosspoints);
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


    #ifdef _DEBUG_POPULATION
        printf("\n\nInitial population\n\n");
        Helper::printPopulation(population);
    #endif


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //                                                  GENERATIONS
    //
    ///////////////////


    for (int g = 0; g < NUMBER_OF_GENERATIONS; g++) {
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

        #ifdef _DEBUG_GENERATION

            printf("\n\nFitnesses:\n");
            printf("Population: %f\n", fitnesses[0]);
            printf("Gladiators: %f\n", fitnesses[1]);
            printf("Crosspoint: %f\n", fitnesses[2]);
            printf("Mutations: %f\n", fitnesses[3]);

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
                        // ReplacePopulation(population);
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


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //                                          PRINT RESULTS
    //
    ///////////////////

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