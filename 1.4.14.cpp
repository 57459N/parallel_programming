#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"

#include "tasks.h"

int main1_4_14(int argc, char **argv) {


    // with omp configuration argv[1] is number of threads

    int thread_num = atoi(argv[1]);
    omp_set_num_threads(thread_num);


    int n_size = 3;

    double *matrix_A = nullptr;

    double *matrix_B = nullptr;

    alloc_fill_matrix(matrix_A, n_size, n_size);
    alloc_fill_matrix(matrix_B, n_size, n_size);

    std::cout << "Matrix A:\n";
    print_matrix(matrix_A, n_size, n_size);
    std::cout << "\nMatrix B:\n";
    print_matrix(matrix_B, n_size, n_size);
    std::cout << std::endl;

    auto matrix_C = new double[n_size * n_size]{};
    std::cout << "start\n";
#pragma omp parallel
    {
        int rank = omp_get_thread_num();

        for (int i = rank; i < n_size; i += thread_num) {
            for (int j = 0; j < n_size; ++j) {
                std::cout << rank << " started " << i << " " << j << "\n";
                double sum = 0;
                for (int k = 0; k < n_size; ++k) {
                #pragma omp critical
                    {
                        sum += matrix_A[i * n_size + k] * matrix_A[k * n_size + j];
                        sum += matrix_B[i * n_size + k] * matrix_B[k * n_size + j];
                    };
                }

                std::cout << rank << " ended " << i << " " << j << "\n";
            }

        }
    };
    std::cout << "end\n";
    print_matrix(matrix_C, n_size, n_size);


    delete[] matrix_A;
    delete[] matrix_B;
    delete[] matrix_C;
    return 0;
}

#pragma clang diagnostic pop