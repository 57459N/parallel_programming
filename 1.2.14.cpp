#include <iostream>
#include "mpi.h"
#include "iomanip"
#include "ostream"

#include "tasks.h"

void with_SendRecv(int root, int n_size, int m_size, int chosen_line, int Rank, int Size, double **matrix_A) {
    auto *matrix_B = new double[Size * m_size]{};

    MPI_Status status;

    if (Rank == root) {
        for (int i = 0; i < Size; ++i) {
            if (i == root)
                continue;

            MPI_Recv(&matrix_B[i * m_size], m_size - i, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
        }

        for (int i = 0; i < m_size - Rank; ++i) {
            matrix_B[Rank * m_size + i] = matrix_A[chosen_line][i];
        }

        for (int i = 0; i < Size; ++i) {
            for (int j = 0; j < m_size; ++j) {
                std::cout << std::setw(5) << matrix_B[i * m_size + j];
            }
            std::cout << std::endl;
        }
    } else {
        MPI_Send(matrix_A[chosen_line], m_size - Rank, MPI_DOUBLE, root, 0, MPI_COMM_WORLD);
    }

    delete[] matrix_B;
}

void with_Gatherv(int root, int n_size, int m_size, int chosen_line, int Rank, int Size, double **matrix_A) {
    // Matrices and arrays

    auto *matrix_B = new double[Size * m_size]{};

    auto *array_counts = new int[Size];
    for (int i = 0; i < Size; ++i) {
        array_counts[i] = m_size - i;
    }

    auto *array_disp = new int[Size];
    for (int i = 0; i < Size; ++i) {
        array_disp[i] = i * m_size;
    }

    // Debugging: showing matrix A

//    for (int i = 0; i < n_size; ++i) {
//        std::cout << Rank << ":" << i << " - ";
//        for (int j = 0; j < m_size; ++j) {
//            std::cout << std::setw(5) << matrix_A[i][j];
//        }
//        std::cout << std::endl;
//    }

    // Sending

    MPI_Gatherv(matrix_A[chosen_line],
                m_size - Rank, MPI_DOUBLE,
                matrix_B,
                (const int *) array_counts,
                (const int *) array_disp,
                MPI_DOUBLE, root,
                MPI_COMM_WORLD);

    // Receiving

    if (Rank == root) {
        for (int i = 0; i < Size; ++i) {
            for (int j = 0; j < m_size; ++j) {
                std::cout << std::setw(5) << matrix_B[i * m_size + j];
            }
            std::cout << std::endl;
        }
    }

    // Cleanup

    delete[] matrix_B;
    delete[] array_counts;
    delete[] array_disp;
}


int main1_2_14(int argc, char **argv) {

    // Init

    int root = 0;
    int n_size = 3;
    int m_size = 3;
    int chosen_line = 1;

    MPI_Init(&argc, &argv);

    int Rank = 0;
    int Size = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &Size);
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

    // Matrix A

    auto **matrix_A = new double *[n_size]{};
    for (int i = 0; i < n_size; ++i) {
        matrix_A[i] = new double[m_size];
        for (int j = 0; j < m_size; ++j) {
            matrix_A[i][j] = Rank + i * m_size + j;
        }
    }

    // Execution

    //with_Gatherv(root, n_size, m_size, chosen_line, Rank, Size, matrix_A);
    with_SendRecv(root, n_size, m_size, chosen_line, Rank, Size, matrix_A);

    // Cleanup

    for (int i = 0; i < n_size; ++i) {
        delete[] matrix_A[i];
    }
    delete[] matrix_A;

    return MPI_Finalize();
}




//int main1_2_14(int argc, char **argv) {
//    int buffer[6]{};
//    int rcv_buffer[6]{};
//    int rank, size, i;
//    int receive_counts[4] = {0, 1, 2, 3};
//    int receive_displacements[4] = {0, 0, 1, 3};
//
//    MPI_Init(&argc, &argv);
//    MPI_Comm_size(MPI_COMM_WORLD, &size);
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    if (size != 4) {
//        if (rank == 0) {
//            printf("Please run with 4 processes\n");
//            fflush(stdout);
//        }
//        MPI_Finalize();
//        return 0;
//    }
//    for (i = 0; i < rank; i++) {
//        buffer[i] = rank;
//    }
//    MPI_Gatherv(buffer, rank, MPI_INT, rcv_buffer, receive_counts, receive_displacements, MPI_INT, 0, MPI_COMM_WORLD);
//    if (rank == 0) {
//        for (i = 0; i < 6; i++) {
//            printf("[%d]", rcv_buffer[i]);
//        }
//        printf("\n");
//        fflush(stdout);
//    }
//    return MPI_Finalize();
//}
