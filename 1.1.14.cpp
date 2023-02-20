#include <iostream>
#include <mpi.h>
#include <string>
#include <cstring>

#include "tasks.h"


int get_next(int rank, int source, int dest, int size) {
    int len = dest - source;

    if (abs(len) <= size / 2) { // going directly, does not cross the start
        return rank + len / abs(len);
    } else {                       // cross the start
        return (rank + (-len / abs(len)) + size) % size;
    }
}

void I2J(int source, int dest, const std::string &message) {

    int myrank, size;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    if (myrank == source) {

        int next = get_next(myrank, source, dest, size);

        std::cout << source << " -> " << next;

        MPI_Send(message.data(), message.length(), MPI_CHAR, next, 0,
                 MPI_COMM_WORLD);

    } else if (myrank != dest) {
        char r_message[100]{};
        MPI_Recv(r_message, 100, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
                 &status);

        if (status.MPI_SOURCE == dest || status.MPI_TAG == 1)
            return;

        int next = get_next(myrank, source, dest, size);

        std::cout << myrank << " -> " << next;

        MPI_Send(r_message, strlen(r_message), MPI_CHAR, next, 0,
                 MPI_COMM_WORLD);
    } else {
        char r_message[100]{};

        int s = get_next(myrank, dest, source, size); // inverse and get prev element
        MPI_Recv(r_message, 100, MPI_CHAR, s, 0, MPI_COMM_WORLD, &status);

        std::cout << " : " << r_message << std::endl;

        if (abs(dest - source) <= size / 2) {
            for (int i = 0; i < std::min(source, dest); ++i)
                MPI_Send("", 0, MPI_CHAR, i, 1, // tag 1 to finish estimated elements
                         MPI_COMM_WORLD);
            for (int i = std::max(source, dest) + 1; i < size; ++i)
                MPI_Send("", 0, MPI_CHAR, i, 1, MPI_COMM_WORLD);
        } else
            for (int i = std::min(dest, source) + 1; i < std::max(dest, source); ++i)
                MPI_Send("", 0, MPI_CHAR, i, 1, MPI_COMM_WORLD);
    }
}


int main1_1_14(int argc, char **argv) {

    MPI_Init(&argc, &argv);

    I2J(0, 4, "asd");

    MPI_Finalize();

    return 0;
}