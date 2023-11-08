#include <individual.h>

#include <stdio.h>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 CONSTRUCTOR AND DESTRUCTOR
//
//      TAKES ARGUMENTS:
//          NONE
//
//    CREATES AN INDIVIDUAL AND PRINTS THE ADDRESS OF THE INDIVIDUAL
//
//      RETURNS:
//          NONE
//
///////////////////

Individual::Individual() {
    #ifdef _DEBUG_INDIVIDUAL
        printf("0x%p constructor\n", this);
    #endif
}

Individual::~Individual() {
    #ifdef _DEBUG_INDIVIDUAL
        printf("0x%p destructor\n", this);
    #endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 FUNCTION TO GET THE FITNESS OF AN INDIVIDUAL
//      TAKES ARGUMENTS:
//          NONE
//
//
//      RETURNS:
//          THE FITNESS OF THE INDIVIDUAL AS A FLOAT
//
///////////////////

float Individual::getFitness() {
    return this->fitness;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 FUNCTION TO GET THE GENES OF AN INDIVIDUAL
//      TAKES ARGUMENTS:
//          NONE
//
//
//      RETURNS:
//          THE GENES OF THE INDIVIDUAL AS A FLOAT ARRAY

float* Individual::getGenes() {
    return this->genes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 FUNCTION TO SET THE FITNESS OF AN INDIVIDUAL
//      TAKES ARGUMENTS:
//          fitness : THE FITNESS TO SET THE INDIVIDUAL TO
//
//
//      SETS THE FITNESS OF THE INDIVIDUAL
//
//      RETURNS:
//          NONE
//
///////////////////

void Individual::setFitness(float fitness) {
    this->fitness = fitness;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 FUNCTION TO SET THE GENES OF AN INDIVIDUAL
//      TAKES ARGUMENTS:
//          genes : THE GENES TO SET THE INDIVIDUAL TO
//
//
//      cYCLE THROUGH EACH GENE IN THE INDIVIDUAL
//          SETS THE GENE
//
//      RETURNS:
//          NONE
//
///////////////////

void Individual::setGenes(float genes[]) {
    for (int i = 0; i < NUMBER_OF_GENES; i++) {
        this->genes[i] = genes[i];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 FUNCTION TO SET A GENE OF AN INDIVIDUAL
//      TAKES ARGUMENTS:
//          index : THE INDEX OF THE GENE TO SET
//          value : THE VALUE TO SET THE GENE TO
//
//
//      SETS THE GENE OF THE INDIVIDUAL
//
//      RETURNS:
//          NONE
//
///////////////////

void Individual::setGene(int index, float value) {
    this->genes[index] = value;
}





