#include "tasks.h"

using namespace std;

int main1_2_5(int argc, char **argv) {

    int Size;
    int Rank;

    int root = 0;
    int line = 1;
    int n = 5;
    int m = 5;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Size);


    int *matrix_A = new int[n * m];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix_A[i * m + j] = Rank;
        }
    }

    int *matrix_B = nullptr;
    if (Rank == root)
        matrix_B = new int[Size * m];

    MPI_Gather(&matrix_A[line * m], m, MPI_INT, matrix_B, m, MPI_INT, root, MPI_COMM_WORLD);

    if (Rank == root)
        for (int i = 0; i < Size; i++) {
            for (int j = 0; j < m; j++) {
                cout << matrix_B[i * m + j] << " ";
            }
            cout << endl;
        }

    return MPI_Finalize();
}