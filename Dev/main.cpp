
#include <assignment.h>

#include <graphing.h>



#include <iostream>

int main (){


    Assignment::runAssignment();

    gui::graphing();


    //ask user to close program
    std::cout << "Press any key to close program...";
    std::cin.get();
    

    return 0;
}