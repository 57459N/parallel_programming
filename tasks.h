#pragma once
#include <iomanip>
#include <iostream>
#include "mpi.h"
#include "omp.h"

int main1_1_5(int, char**);
int main1_1_14(int, char**);
int main1_2_14(int, char**);
int main1_3_14(int, char**);
int main1_4_14(int, char**);

template<typename T>
void calculate_line(T *matrix_A, T *matrix_B, int matrix_inner_size, int line_number, T *Line, bool additive = false) {
    if (matrix_A == nullptr || matrix_B == nullptr)
        return;

    for (int j = 0; j < matrix_inner_size; ++j) {
        T sum = 0;
        if (additive)
            sum = Line[j];

        for (int k = 0; k < matrix_inner_size; ++k) {
            sum += matrix_A[line_number * matrix_inner_size + k] * matrix_B[k * matrix_inner_size + j];
        }
        Line[j] = sum;
    }
}

template<typename T>
void print_matrix(T *matrix, int n, int m) {
    if (matrix == nullptr)
        return;

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

    matrix = new T[n * m];
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            matrix[i * n + j] = rand() % 10;
}

