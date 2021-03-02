#include "tema3.h"

static vector<string> paragraphLines;
static int P = std::thread::hardware_concurrency() - 1;
static int genre;

void * workerThread(void * arg) {
    vector<int>& ids = *reinterpret_cast<vector<int>*>(arg);
    
    // process each required batch of 20 lines
    for (auto id : ids) {
        int start = id * MIN_LINES_PER_THREAD;
        int end = min((double)start + MIN_LINES_PER_THREAD, (double)paragraphLines.size());

        for (int i = start; i < end; i++) {
            // decide how to process each line based on the genre
            switch(genre) {
                case HORROR: doubleConsonants(i, paragraphLines);
                    break;

                case COMEDY: capitalEven(i, paragraphLines);
                    break;
                
                case FANTASY: capitalFirstLetter(i, paragraphLines);
                    break;

                case SCIFI: reverseSeventh(i, paragraphLines);
                    break;
            }
        }
    }

    pthread_exit(NULL);
}

string processParagraph(char * paragraph) {
    stringstream paragraphStream(paragraph);
    string line;

    // delete the previous paragraph
    paragraphLines.erase(paragraphLines.begin(), paragraphLines.end());

    // split the paragraph into a vector of lines
    while (getline(paragraphStream, line, '\n')) {
        paragraphLines.push_back(line);
    }

    pthread_t tid[P];
    vector<vector<int>> thread_ids(P);

    // decide which portions of 20 lines each thread will process
    int createdThreads = 0;
    int remainingLines = paragraphLines.size();
    int neededThreads = remainingLines / MIN_LINES_PER_THREAD + 
                        ((remainingLines % MIN_LINES_PER_THREAD != 0) ? 1 : 0);

    while (createdThreads != neededThreads) {
        int currentBatchThreads = 0;

        while (remainingLines > 0 && currentBatchThreads != P) {
            thread_ids[currentBatchThreads].push_back(createdThreads);
            remainingLines -= MIN_LINES_PER_THREAD;
            createdThreads++;
            currentBatchThreads++;
        }
    }

    if (createdThreads >= P)
        createdThreads = P;

    // start the required P threads, P = number of available threads - 1
    for (int i = 0; i < createdThreads; i++) {
        pthread_create(&tid[i], NULL, workerThread, &thread_ids[i]);
    }

    // join the worker threads
    for (int i = 0; i < createdThreads; i++) {
        pthread_join(tid[i], NULL);
    }

    string newParagraph;

    // join the vector of processed lines back into a single string
    for (uint64_t i = 0; i < paragraphLines.size(); i++) {
        newParagraph += paragraphLines[i] + "\n";
    }
    
    return newParagraph;
}

void * readerThread(void * arg) {
    int type = *(int *)arg;
    int paragraphSize;

    // receive paragraphs until one of zero size is sent
    while (true) {
        MPI_Recv(&paragraphSize, 1, MPI_INT, MASTER, type, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if(paragraphSize == 0)
            break;
            
        char *paragraph = (char *)calloc(paragraphSize + 1, sizeof(char));
        MPI_Recv(paragraph, paragraphSize, MPI_CHAR, MASTER, type, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        // process the paragraph
        string stringParagraph = processParagraph(paragraph);
        paragraphSize = stringParagraph.size();

        // send the size of the paragraph and the paragraph back to the master
        MPI_Send(&paragraphSize, 1, MPI_INT, MASTER, type, MPI_COMM_WORLD);
        MPI_Send(stringParagraph.c_str(), stringParagraph.size(), MPI_CHAR, MASTER, type, MPI_COMM_WORLD);

        free(paragraph);
    }

    pthread_exit(NULL);
}

void startWorker(int type) {
    genre = type;
    pthread_t reader;

    // start the reader thread
    pthread_create(&reader, NULL, readerThread, &type);

    // join the reader thread
    pthread_join(reader, NULL);
}