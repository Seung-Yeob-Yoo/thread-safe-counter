#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <time.h>

#define PATH "/mnt/d/homework/thread-saft-counter/key"

union semun
{
	int val;
	struct semid_ds* buf;
	ushort* array;
};

typedef struct __counter_t
{
	key_t key;
	int value;
	int semid;
	union semun arg;
} counter_t;

unsigned int loop_cnt;

counter_t counter;

int init(counter_t* c)
{
	c->key = ftok(PATH, 'z');
	c->value = 0;
	c->semid = semget(c->key, 1, 0600 | IPC_CREAT);
	c->arg.val = 1;
	semctl(c->semid, 0, SETVAL, c->arg);
}

void increment(counter_t* c)
{
	struct sembuf s;
	
	s.sem_num = 0;
	s.sem_op = -1;
	s.sem_flg = 0;
	semop(c->semid, &s, 1);
	
	c->value++;
	
	s.sem_num = 0;
	s.sem_op = 1;
	s.sem_flg = 0;
	semop(c->semid, &s, 1);
}

void decrement(counter_t* c)
{

	struct sembuf s;
	
	s.sem_num = 0;
	s.sem_op = -1;
	s.sem_flg = 0;
	semop(c->semid, &s, 1);
	
	c->value--;
	
	s.sem_num = 0;
	s.sem_op = 1;
	s.sem_flg = 0;
	semop(c->semid, &s, 1);
}

int get(counter_t* c)
{
	struct sembuf s;
	
	s.sem_num = 0;
	s.sem_op = -1;
	s.sem_flg = 0;
	semop(c->semid, &s, 1);
	
	int rc = c->value;
	
	s.sem_num = 0;
	s.sem_op = 1;
	s.sem_flg = 0;
	semop(c->semid, &s, 1);
	return rc;
}

void* mythread(void* arg)
{
	char* letter = arg;
	int a;
	printf("%s: begin\n", letter);
	for (a = 0; a < loop_cnt; a++)
	{
		increment(&counter);
	}
	printf("%s: done\n", letter);
	return NULL;
}

int main(int argc, char* argv[])
{
	clock_t start, end;
	float result;

	loop_cnt = atoi(argv[1]);
	init(&counter);

	printf("semid=%d\n", counter.semid);
	
	start = clock(); /*시간 측정 시작*/
	pthread_t p1, p2;

	printf("main: begin [counter = %d]\n", get(&counter));

	pthread_create(&p1, NULL, mythread, "A");
	pthread_create(&p2, NULL, mythread, "B");

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	end=clock(); /*시간 측정 종료*/

	result = (float)(end-start)/CLOCKS_PER_SEC;

	printf("main: done [counter: %d] [should be: %d]\n", get(&counter), loop_cnt * 2);
	printf("소요시간 : %.3f\n", result);

	return 0;
}
