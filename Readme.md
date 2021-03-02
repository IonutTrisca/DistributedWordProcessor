# Distributed Word Processor
This program takes a file with different types of paragraphs and processes each one according to specific rules.
The processing is done in a distributed manner using MPI, each type of paragraph has a thread in the master process, and a worker process which processes, in a parallel manner, the paragraph.
