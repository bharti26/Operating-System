/* Name Bharti Goel
#U20683088
*/



/*#define _REENTRANT*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
struct rusage mytiming;
struct timeval mytimeval;
/*Tested on linux 1/23/11 */
/* compile with gcc -ofname thread-template.c -lpthread */
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER;

struct shared_dat
   {
   int value;     /* shared variable to store result*/
   };


struct shared_dat  *counter;
int temp;

/****************************************************************
* This function increases the value of shared variable "counter"
  by one 2000000 times
****************************************************************/
void * thread1(void *arg)
{	int count=0; /* for how many times thread 1 executes.*/
	int line = 0;	
	while (line < 2000000)
	   {
            

/* Critical Section */
	
	
	/*pthread_cond_wait( &condition_var, &mutex1 );*/
	    if((counter->value%100)==0){
		line=line+100;
		pthread_mutex_trylock( &mutex1 );
	    	counter->value = counter->value + 100;
		count++;
	    	counter->value = counter->value * 2;
	    	counter->value = counter->value / 2;
		pthread_mutex_unlock( &mutex1 );
		}
	    else{
		line=line+1;
		pthread_mutex_trylock( &mutex1 );
		counter->value = counter->value + 1;
		/*count++;*/
	    	counter->value = counter->value * 2;
	    	counter->value = counter->value / 2;
		pthread_mutex_unlock( &mutex1 );
		/*printf("T1");*/
		}

	
           }
	   printf("from process1 counter  =  %d\n", counter->value); 
		printf("counter incremented by thread1 %d\n", count);
return(NULL);
}


/****************************************************************
This function increases the value of shared variable "counter"
by one 2000000 times
****************************************************************/
void * thread2(void *arg)
{
	int line = 0;	
	while (line < 2000000)
	   {
            line++;

/* Critical Section */
	pthread_mutex_trylock( &mutex1 );
	/*if((counter->value%100)==0){
        pthread_cond_signal( &condition_var );}
	else{*/
	    counter->value = counter->value + 1;
	    counter->value = counter->value * 2;
	    counter->value = counter->value / 2;
	pthread_mutex_unlock( &mutex1 );    
        	/*printf("T2\n"); 
		}*/  
	}
	   printf("from process2 counter = %d\n", counter->value); 
return(NULL);
}


/****************************************************************
*                  Main Body                                    *
****************************************************************/
int main()
{
        int             r=0;
	int 		i;
	int 		shmid;	  /* shared memory ID */
        pthread_t	tid1[1];     /* process id for thread 1 */
        pthread_t	tid2[1];     /* process id for thread 2 */
        pthread_attr_t	attr[1];     /* attribute pointer array */

         counter = (struct shared_dat *) malloc(sizeof(struct shared_dat));

	/* initialize shared memory to 0 */
	counter->value = 0 ;
           printf("1 - I am here %d in pid %d\n",r,getpid());
    
	fflush(stdout);
 /* Required to schedule thread independently.
 Otherwise use NULL in place of attr. */
        pthread_attr_init(&attr[0]);
        pthread_attr_setscope(&attr[0], PTHREAD_SCOPE_SYSTEM);  /* system-wide contention */ 

	/* end to schedule thread independently */

/* Create the threads */
        pthread_create(&tid1[0], &attr[0], thread1, NULL);
        pthread_create(&tid2[0], &attr[0], thread2, NULL);

/* Wait for the threads to finish */
    pthread_join(tid1[0], NULL);
    pthread_join(tid2[0], NULL);
	        printf("from parent counter  =  %d\n", counter->value);
		printf("---------------------------------------------------------------------------\n");
		printf("\t\t	End of simulation\n");



/* Timing of Program */
getrusage(RUSAGE_SELF, &mytiming);
printf("Time used is sec: %ld, usec %ld\n",mytiming.ru_utime.tv_sec,mytiming.ru_utime.tv_usec);
printf("System Time used is sec: %ld, usec %ld\n",mytiming.ru_stime.tv_sec,mytiming.ru_stime.tv_usec);

		exit(0);
		
}


