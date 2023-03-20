#include <chrono>
#include "tasks.h"

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
        std::cout << "\n";
    }
    std::cout.flush();
}

template<typename T>
void alloc_fill_matrix(T *matrix, int n, int m) {
    if (matrix != nullptr)
        throw;


}

uint64_t main1_3_14(int argc, char **argv) {
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

    srand(0); //time(nullptr)

    int root = 0;
    int n_size = 500;

    MPI_Init(&argc, &argv);

    int Size = 0;
    int Rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Size);


    double *matrix_A = nullptr;
    double *matrix_B = nullptr;

    matrix_A = new double[n_size * n_size];
    for (int i = 0; i < n_size; ++i)
        for (int j = 0; j < n_size; ++j)
            matrix_A[i * n_size + j] = rand() % 10;

    matrix_B = new double[n_size * n_size];
    for (int i = 0; i < n_size; ++i)
        for (int j = 0; j < n_size; ++j)
            matrix_B[i * n_size + j] = rand() % 10;


    if (Rank == root && n_size < 10) {
        std::cout << "Matrix A:\n";
        print_matrix(matrix_A, n_size, n_size);
        std::cout << "\nMatrix B:\n";
        print_matrix(matrix_B, n_size, n_size);
        std::cout << std::endl;
    }


    double *matrix_C = nullptr;
    if (Rank == root)
        matrix_C = new double[n_size * n_size]{};


    auto start_point = std::chrono::high_resolution_clock::now();


    int div = n_size / Size;
    int counting_lines = n_size / Size + (Rank >= Size - (n_size % Size));

    auto buffer = new double[n_size * counting_lines]{};

    auto recv_disps = new int[Size];
    auto recv_counts = new int[Size];
    for (int i = 0; i < Size; ++i) {
        // counting displacement
        int s_minus_mod = Size - (n_size % Size);
        recv_disps[i] = i * div;
        if (i > s_minus_mod)
            recv_disps[i] += i - s_minus_mod;
        recv_disps[i] *= n_size;

        //counting amount
        recv_counts[i] = n_size * (n_size / Size + (i >= s_minus_mod));
    }

    // Calculating buffer

    int start_line = recv_disps[Rank] / n_size;
    for (int i = start_line; i < start_line + counting_lines; ++i) {
        for (int j = 0; j < n_size; ++j) {
            double sum = 0;
            for (int k = 0; k < n_size; ++k) {
                sum += matrix_A[i * n_size + k] * matrix_A[k * n_size + j];
                sum += matrix_B[i * n_size + k] * matrix_B[k * n_size + j];
            }
            buffer[(i - start_line) * n_size + j] = sum;
        }
    }

    MPI_Gatherv(buffer, recv_counts[Rank], MPI_DOUBLE, matrix_C, recv_counts, recv_disps, MPI_DOUBLE, root,
                MPI_COMM_WORLD);

    auto end_point = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(start_point).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_point).time_since_epoch().count();


    if (Rank == root) {
        std::cout << (end - start) << std::endl;
        if (n_size < 10) {
            std::cout << "Matrix C:\n";
            print_matrix(matrix_C, n_size, n_size);
            std::cout << std::endl;
        }
    }

    delete[] matrix_A;
    delete[] matrix_B;
    delete[] matrix_C;
    delete[] recv_disps;
    delete[] recv_counts;


    return MPI_Finalize();

}