#ifndef fourrier_h
#define fourrier_h

#include <stdio.h>
#include <ginac/ginac.h>
#include <numeric>
#include <math.h>
#include <mpi.h>
#include <iostream>
#include <ctype.h>
#include <string>
#include <vector>

struct Integral_arguments {int n; float precision; int n_int; GiNaC::symbol x; GiNaC::ex input_expr; float int_start; float int_end; int rank; int size;};

float compute_integral(int n, GiNaC::ex integral_exp, GiNaC::symbol x, float start_int, float end_int, int rank, int size);
std::string exp_input(GiNaC::symbol x);
int fourrier_parallel(Integral_arguments args);

#endif /* fourrier_h */
