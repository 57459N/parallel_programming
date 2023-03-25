#include "tasks.h"


void print_vector(double *vector, int n) {
    for (int i = 0; i < n; i++) {
        std::cout << vector[i] << " ";
    }
}

void print_matrix(double *matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        print_vector(&matrix[i * m], m);
        std::cout << "\n";
    }
    std::cout.flush();
}

int main1_3_10(int argc, char **argv) {
    // Init

    // don't run more processes than n_size
    int n_size = 3;
    int root = 0;

    MPI_Init(&argc, &argv);

    int Size;
    int Rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Size);


    // Creating and initializing matrices and vectors

    double *matrix_A = nullptr;
    double *matrix_At = nullptr;
    auto vector_X = new double[n_size];
    auto vector_Y = new double[n_size];

    srand(time(nullptr));
    if (Rank == root) {
        matrix_A = new double[n_size * n_size]{};
        matrix_At = new double[n_size * n_size]{};
        for (int i = 0; i < n_size; i++) {
            vector_X[i] = rand() % 10;
            vector_Y[i] = rand() % 10;
            for (int j = 0; j < n_size; j++) {
                double v = rand() % 10;
                matrix_A[i * n_size + j] = v;
                matrix_At[j * n_size + i] = v;
            }

        }
        std::cout << "Matrix A:\n";
        print_matrix(matrix_A, n_size, n_size);
        std::cout << std::endl;

        std::cout << "Matrix At:\n";
        print_matrix(matrix_At, n_size, n_size);
        std::cout << std::endl;

        std::cout << "Vector X:\t";
        print_vector(vector_X, n_size);
        std::cout << std::endl;

        std::cout << "Vector Y:\t";
        print_vector(vector_Y, n_size);
        std::cout << std::endl;
    }


    // Calculating send and receive data

    auto recv_disps = new int[Size];
    auto recv_counts = new int[Size];
    auto send_disps = new int[Size];
    auto send_counts = new int[Size];

    for (int i = 0; i < Size; ++i) {
        // counting displacement
        int s_minus_mod = Size - (n_size % Size);
        recv_disps[i] = i * n_size / Size;
        if (i > s_minus_mod)
            recv_disps[i] += i - s_minus_mod;

        send_disps[i] = recv_disps[i] * n_size;

        //counting amount
        recv_counts[i] = n_size / Size + (i >= s_minus_mod);
        send_counts[i] = recv_counts[i] * n_size;
    }


    // Initializing arrays for matrices parts of each process'

    int counting_elements_amount = recv_counts[Rank];
    int mat_sizes = counting_elements_amount * n_size;
    auto matrix_A_lines = new double[mat_sizes];
    auto matrix_At_lines = new double[mat_sizes];


    // Sending each process his own matrix part

    MPI_Scatterv(matrix_A, send_counts, send_disps, MPI_DOUBLE, matrix_A_lines, mat_sizes, MPI_DOUBLE, root,
                 MPI_COMM_WORLD);
    MPI_Scatterv(matrix_At, send_counts, send_disps, MPI_DOUBLE, matrix_At_lines, mat_sizes, MPI_DOUBLE, root,
                 MPI_COMM_WORLD);


    // Sending vectors X aand Y

    MPI_Bcast(vector_X, n_size, MPI_DOUBLE, root, MPI_COMM_WORLD);
    MPI_Bcast(vector_Y, n_size, MPI_DOUBLE, root, MPI_COMM_WORLD);


    // Calculating own elements
    auto buffer = new double[counting_elements_amount]{};
    for (int i = 0; i < counting_elements_amount; i++) {
        double sum = 0;
        for (int k = 0; k < n_size; k++) {
            sum += matrix_A_lines[i * n_size + k] * vector_X[k];
            sum += matrix_At_lines[i * n_size + k] * vector_Y[k];
        }
        buffer[i] = sum;
    }


    // Gathering final vector

    auto recv_buff = new double[n_size];
    MPI_Gatherv(buffer, counting_elements_amount, MPI_DOUBLE, recv_buff, recv_counts, recv_disps, MPI_DOUBLE, root,
                MPI_COMM_WORLD);


    // Final vector output

    if (Rank == root) {
        std::cout << "\nFinal vector:\t";
        for (int i = 0; i < n_size; i++) {
            std::cout << recv_buff[i] << " ";
        }
        std::cout << std::endl;
    }


    // Cleanup

    delete[] vector_X;
    delete[] vector_Y;
    delete[] matrix_A;
    delete[] matrix_At;
    delete[] matrix_A_lines;
    delete[] matrix_At_lines;
    delete[] send_counts;
    delete[] send_disps;
    delete[] recv_counts;
    delete[] recv_disps;
    delete[] buffer;
    delete[] recv_buff;

    return MPI_Finalize();
}
