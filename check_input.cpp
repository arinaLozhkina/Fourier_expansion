#include "fourrier.h"

using namespace GiNaC;  // Для символьных вычислений

int check_input(std::string input){
   std::string operations = "+-*/^()";
   for (int i=0; i < input.length(); ++i)
   {
       if (not(isalnum(input[i]))){
           if ((i == input.length() - 1) && (input[i] != ')'))
               return 0;
           if ((i == 0) && (input[i] != '('))
               return 0;
           if ((operations.find(input[i])) == std::string::npos)
               return 0;
       }
   }
               return 1;
}

std::string exp_input(symbol x){
    std::cout << "Enter the function without space: f(x) = ";
    std::string input;
    std::cin >> input;

    if (check_input(input) == 0){
        std::cout << "Wrong input" << std::endl;
        return "0";
    }
    else
        return input;
}
