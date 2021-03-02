#include <thread>
#include <iostream>
#include <pthread.h>
#include <string>
#include <vector>
#include <mpi.h>
#include <sstream>
#include <algorithm>

using namespace std;

#define REQ_NUM_MPI_PROCESSES 5
#define MIN_LINES_PER_THREAD 20

#define MASTER 0
#define HORROR 1
#define COMEDY 2
#define FANTASY 3
#define SCIFI 4

void startMaster(string inputFile);
void startWorker(int type);

void doubleConsonants(int i, vector<string> &paragraphLines);
void capitalEven(int i, vector<string> &paragraphLines);
void capitalFirstLetter(int i, vector<string> &paragraphLines);
void reverseSeventh(int i, vector<string> &paragraphLines);
