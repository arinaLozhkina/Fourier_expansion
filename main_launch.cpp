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

int main(int argc, char * argv[]){
    GiNaC::symbol x("x"); // Переменная
    int rank, size;
    char input_char[50];
    
    int err = MPI_Init(&argc, &argv);
    if (err != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 1);
    if (MPI_Comm_size(MPI_COMM_WORLD, &size) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 2);
    if (MPI_Comm_rank(MPI_COMM_WORLD, &rank) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 3);
    
    double start_run_time, end_run_time;
    MPI_Barrier(MPI_COMM_WORLD);
    start_run_time = MPI_Wtime();
    
    if (rank == 0){
        std::string input = exp_input(x);
        std::strcpy (input_char, input.c_str());
        if (input == "0"){
            err = MPI_Finalize();
               if (err != MPI_SUCCESS)
                   MPI_Abort(MPI_COMM_WORLD, 6);
            return 0;
        }
    }
    err = MPI_Bcast(input_char, 50, MPI_CHAR, 0, MPI_COMM_WORLD);
    if (err != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 4);
    GiNaC::symtab table;
    table["x"] = x;
    GiNaC::parser reader(table);
    GiNaC::ex input_exp = reader(input_char);
    
    float int_start = -3.141592;
    float int_end = 3.141592;
    int n = 10; // Число выделяемых гармоник
    float precision = 0.001; // Точность, при которой коэффициенты не считаем равными 0
    int n_int = 10001; // Число отрезков разбиения при интегрировании
    Integral_arguments args{n, precision, n_int, x, input_exp, int_start, int_end, rank, size};
    int new_m = fourrier_parallel(args);
    
    MPI_Barrier(MPI_COMM_WORLD);
    end_run_time = MPI_Wtime();
    err = MPI_Finalize();
    if (err != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 6);
    
    if (rank == 0)
        std::cout << "Program run time: " << end_run_time - start_run_time << std::endl;
    return 0; 
}
