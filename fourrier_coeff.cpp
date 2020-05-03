#include "fourrier.h"

using namespace GiNaC;

int fourrier_parallel(symbol x, ex input_expr, float int_start, float int_end) {
    int size, rank; // Ранк и число исполнителей
    int n = 100; // Число выделяемых гармоник
    float l = (int_end - int_start) / 2;
    float cur_cos, cur_sin;
    Digits = 5; // Константа, определяющая точность
    float precision = 0.1; // Точность, при которой коэффициенты не считаем равными 0
    
    float a_0 = compute_integral(input_expr, x, -l, l) / l;
    ex result = a_0 / 2;

    for (int i = 1; i <= n; i++){
        cur_cos = compute_integral(input_expr * cos(3.141592 * i * x / l), x, -l, l) / l;
        cur_sin = compute_integral(input_expr * sin(3.141592 * i * x / l), x, -l, l) / l;
        if (cur_cos > precision)
            result += cur_cos * cos(Pi * i * x / l);
        if (cur_sin > precision)
            result += cur_sin * sin(Pi * i * x / l);
    }
    std::cout << "Result = " << result.simplify_indexed().evalf().expand() << std::endl;
    std::cout << "In point = " << result.simplify_indexed().evalf().subs(x==0) << std::endl;
    std::cout << "In point start = " << input_expr.simplify_indexed().evalf().subs(x==0) << std::endl;
    return 0;
}
