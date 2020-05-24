#include "fourrier.h"


// Численно считает  определенный интеграл при помощи метода трапеций
// input: подынтегральная функция, границы интегрирования
// Для удовлетворительной точности используется 1001 отрезок разбиения
float compute_integral(int n, GiNaC::ex integral_exp, GiNaC::symbol x, float start_int, float end_int, int rank, int size) {
    float h = (end_int - start_int) / n; // Длина отрезка разбиения
    GiNaC::ex result = (integral_exp.subs(x==start_int) + integral_exp.subs(x==end_int)) * h / 2;
    int n0 = n - 1; // Число точек
    int points_for_proc = n0 / size; // Число точек, обрабатывающихся за процесс
    int iterate = n0 / size;
    if (rank == size - 1)
        iterate += n0 % size;
    int max_points_in_rank = n0 / size + n0 % size;
    float* coeff = new float[max_points_in_rank];
    
    for (int i = 0; i < max_points_in_rank; i++){
        if (i < iterate)
            coeff[i] = float(GiNaC::ex_to<GiNaC::numeric>((integral_exp.subs(x==(start_int + (rank * points_for_proc + i + 1) * h)) * h)).to_double());
        else
            coeff[i] = 0;
    }
    
    float* rec_buf; // Массив, принимающий коэффициенты от всех процессов
    if (rank == 0)
        rec_buf = new float [max_points_in_rank];

    int err = MPI_Reduce(coeff, rec_buf, max_points_in_rank, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (err != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 5);

    if (rank == 0){
        for (int j = 0; j < max_points_in_rank; j++)
            result += rec_buf[j];
        delete[] rec_buf;
//        std::cout << " Integral calculated = " << result << std::endl;
    }
    
    return float(GiNaC::ex_to<GiNaC::numeric>(result).to_double());
}
    
