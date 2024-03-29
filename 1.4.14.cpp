#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"

#include <chrono>
//#include "vector"

#include "tasks.h"
//
//template<typename T>
//void fill_vector(std::vector<std::vector<T>> &matrix, int n, int m) {
//
//    for (int i = 0; i < n; ++i) {
//        matrix[i].resize(n);
//        for (int j = 0; j < m; ++j)
//            matrix[i][j] = rand() % 10;
//    }
//}
//
//
//template<typename T>
//void print_matrix(const std::vector<std::vector<T>> &matrix, int n, int m) {
//    for (int i = 0; i < n; ++i) {
//        for (int j = 0; j < m; ++j)
//            std::cout << std::setw(15) << matrix[i][j];
//        std::cout << std::endl;
//    }
//}

template<typename T>
void fill_pointer(T **&matrix, int n, int m) {
    matrix = new T *[n];
    for (int i = 0; i < n; ++i) {
        matrix[i] = new T[m];
        for (int j = 0; j < m; ++j)
            matrix[i][j] = rand() % 10;
    }
}


template<typename T>
void print_matrix(T **matrix, int n, int m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j)
            std::cout << std::setw(15) << matrix[i][j];
        std::cout << std::endl;
    }
}


uint64_t main1_4_14(int argc, char **argv, int n_size) {

    // with omp configuration argv[1] is number of threads

    int thread_num = atoi(argv[1]);
    omp_set_num_threads(thread_num);

    std::srand(time(nullptr));

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

//    std::vector<std::vector<double>> matrix_A(n_size);
//    std::vector<std::vector<double>> matrix_B(n_size);
//
//    fill_vector(matrix_B, n_size, n_size);
//    fill_vector(matrix_A, n_size, n_size);
//
//    std::vector<std::vector<double>> matrix_C_1(n_size);
//    std::vector<std::vector<double>> matrix_C_2(n_size);
//
//    for (auto &el: matrix_C_1) {
//        el.resize(n_size);
//    }
//    for (auto &el: matrix_C_2) {
//        el.resize(n_size);
//    }


    double **matrix_A = nullptr;
    double **matrix_B = nullptr;

    matrix_A = new double *[n_size];
    for (int i = 0; i < n_size; ++i) {
        matrix_A[i] = new double[n_size];
        for (int j = 0; j < n_size; ++j)
            matrix_A[i][j] = rand() % 10;
    }

    matrix_B = new double *[n_size];
    for (int i = 0; i < n_size; ++i) {
        matrix_B[i] = new double[n_size];
        for (int j = 0; j < n_size; ++j)
            matrix_B[i][j] = rand() % 10;
    }

    auto matrix_C_1 = new double *[n_size];

    for (int i = 0; i < n_size; ++i) {
        matrix_C_1[i] = new double[n_size]{};
    }

    if (n_size < 10) {
        std::cout << "Matrix A:\n";
        print_matrix(matrix_A, n_size, n_size);
        std::cout << "\nMatrix B:\n";
        print_matrix(matrix_B, n_size, n_size);
        std::cout << std::endl;
    }

    auto start_point = std::chrono::high_resolution_clock::now();

#pragma omp parallel for
    for (int i = 0; i < n_size; i++) {
        for (int j = 0; j < n_size; ++j) {
            double sum = 0;
            for (int k = 0; k < n_size; ++k) {
                sum += matrix_A[i][k] * matrix_A[k][j];
                sum += matrix_B[i][k] * matrix_B[k][j];
            }
            matrix_C_1[i][j] = sum;
        }
    }


    auto end_point = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(start_point).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_point).time_since_epoch().count();

    std::cout << (end - start) << " microseconds offset\n";

    for (int i = 0; i < n_size; ++i) {
        delete[] matrix_A[i];
        delete[] matrix_B[i];
        delete[] matrix_C_1[i];
    }

    delete[] matrix_A;
    delete[] matrix_B;
    delete[] matrix_C_1;

    return end - start;
}

#pragma clang diagnostic pop