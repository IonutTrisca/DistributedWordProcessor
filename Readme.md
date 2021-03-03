# Distributed Word Processor
This program takes a file with different types of paragraphs and processes each one according to specific rules.
The processing is done in a distributed manner using MPI. Each type of paragraph has a thread in the master process, and a worker process which processes the paragraph.
The worker process has its own threads to process each paragraph in a parallel manner.

## Detailed Functionality
The program accepts 4 paragraph types and processes each one with a different worker.  
  
The processing is done as follows:
* **horror** -> each consonant is doubled
* **comedy** -> each even position letter is capitalized
* **fantasy** -> first letter of each word is capitalized
* **science-fiction** -> each 7th word of a sentence is reversed
