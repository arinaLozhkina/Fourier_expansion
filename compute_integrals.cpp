#include "fourrier.h"

using namespace GiNaC;  // Для символьных вычислений

// Численно считает  определенный интеграл при помощи метода трапеций
// input: подынтегральная функция, границы интегрирования
// Для удовлетворительной точности используется 1001 отрезок разбиения
float compute_integral(ex integral_exp, symbol x, float start_int, float end_int) {
    int size, rank; // Ранк и число исполнителей
    int n = 1001; // Число отрезков разбиения
    float h = (end_int - start_int) / n; // Длина отрезка разбиения
    float* rec_buf; // Массив, принимающий коэффициенты от всех процессов
    int n0 = n - 1; // Число точек
    ex result = (integral_exp.subs(x==start_int) + integral_exp.subs(x==end_int)) * h / 2;
    MPI_Status status;
    if (MPI_Comm_size(MPI_COMM_WORLD, &size) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 2);
    if (MPI_Comm_rank(MPI_COMM_WORLD, &rank) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 3);

    int points_for_proc = n0 / size; // Число точек, обрабатывающихся за процесс
    int iterate = n0 / size;
    if (rank == size - 1)
        iterate += n0 % size;
    int max_points_in_rank = n0 / size + n0 % size;
    float coeff[max_points_in_rank]; // Коэффициенты, найденные за rank
    
    for (int i = 0; i < max_points_in_rank; i++){
        if (i < iterate)
            coeff[i] = float(ex_to<numeric>((integral_exp.subs(x==(start_int + (rank * points_for_proc + i + 1) * h)) * h)).to_double());
        else
            coeff[i] = 0;
    }

    if (rank == 0)
        rec_buf = (float *)malloc(size * sizeof(float) * max_points_in_rank);

    int st = MPI_Gather(coeff, max_points_in_rank, MPI_FLOAT, rec_buf, max_points_in_rank, MPI_FLOAT, 0, MPI_COMM_WORLD);
    if (st != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 5);

    if (rank == 0){
        for (int j = 0; j < size; j++){
            for (int i = 0; i < max_points_in_rank; i++){
                result += rec_buf[j * max_points_in_rank  + i];
            }
        }
        free(rec_buf);
        std::cout << " Integral calculated = " << result << std::endl;
    }
    
    return float(ex_to<numeric>(result).to_double());
}
    
