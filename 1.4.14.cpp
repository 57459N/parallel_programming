#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"

#include "tasks.h"
#include <chrono>

template<typename T>
void fill_matrix(T *matrix, int n, int m) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            matrix[n * i + j] = rand() % 10;
}

template<typename T>
void print_matrix(T *matrix, int n, int m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j)
            std::cout << std::setw(20) << matrix[n * i + j];
        std::cout << std::endl;
    }
}

int main1_4_14(int argc, char **argv) {

    // with omp configuration argv[1] is number of threads

    int thread_num = atoi(argv[1]);
    omp_set_num_threads(thread_num);

    std::srand(0);
    int n_size = 3;

    /*  with srand(0)
        Matrix A:
            8              9              8
            7              5              7
            5              5              0

        Matrix B:
            2              3              0
            2              1              7
            1              5              5

        Matrix C:
            177            166            148
            139            165            133
             92            103            135
     */

    auto *matrix_A = new double[n_size * n_size];

    auto *matrix_B = new double[n_size * n_size];

    fill_matrix(matrix_A, n_size, n_size);
    fill_matrix(matrix_B, n_size, n_size);


    std::cout << "Matrix A:\n";
    print_matrix(matrix_A, n_size, n_size);
    std::cout << "\nMatrix B:\n";
    print_matrix(matrix_B, n_size, n_size);
    std::cout << std::endl;

    auto matrix_C = new double[n_size * n_size]{};

    auto start_point = std::chrono::high_resolution_clock::now();

#pragma omp parallel
    {
        int rank = omp_get_thread_num();

        for (int i = rank; i < n_size; i += thread_num) {
            for (int j = 0; j < n_size; ++j) {
                double sum = 0;
                for (int k = 0; k < n_size; ++k) {
                    sum += matrix_A[i * n_size + k] * matrix_A[k * n_size + j];
                    sum += matrix_B[i * n_size + k] * matrix_B[k * n_size + j];
                }
                matrix_C[i * n_size + j] = sum;
            }

        }
    };

    auto end_point = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(start_point).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_point).time_since_epoch().count();


    std::cout << (end - start) << " microseconds\n";

    print_matrix(matrix_C, n_size, n_size);


    delete[] matrix_A;
    delete[] matrix_B;
    delete[] matrix_C;
    return 0;
}

#pragma clang diagnostic pop