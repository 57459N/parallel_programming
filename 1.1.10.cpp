#include "tasks.h"

void fromI2J(int i, int j, int data) {
    int Size = 0;
    int Rank = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Size);

    if (i > j) {
        if (Rank == i) {
            MPI_Send(&data, 1, MPI_INT, i - 1, 0, MPI_COMM_WORLD);
            std::cout << Rank << ": i've sent " << data << "\n";
        } else if (Rank == j) {
            int r_data = 0;
            MPI_Status status;
            MPI_Recv(&r_data, 1, MPI_INT, j + 1, 0, MPI_COMM_WORLD, &status);
            std::cout << Rank << ":i've got " << r_data << "\n";
        } else if (Rank < i and Rank > j) {
            int r_data = 0;
            MPI_Status status;
            MPI_Recv(&r_data, 1, MPI_INT, Rank + 1, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&r_data, 1, MPI_INT, Rank - 1, 0, MPI_COMM_WORLD);
            std::cout << Rank << ": i've transferred " << r_data << "\n";
        }
    } else if (i < j) {
        if (Rank == i) {
            MPI_Send(&data, 1, MPI_INT, i + 1, 0, MPI_COMM_WORLD);
            std::cout << Rank << ": i've sent " << data << "\n";
        } else if (Rank == j) {
            int r_data = 0;
            MPI_Status status;
            MPI_Recv(&r_data, 1, MPI_INT, j - 1, 0, MPI_COMM_WORLD, &status);
            std::cout << Rank << ": i've got " << r_data << "\n";
        } else if (Rank > i and Rank < j) {
            int r_data = 0;
            MPI_Status status;
            MPI_Recv(&r_data, 1, MPI_INT, Rank - 1, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&r_data, 1, MPI_INT, Rank + 1, 0, MPI_COMM_WORLD);
            std::cout << Rank << ": i've transeferred " << r_data << "\n";
        }
    }
}

int main1_1_10(int argc, char **argv) {


    MPI_Init(&argc, &argv);

    int Size = 0;
    int Rank = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Size);

    // from i to j
    int I = 1;
    int J = 2;
    int DATA = 777;

//    fromI2J(I, J, DATA);

    std::cout << "===============================================" << std::endl;

    // from all to all
    for (int i = 0; i < Size; i++){
        for (int j = 0; j < Size; j++){
            if (i == j)
                continue;
            int data = i * 100 + j;
            fromI2J(i, j, data);
        }
    }

    std::cout.flush();

    MPI_Finalize();
    return 0;
}