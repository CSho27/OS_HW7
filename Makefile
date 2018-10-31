all: prob1.o prob2.o prob3.o
	gcc -o prob1 prob1.o -lrt -pthread
	gcc -o prob2 prob2.o -lrt -pthread
	gcc -o prob3 prob3.o -lrt -pthread
	
prob1.o: prob1.c
	gcc -Wall -Werror -g -c prob1.c -fopenmp
	
prob2.o: prob2.c
	gcc -Wall -Werror -g -c prob2.c -fopenmp

prob3.o: prob3.c
	gcc -Wall -Werror -g -c prob3.c -fopenmp

	
clean:
	rm -f *.o
	rm -f prob1
	rm -f prob2
	rm -f prob3