//Christopher Shorter
//OS_HW7
//Problem 2

#include <stdio.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define N 1000

int global_index = 0;
sem_t mutex;
sem_t empty;
sem_t full;
int buffer[N/2];

void *child();

int main(){
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, N);
	sem_init(&full, 0, 0);

	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, child, NULL);

	int i=0;
	int buffer_index = 0;
	for(; i<N; i++){
		/*produce an item in next_produced */
		int next_produced = rand() % 10;
		printf("%d) Parent Iteration #%d. data = %d\n", global_index, i, next_produced);
		fflush(stdout);

		sem_wait(&empty);
		sem_wait(&mutex);

		/*add next_produced to buffer*/
		buffer[buffer_index] = next_produced;
		global_index++;

		sem_post(&mutex); 
		sem_post(&full);
		
		buffer_index++;
		if(buffer_index == N/2){
			buffer_index = 0;
		}
		
	}
	
	sleep(2);
	return 0;
}

void* child(){
	sleep(1);

	int i = 0;
	int buffer_index = 0;
	for(; i<N; i++){
		sem_wait(&full);
		sem_wait(&mutex);

		int next_consumed = buffer[buffer_index];
		buffer[buffer_index] = 0;
		
		global_index++;

		sem_post(&mutex);
		sem_post(&empty);
		
		printf("%d) Child Iteration #%d. data = %d\n", global_index, i, next_consumed);
		fflush(stdout);
		
		buffer_index++;
		if(buffer_index == N/2)
			buffer_index = 0;
	}
	
	return NULL;
}