#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <ginac/ginac.h>
#include <numeric>
#include <math.h>
#include <mpi.h>
#include <ctype.h>

#include "fourrier.h"
using namespace GiNaC;

int main(int argc, char * argv[]){
    symbol x("x"); // Переменная

    float int_start = -3.141592;
    float int_end = 3.141592;
    int rank, size;
    char input_char[50];
    
    int st = MPI_Init(&argc, &argv);
    MPI_Status status;
    if (st != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 1);
    if (MPI_Comm_size(MPI_COMM_WORLD, &size) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 2);
    if (MPI_Comm_rank(MPI_COMM_WORLD, &rank) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 3);
    
    if (rank == 0){
        std::string input = exp_input(x);
        std::copy(input.begin(), input.end(), input_char);
        if (input == "0"){
            st = MPI_Finalize();
               if (st != MPI_SUCCESS)
                   MPI_Abort(MPI_COMM_WORLD, 6);
            return 0;
        }
    }
    st = MPI_Bcast(input_char, 50, MPI_CHAR, 0, MPI_COMM_WORLD);
    if (st != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 4);
    symtab table;
    table["x"] = x;
    parser reader(table);
    ex input_exp = reader(input_char);
    
    int new_m = fourrier_parallel(x, input_exp, int_start, int_end);
    st = MPI_Finalize();
    if (st != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 6);
    return 0; 
}
