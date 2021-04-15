#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
void *work_thread(void * arg); 

int num_threads;
int max_num; 

int main(int argc, char *argv[])
{
	pthread_t *thread;
	int *index; 
	int sum = 0, i;
	void * ret;

	printf("Number of threads: ");
	scanf("%d", &num_threads);
	printf("Maximum integer (> Number of threads):");
	scanf("%d", &max_num);

	// TODO: pthread & index array
	thread = (pthread_t*) malloc(sizeof(pthread_t)*num_threads);
	index = (int*) malloc(sizeof(int)*num_threads);

	// TODO: pthread create	
	for (i = 0; i < num_threads; i++)
	{
		index[i] = i;
		pthread_create(&thread[i], NULL, work_thread, (void*)&index[i]);
	}

	// TODO: pthread_join
	for (i = 0; i < num_threads; i++)
	{
		pthread_join(thread[i], &ret);
		sum += *((int*)ret);
		free(ret);
	}
	
	
	printf("result: %d \n", sum);
	return 0;
}

void *work_thread(void *arg) 
{
	int idx = *((int*)arg);
	int i, start = idx * (max_num/num_threads), end = (idx+1) * (max_num/num_threads)-1;
	int * sum = (int*)malloc(sizeof(int));

	if(idx == num_threads-1) end += ((max_num+1)%num_threads);
	for(i = start; i <= end; i++)
		*sum += i;
	printf("thread result(%d, %d) : %d\n", start, end, *sum);
	return (void*)sum;
}
