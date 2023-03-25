#include <iostream>
#include <cmath>
#include <numeric>

#include "omp.h"


void print_vector(double *vector, int n);


void print_matrix(double *matrix, int n, int m);


int main1_4_5(int argc, char **argv) {

    // Init

    int num_threads = 4;
    int n_size = 3;

    omp_set_num_threads(num_threads);


    // Initializing matrix A

    auto matrix_A = new double[n_size * n_size]{};
//    srand(time(nullptr));
    for (int i = 0; i < n_size * n_size; ++i) {
        matrix_A[i] = rand() % 10;
    }


    // Finding first norm

    double max_el = 0;
#pragma omp parallel for default(none) shared(n_size, matrix_A) reduction(max:max_el)
    for (int i = 0; i < n_size; ++i) {
        double sum = std::fabs(std::accumulate(&matrix_A[i * n_size], &matrix_A[(i + 1) * n_size], 0.));
        max_el = std::max(max_el, sum);
    }


    // Calculating matrix B = A^2 * norm1

    auto matrix_B = new double[n_size * n_size]{};
#pragma omp parallel for default(none) shared(max_el, n_size, matrix_A, matrix_B)
    for (int i = 0; i < n_size; ++i) {
        for (int j = 0; j < n_size; ++j) {
            double sum = 0;
            for (int k = 0; k < n_size; ++k) {
                sum += matrix_A[i * n_size + k] * matrix_A[k * n_size + j];
            }
            sum *= max_el;
            matrix_B[i * n_size + j] = sum;
        }
    }


    // Printing matrices and norm

    std::cout << "Matrix A:\n\n";
    print_matrix(matrix_A, n_size, n_size);
    std::cout << std::endl << max_el;
    std::cout << "\n\nMatrix B = A^2 * norm1(A):\n\n";
    print_matrix(matrix_B, n_size, n_size);


    // Cleanup

    delete[] matrix_A;
    delete[] matrix_B;

    return 0;
}