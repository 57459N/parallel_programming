#include <cmath>
#include <numeric>

#include "tasks.h"


void print_vector(double *vector, int n);


void print_matrix(double *matrix, int n, int m);


int main1_3_5(int argc, char **argv) {

    // Init

    int n_size = 5;
    int root = 0;

    MPI_Init(&argc, &argv);

    int Size;
    int Rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Size);


    // Creating and initializing matrix A
    // Finding first norm of matrix_A

    auto matrix_A = new double[n_size * n_size]{};
    auto max_el = matrix_A[0];

    //    srand(time(nullptr));
    if (Rank == root) {
        for (int i = 0; i < n_size; ++i) {
            for (int j = 0; j < n_size; ++j)
                matrix_A[i * n_size + j] = rand() % 10;
            double sum = std::fabs(std::accumulate(&matrix_A[i * n_size], &matrix_A[(i + 1) * n_size], 0.));
            max_el = std::max(max_el, sum);
        }
        std::cout << "Matrix A:\n\n";
        print_matrix(matrix_A, n_size, n_size);
        std::cout << "\nfirst norm: " << max_el;
    }


    // Sending max element and matrix_A to all processes

    MPI_Bcast(&max_el, 1, MPI_DOUBLE, root, MPI_COMM_WORLD);
    MPI_Bcast(matrix_A, n_size * n_size, MPI_DOUBLE, root, MPI_COMM_WORLD);


    // Calculating displacements and amount of receiving data to matrix_B

    int div = n_size / Size;
    int counting_lines = n_size / Size + (Rank >= Size - (n_size % Size));

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


    // Calculating each process' lines and store them into buffer

    auto buffer = new double[n_size * counting_lines]{};
    int start_line = recv_disps[Rank] / n_size;
    for (int i = start_line; i < start_line + counting_lines; ++i) {
        for (int j = 0; j < n_size; ++j) {
            double sum = 0;
            for (int k = 0; k < n_size; ++k) {
                sum += matrix_A[i * n_size + k] * matrix_A[k * n_size + j];
            }
            sum *= max_el;
            buffer[(i - start_line) * n_size + j] = sum;
        }
    }


    // Creating and receiving matrix B

    double *matrix_B = nullptr;
    if (Rank == root) {
        matrix_B = new double[n_size * n_size];
    }

    MPI_Gatherv(buffer, recv_counts[Rank], MPI_DOUBLE, matrix_B, recv_counts, recv_disps, MPI_DOUBLE, root,
                MPI_COMM_WORLD);


    // Printing final matrix_B

    if (Rank == root) {
        std::cout << "\n\nMatrix B = A^2 * norm1(A):\n\n";
        print_matrix(matrix_B, n_size, n_size);
    }


    // Cleanup

    delete[] matrix_A;
    delete[] buffer;
    delete[] recv_disps;
    delete[] recv_counts;
    delete[] matrix_B;

    return MPI_Finalize();
}
