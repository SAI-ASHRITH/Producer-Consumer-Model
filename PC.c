#include<stdio.h>
#include<pthread.h>
#define MAX 50

int no=0;
int arr[50];
int frnt=-1;
int rear=0;

pthread_mutex_t lck= PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t pcv = PTHREAD_COND_INITIALIZER;
pthread_cond_t ccv = PTHREAD_COND_INITIALIZER;

void insert1(int);
int remove1();

void insert1(int val)
{
	no++;
	frnt=(frnt+1)%MAX;
	arr[frnt]=val;
}

int remove1()
{
	no--;
	int ret= arr[rear];
	rear=(rear+1)%MAX;
	return	ret;
}


void Producer(void * ptr)
{
	int i;
	
	for(i=0;i<500;i++)
	{
		pthread_mutex_lock(&lck);
		while(no==MAX)
		{	
			pthread_cond_wait(&pcv,&lck);
		}	
		insert1(i);		
		pthread_mutex_unlock(&lck);
		pthread_cond_signal(&ccv);
	}

}

void Consumer(void * ptr)
{
	int i;
	int cval;
	for(i=0;i<500;i++)
	{
		pthread_mutex_lock(&lck);
		while(no==0)
		{	
			pthread_cond_wait(&ccv,&lck);
		}	
		cval=remove1();		
		printf("%d\t",cval);
		pthread_mutex_unlock(&lck);
		pthread_cond_signal(&pcv);
	}
}


int main()
{
	int ptr;
	pthread_t pro;
	pthread_t con;
	
	int * pret;
	int * cret;

	pthread_create(&pro,NULL,(void *)Producer,(void *)&ptr);
	pthread_create(&con,NULL,(void *)Consumer,(void *)&ptr);

	pthread_join(pro,(void *)&pret);
	pthread_join(con,(void *)&cret);
}
