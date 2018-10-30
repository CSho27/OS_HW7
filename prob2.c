//Christopher Shorter
//OS_HW7
//Problem 2

#include <stdio.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define N 25

int order[2*N];
int global_index = 0;
sem_t mutex;
sem_t empty;
sem_t full;
int buffer[N/2];

void* child(void* id);
void* parent(void* id);

int main(){
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, N);
	sem_init(&full, 0, 0);

	pid_t pid;
	pid = fork();
	
	void* id;
	if(pid == 0)
		id = "2";
	else
		id = "1";
	
	pthread_t parent_tid;
	pthread_attr_t parent_attr;
	pthread_attr_init(&parent_attr);
	pthread_create(&parent_tid, &parent_attr, parent, id);
	
	pthread_t child_tid;
	pthread_attr_t child_attr;
	pthread_attr_init(&child_attr);
	pthread_create(&child_tid, &child_attr, child, id);

	
	sleep(2);
	
	int i = 0;
	for(; i<N*2; i++){
		printf("%d\n", order[i]);
		fflush(stdout);
	}
	sleep(2);
	return 0;
}

void* child(void* id){
	sleep(1);

	int i = 0;
	int buffer_index = 0;
	for(; i<N; i++){
		sem_wait(&full);
		sem_wait(&mutex);

		int next_consumed = buffer[buffer_index];
		buffer[buffer_index] = 0;
		
		if(strcmp((char*) id, "1"))
			order[global_index] = 3;
		else
			order[global_index] = 4;
		
		global_index++;

		sem_post(&mutex);
		sem_post(&empty);
		
		printf("Consumer #%s, iteration #%d. data = %d\n", (char*) id, i, next_consumed);
		fflush(stdout);
		
		buffer_index++;
		if(buffer_index == N/2)
			buffer_index = 0;
	}
		   
	return NULL;
}

void* parent(void* id){
	int i=0;
	int buffer_index = 0;
	for(; i<N; i++){
		/*produce an item in next_produced */
		int next_produced = rand() % 10;
		printf("Producer #%s, iteration #%d. data = %d\n", (char*) id, i, next_produced);
		fflush(stdout);

		sem_wait(&empty);
		sem_wait(&mutex);

		/*add next_produced to buffer*/
		buffer[buffer_index] = next_produced;
		
		if(strcmp((char*) id, "1"))
			order[global_index] = 1;
		else
			order[global_index] = 2;
		global_index++;

		sem_post(&mutex); 
		sem_post(&full);
		
		buffer_index++;
		if(buffer_index == N/2){
			buffer_index = 0;
		}
		
	}
	return NULL;
}