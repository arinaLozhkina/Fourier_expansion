#include "fourrier.h"

int fourrier_parallel(Integral_arguments args){
    float len = (args.int_end - args.int_start) / 2;
    float a_0 = compute_integral(args.n_int, args.input_expr, args.x, -len, len, args.rank, args.size) / len;
    GiNaC::ex result = a_0 / 2;
    
    float cur_cos, cur_sin;
    for (int i = 1; i <= args.n; i++){
        cur_cos = compute_integral(args.n_int, args.input_expr * cos(3.141592 * i * args.x / len), args.x, -len, len, args.rank, args.size) / len;
        cur_sin = compute_integral(args.n_int, args.input_expr * sin(3.141592 * i * args.x / len), args.x, -len, len, args.rank, args.size) / len;
        if (abs(cur_cos) > args.precision)
            result += cur_cos * cos(GiNaC::Pi * i * args.x / len);
        if (abs(cur_sin) > args.precision)
            result += cur_sin * sin(GiNaC::Pi * i * args.x / len);
    }
    GiNaC::Digits = 5; // Константа, определяющая точность
    if (args.rank == 0){
        std::cout << "Result = " << result.simplify_indexed().evalf().expand() << std::endl;
        std::cout << "In point = " << result.simplify_indexed().evalf().subs(args.x==0) << std::endl;
        std::cout << "In point start = " << args.input_expr.simplify_indexed().evalf().subs(args.x==0) << std::endl;
    }
    return 0;
}
