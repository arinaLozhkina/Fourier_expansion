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

using namespace GiNaC;

float compute_integral(ex integral_exp, symbol x, float start_int, float end_int);
std::string exp_input(symbol x);
int fourrier_parallel(symbol x, ex input_expr, float int_start, float int_end);

#endif /* fourrier_h */
