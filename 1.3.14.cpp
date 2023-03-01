#include <iomanip>
#include <iostream>
#include "mpi.h"

int main1_3_14(int argc, char **argv) {
    // Init

    srand(time(nullptr));

    int root = 0;
    int n_size = 3;

    MPI_Init(&argc, &argv);

    int Size = 0;
    int Rank = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &Size);
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

    auto matrix_A = new int[n_size * n_size];
    for (int i = 0; i < n_size; ++i) {
        for (int j = 0; j < n_size; ++j) {
            matrix_A[i * n_size + j] = rand() % 10;
        }
    }

    auto matrix_B = new int[n_size * n_size];
    for (int i = 0; i < n_size; ++i) {
        for (int j = 0; j < n_size; ++j) {
            matrix_B[i * n_size + j] = rand() % 10;
        }
    }

    auto matrix_C = new int[n_size * n_size]{};


    auto Line = new double[n_size]{};

    if (Rank != root) {
        for (int i = Rank; i < n_size; i += Size) {
            for (int j = 0; j < n_size; ++j) {
                for (int k = 0; k < n_size; ++k) {
                    Line[j] += matrix_A[i * n_size + k] * matrix_A[k * n_size + j];
                }
            }
            std::cout << Rank << " sending line " << i << " : ";
            for (int j = 0; j < n_size; ++j){
                std::cout << "[" << Line[j] << "]";
            }
            std::cout << std::endl;
            MPI_Send(Line, n_size, MPI_DOUBLE, root, 0, MPI_COMM_WORLD);

            for (int f = 0; f < n_size; ++f) {
                Line[f] = 0;
            }
        }
    } else {
        MPI_Status status;

        for (int i = 0; i < n_size; ++i) {
            int source = i % Size;
            if (source == 0)
                continue;

            std::cout << Rank << " (root) receiving line " << i << std::endl;
            MPI_Recv(&matrix_C[i * n_size], n_size, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &status);
            std::cout << Rank << " (root) got line " << i << " : ";
            for (int j = 0; j < n_size; ++j){
                std::cout << "[" << matrix_C[i * n_size + j] << "]";
            }
            // Проблема в адресации матрицы C
            // Нужно использовать массовый забор с адресацией
            // Смотреть преддущее задание с Gatherv
            std::cout << std::endl;
        }

        for (int i = 0; i < n_size; ++i) {
            for (int j = 0; j < n_size; ++j) {
                std::cout << std::setw(15) << matrix_C[i * n_size + j];
            }
            std::cout << std::endl;
        }
    }

    delete[] matrix_A;
    delete[] matrix_B;
    delete[] matrix_C;
    delete[] Line;

    return MPI_Finalize();
}
