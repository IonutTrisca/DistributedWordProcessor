build:
	mpicxx -Wall worker.cpp main.cpp master.cpp scifi.cpp horror.cpp comedy.cpp fantasy.cpp -o main

clean:
	rm -rf main
