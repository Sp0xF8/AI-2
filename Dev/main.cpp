
#include <assignment.h>

#include <graphing.h>

#include <defines.h>

#include <chrono>



#include <iostream>

int main (){


    #ifdef _TIMER

        std::chrono::steady_clock totalelapsed;
        auto start = std::chrono::high_resolution_clock::now();
  
        for (int i = 0; i < 100; i++){
    #endif

    #ifdef RUN_GENETIC_ALGORITHM

            Assignment::runAssignment();

    #endif

    #ifdef RUN_HILL_CLIMBER

            Assignment::runHillClimber();
    #endif

    #ifdef _TIMER
        }


        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        float average = elapsed.count() / 100;
        std::cout << "Time taken by program is : " << average << " seconds" << std::endl;
    #endif

    gui::graphing();


    //ask user to close program
    std::cout << "Press any key to close program...";
    std::cin.get();
    

    return 0;
}