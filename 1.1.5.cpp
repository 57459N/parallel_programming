#include <iostream>
#include <mpi.h>
#include <string>
#include <cstring>

using namespace std;

void fromItoJ(int i, int j, std::string message) {
    int Rank = 0;
    int Size = 0;
    MPI_Status status;

    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Size);

    if (Rank == j) {
        char received[100];
        MPI_Recv(received, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        cout << Rank << " > " << received;
    } else if (Rank == i) {
        if (Rank == 0){
            MPI_Send(message.data(), 100, MPI_CHAR, j, 0, MPI_COMM_WORLD);
        } else {
            MPI_Send(message.data(), 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }
    } else if (Rank == 0) {
        char received[100];
        MPI_Recv(received, 100, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
        MPI_Send(received, strlen(received), MPI_CHAR, j, 0, MPI_COMM_WORLD);
    }
}


void fromAlltoAll() {
    int Rank = 0;
    int Size = 0;
    MPI_Status status;

    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Size);


    if (Rank == 0) {

        for (int i = 0; i < (Size - 1) * (Size - 1); i++) {
            char r[100];
            MPI_Recv(r, 100, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

            string received = r;

            int delim_pos = received.find_first_of(":");
            int dest = stoi(received.substr(0, delim_pos)); // 12:Hello
            string message = received.substr(delim_pos + 1);

            if (dest == 0) {
                cout << Rank << " > '" << message << "'" << endl;
            } else {
                MPI_Send(message.c_str(), 100, MPI_CHAR, dest, 0, MPI_COMM_WORLD);
            }
        }

        for (int i = 1; i < Size; i++){
            string message = "Hello from 0 to " + to_string(i);
            MPI_Send(message.c_str(), 100, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }

    } else {

        for (int i = 0; i < Rank - 1; i++){
            char r[100];
            MPI_Recv(r, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
            cout << Rank << " > '" << r << "'" << endl;
        }

        for (int i = 0; i < Size; i++) {
            if (i == Rank)
                continue;

            string package;
            package += std::to_string(i);
            package += ":";
            package += "Hello from " + std::to_string(Rank) + " to " + to_string(i);

            MPI_Send(package.c_str(), 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }

        for (int i = 0; i < Size - Rank; i++) {
            char r[100];
            MPI_Recv(r, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
            cout << Rank << " > '" << r << "'" << endl;
        }
    }
}

int main1_1_5(int argc, char **argv) {

    MPI_Init(&argc, &argv);

//    fromItoJ(1, 2, "Hello world!");
    fromAlltoAll();

    MPI_Finalize();

    return 0;
}