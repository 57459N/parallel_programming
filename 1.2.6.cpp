#include "tasks.h"

int main1_2_6(int argc, char **argv) {
    int rank;
    int size;

    int k = 0;

    int *buf = nullptr;

    const int a = 5, b = 5;
    int M[a][b];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < a; ++i) {
        for (int j = 0; j < b; ++j) {
            M[i][j] = rank;
        }
    }

    int *sendbuf = new int[a];
    for (int i = 0; i < a; ++i) {
        sendbuf[i] = M[k][i];
    }


    buf = new int[size * a]{};

    MPI_Gather(M, 5, MPI_INT, buf, 5, MPI_INT, k, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < a; ++i) {
            for (int j = 0; j < size; ++j) {
                std::cout << buf[i * a + j] << " ";
            }
            std::cout << std::endl;
        }
    }

    delete[] buf;

    return MPI_Finalize();
}