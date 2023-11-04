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
            // Helper::printIndividual(&selection[j]);
        }

        //find best individual from selection
        Individual best = selection[0];
        for (int j = 1; j < TOURNAMENT_SIZE; j++) {
            if(selection[j].getFitness() > best.getFitness()){
                best = selection[j];
            }
        }

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
            printf("crashcheck\n");

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
        bool mutated = false;

        for (int j = 0; j < NUMBER_OF_GENES; j++) {
            float random = (float)rand() / RAND_MAX;
            if(random < MUTATION_RATE){
                printf("Mutation!\n");
                
                float alteration = (float)rand() / RAND_MAX * MUTATION_HEIGHT;
                printf("Gene: %f + %f\n", genes[j], alteration);
                genes[j] += alteration;
                
                mutated = true;
            }
        }

        
        if(mutated == true){
            Individual *individual = new Individual();
            individual->setGenes(genes);
            Helper::calculateFitness(individual);
            mutations[i] = individual;
        } else {
            mutations[i] = crosspoints[i];
        }
    }


    return true;

}


bool Assignment::runAssignment(){

    if(RANDOM_SEED != 0){
        srand(RANDOM_SEED);
    }else{
        int seed = time(NULL);
        printf("Random seed: %d\n", seed);
        srand(seed);
    }



    if(!GeneratePopulatin()){
        printf("Error generating population\n");
        return false;
    }


    
    //print population

    Helper::printPopulation(population);


    // printf("\n\n Gladiators Selection\n\n");




    //Gladiator Selection

    if(!GladiatorSelection()){
        printf("Error selecting gladiators\n");
        return false;
    }


    // printf("\n\n Gladiators\n\n");
    //print gladiators

    Helper::printPopulation(gladiators);




    printf("\n\n Crosspoint\n\n");

    //Crosspoint

    if(!Crosspoint()){
        printf("Error crosspointing\n");
        return false;
    }


    //print crosspoint
    printf("\n\n Crosspoints\n\n");
    Helper::printPopulation(crosspoints);



    //Mutation

    if(!Mutation()){
        printf("Error mutating\n");
        return false;
    }

    Helper::printPopulation(mutations);





    fitnesses[0] = Helper::getPopulationFitness(population);
    fitnesses[1] = Helper::getPopulationFitness(gladiators);
    fitnesses[2] = Helper::getPopulationFitness(crosspoints);
    fitnesses[3] = Helper::getPopulationFitness(mutations);


    // print

    printf("\n\nFitnesses:\n");
    printf("Population: %f\n", fitnesses[0]);
    printf("Gladiators: %f\n", fitnesses[1]);
    printf("Crosspoint: %f\n", fitnesses[2]);
    printf("Mutations: %f\n", fitnesses[3]);

    if (fitnesses[0] > fitnesses[1]) {
        if(fitnesses[0] > fitnesses[2])
            if(fitnesses[0] > fitnesses[3])
                printf("Population wins!\n");
            else
                printf("Mutations win!\n");
        else
            if(fitnesses[2] > fitnesses[3])
                printf("Crosspoint wins!\n");
            else
                printf("Mutations win!\n");
    }else{
        if(fitnesses[1] > fitnesses[2])
            if(fitnesses[1] > fitnesses[3])
                printf("Gladiators win!\n");
            else
                printf("Mutations win!\n");
        else
            if(fitnesses[2] > fitnesses[3])
                printf("Crosspoint wins!\n");
            else
                printf("Mutations win!\n");
    }













    return true;
}