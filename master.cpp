#include "tema3.h"
#include <fstream>

pthread_mutex_t lock; 

static string inputFile;
static string outputParagraph;
static int crtParagraphNum = 0;

void * paragraphThread(void * arg) {
    int id = *(int *)arg + 1;
    int paragraphCount = 0;

    string paragraphType;

    // decide what paragraph type each thread is going to read
    switch (id) {
        case HORROR: paragraphType = "horror";
            break;

        case COMEDY: paragraphType = "comedy";
            break;

        case FANTASY: paragraphType = "fantasy";
            break;

        case SCIFI: paragraphType = "science-fiction";
            break;
    }

    // open the input file
    ifstream inFile(inputFile);
    
    string line;
    string paragraph;

    // read the input file line by line
    while(getline(inFile, line)) {
        // if the read line is the beginning of a paragraph
        // increment the paragraph count and check if it is the current
        // thread's type of paragraph
        if (line == "horror" || line == "fantasy" || 
            line == "comedy" || line == "science-fiction") {
            
            if(line == paragraphType) {
                paragraph = "";

                
                // read every line until the end of a paragraph
                // and put it in a string to be sent to the worker
                while(getline(inFile, line)) {
                    if (line == "") {
                        break;
                    }
                    paragraph += line + "\n";
                }

                int paragraphSize = paragraph.size();
                // send the size of the paragraph then the paragraph itself to the worker
                MPI_Send(&paragraphSize, 1, MPI_INT, id, id, MPI_COMM_WORLD);
                MPI_Send(paragraph.c_str(), paragraph.size(), MPI_CHAR, id, id, MPI_COMM_WORLD);

                // receive the size of the processed paragraph and the paragraph itself from the worker
                MPI_Recv(&paragraphSize, 1, MPI_INT, id, id, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                char *charParagraph = (char *)calloc(paragraphSize + 1, sizeof(char));
                MPI_Recv(charParagraph, paragraphSize, MPI_CHAR, id, id, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                bool written = false;

                // wait for the pragraph to be written in the correct place
                while (!written) {
                    pthread_mutex_lock(&lock);
                    if(paragraphCount == crtParagraphNum) {
                        outputParagraph += paragraphType;
                        outputParagraph +=  "\n" ;
                        outputParagraph += charParagraph;
                        outputParagraph +=  "\n";
                        crtParagraphNum++;
                        written = true;
                    }
                    pthread_mutex_unlock(&lock); 
                }

                free(charParagraph);
            }

            paragraphCount++;
        }
    }

    // send the zero size paragraph to the worker thread as a shutdown signal
    int paragraphSize = 0;
    MPI_Send(&paragraphSize, 1, MPI_INT, id, id, MPI_COMM_WORLD);

    inFile.close();

    pthread_exit(NULL);
}


void startMaster(string inFile) {
    int thread_id[REQ_NUM_MPI_PROCESSES - 1];
	pthread_t tid[REQ_NUM_MPI_PROCESSES - 1];
    
    inputFile = inFile;

    if (pthread_mutex_init(&lock, NULL) != 0) { 
        printf("Mutex init has failed\n"); 
        return; 
    }

    // start and close the 4 master threads
    for (int i = 0; i < REQ_NUM_MPI_PROCESSES - 1; i++) {
		thread_id[i] = i;
		pthread_create(&tid[i], NULL, paragraphThread, &thread_id[i]);
	}

    for (int i = 0; i < REQ_NUM_MPI_PROCESSES - 1; i++) {
		pthread_join(tid[i], NULL);
    }
    

    // write the final string containing all the paragraphs to the output file
    string delimiter = ".txt";
    string outputFile = inputFile.substr(0, inputFile.find(delimiter));
    outputFile += ".out";

    ofstream outFile(outputFile);
    outFile << outputParagraph;
    outFile.close();

    pthread_mutex_destroy(&lock);
}
