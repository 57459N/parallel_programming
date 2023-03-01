#include <iomanip>
#include <iostream>
#include "mpi.h"

template<typename T>
void calculate_line(T *matrix_A, T *matrix_B, int matrix_inner_size, int line_number, T *Line, bool additive = false) {
    for (int j = 0; j < matrix_inner_size; ++j) {
        double sum = 0;
        if (additive)
            sum = Line[j];

        for (int k = 0; k < matrix_inner_size; ++k) {
            sum += matrix_A[line_number * matrix_inner_size + k] * matrix_A[k * matrix_inner_size + j];
        }
        Line[j] = sum;
    }
}

template<typename T>
void print_matrix(T *matrix, int n, int m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j)
            std::cout << std::setw(15) << matrix[i * n + j];
        std::cout << std::endl;
    }
}

template<typename T>
void alloc_fill_matrix(T* matrix, int n, int m){
    if (matrix != nullptr)
        throw;

    matrix = new T[n_size * n_size];
    for (int i = 0; i < n_size; ++i)
        for (int j = 0; j < n_size; ++j)
            matrix[i * n_size + j] = rand() % 10;
}

int main1_3_14(int argc, char **argv) {
    // Init


    /* srand(0); /*
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

    srand(time(nullptr));

    int root = 0;
    int n_size = 3;

    double* matrix_A = nullptr;

    double* matrix_B = nullptr;

    std::cout << "Matrix A:\n";
    print_matrix(matrix_A, n_size, n_size);
    std::cout << "\nMatrix B:\n";
    print_matrix(matrix_B, n_size, n_size);
    std::cout << std::endl;


    MPI_Init(&argc, &argv);

    int Size = 0;
    int Rank = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &Size);
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);


    auto matrix_C = new double[n_size * n_size]{};

    auto Line = new double[n_size]{};

    if (Rank != root) {

        for (int i = Rank; i < n_size; i += Size) {

            calculate_line(matrix_A, matrix_A, n_size, i, Line);
            calculate_line(matrix_B, matrix_B, n_size, i, Line, true);

            std::cout << Rank << " sending line " << i << " : ";
            for (int j = 0; j < n_size; ++j)
                std::cout << "[" << Line[j] << "]";
            std::cout << std::endl;

            MPI_Send(Line, n_size, MPI_DOUBLE, root, 0, MPI_COMM_WORLD);

            for (int j = 0; j < n_size; ++j)
                Line[j] = 0;
        }
    } else {
        // Receiving lines from other processes

        MPI_Status status;

        for (int i = 0; i < n_size; ++i) {
            int source = i % Size;
            if (source == 0)
                continue;

            std::cout << Rank << " (root) receiving line " << i << std::endl;
            MPI_Recv(&matrix_C[i * n_size], n_size, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &status);
            std::cout << Rank << " (root) got line " << i << " : ";
            for (int j = 0; j < n_size; ++j) {
                std::cout << "[" << matrix_C[i * n_size + j] << "]";
            }
            std::cout << std::endl;
        }

        // Calculating own lines

        for (int i = root; i < n_size; i += Size) {
            calculate_line(matrix_A, matrix_A, n_size, i, &matrix_C[i * n_size]);
            calculate_line(matrix_B, matrix_B, n_size, i, &matrix_C[i * n_size], true);
        }

        // Output matrix C

        std::cout << "Matrix C:\n";
        print_matrix(matrix_C, n_size, n_size);
        std::cout << std::endl;
    }

    delete[] matrix_A;
    delete[] matrix_B;
    delete[] matrix_C;
    delete[] Line;

    return MPI_Finalize();
}