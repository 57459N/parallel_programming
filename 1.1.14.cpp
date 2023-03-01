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

void ItoAll(int source) {

    int Size = 0;
    int Rank = 0;
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &Size);
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

    if (Rank == source) {
        char m[] = "Hello World!";
        MPI_Request requests[2];
        MPI_Status statuses[2];

        MPI_Isend(m, strlen(m), MPI_CHAR, (Rank + 1) % Size, 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Isend(m, strlen(m), MPI_CHAR, (Rank - 1 + Size) % Size, 0, MPI_COMM_WORLD, &requests[1]);
        MPI_Waitall(2, requests, statuses);
        std::cout << "Source after wait" << std::endl;
    } else {
        char r_message[100];

        MPI_Request requests[2];

        MPI_Irecv(r_message, 100, MPI_CHAR, (Rank + 1) % Size, 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Irecv(r_message, 100, MPI_CHAR, (Rank - 1 + Size) % Size, 0, MPI_COMM_WORLD, &requests[1]);

        int idx;
        MPI_Waitany(2, requests, &idx, &status);


        char s_message[100];
        strcpy_s(s_message, r_message);

        if (idx == 0) {
            MPI_Irecv(r_message, 100, MPI_CHAR, (Rank - 1 + Size) % Size, 0, MPI_COMM_WORLD, &requests[0]);
            MPI_Isend(s_message, 100, MPI_CHAR, (Rank - 1 + Size) % Size, 0, MPI_COMM_WORLD, &requests[1]);
        } else {
            MPI_Irecv(s_message, 100, MPI_CHAR, (Rank + 1) % Size, 0, MPI_COMM_WORLD, &requests[0]);
            MPI_Isend(r_message, 100, MPI_CHAR, (Rank + 1) % Size, 0, MPI_COMM_WORLD, &requests[1]);
        }

        MPI_Waitany(2, requests, &idx, &status);
        std::cout << "status : " << "error : " << status.MPI_ERROR << " source : " << status.MPI_SOURCE << std::endl;

        std::cout << Rank << " > " << r_message << std::endl;

    }
//    std::cout << "ItToAll" << std::endl;

}

int main1_1_14(int argc, char **argv) {

    MPI_Init(&argc, &argv);

//    I2J(0, 4, "asd");
    ItoAll(0);
    std::cout << " Finalize " << std::endl;
    MPI_Finalize();

    return 0;
}