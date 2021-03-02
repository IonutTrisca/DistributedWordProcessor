#include <iostream>
#include <mpi.h>

#include "tema3.h"

int main(int argc, char *argv[]) {
    int numProcesses, rank;
    int provided;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);

    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (numProcesses != REQ_NUM_MPI_PROCESSES) {
        cout << "Wrong number of processes. Only 5 allowed!\n";
        MPI_Finalize();
        return 0;
    }

    if (argc != 2) {
        cout << "The program needs an input file as argument!\n";
        MPI_Finalize();
        return 0;
    }

    if(provided != MPI_THREAD_MULTIPLE) {
        cout << "Parallel MPI not provided!\n";
        MPI_Finalize();
        return 0;
    }
    
    // Decide which process is master and which are workers
    switch (rank) {
        case MASTER: startMaster(string(argv[1]));
            break;

        case HORROR: startWorker(HORROR);
            break;

        case COMEDY: startWorker(COMEDY);
            break;

        case FANTASY: startWorker(FANTASY);
            break;

        case SCIFI: startWorker(SCIFI);
            break;
    }

    MPI_Finalize();

    return 0;
}
