/* Name Bharti Goel
#U20683088
*/
/* I have used solution from course book, which is also a classical solution but to give fair chance to writer i have introduced the flag want_to_write to be set 1 when writer is ready*/
#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <semaphore.h>
struct rusage mytiming;
struct timeval mytimeval;
sem_t wrt;
sem_t mutex;
sem_t in;

/*Tested on linux 1/23/11 */
/* compile with gcc -ofname thread-template.c -lpthread */


struct shared_dat
   {
   int value;     /* shared variable to store result*/
   };


struct shared_dat  *counter;
int readcount=0;
int in_cs=0;
int want_to_write=0;
int relaxandspendtime()
{
int i;
int x;
for(i = 0; i < 250000000; i++) x=counter->value;
return x;
}

/****************************************************************
* This function Reads the value of shared variable "counter"
  by one  250000000 times
****************************************************************/
void * reader_thread(void *arg)
{	int readerid= (intptr_t)arg;
  int val;
  /*printf("reading");*/
  while(want_to_write==1);
  
  sem_wait(&mutex);
  readcount++;
  if(readcount==1) 
    sem_wait(&wrt);
  sem_post(&mutex);
  if(in_cs) printf("error \n");
    val=relaxandspendtime();
    printf("from reader %d counter  =  %d\n", readerid,val);
    /*printf("from reader %d counter  =  %d\n", readerid,counter->value); */
  sem_wait(&mutex);
  readcount--; 
  if(readcount==0)
    sem_post(&wrt);
  sem_post(&mutex);
  
	   
		/*printf("Number of times thread1 increaments counter by 100 = %d \n", count1);*/
return(NULL);
}


/****************************************************************
This Writer function increases the value of shared variable "counter"
by one 25000 times
****************************************************************/
void * writer_thread(void *arg)
{
	int line = 0;	
 int count=0;
 want_to_write=1; /* set to 1 in order to show writer is ready and no reader coming after this writer will be allowed to enter CS.*/
/* Critical Section */
      sem_wait(&wrt);
      in_cs= 1;
	    while(line<25000){
      counter->value = counter->value + 1;
      line++;
       }in_cs=0; 
     	
  
  	   printf("Done! from writer counter = %d\n", counter->value); 
         sem_post(&wrt);
         want_to_write=0;
return(NULL);
}


/****************************************************************
*                  Main Body                                    *
****************************************************************/
int main(int argc, char *argv[])
{
        int     r=0,k;
	      int 		i;
	      int 		shmid;	  /* shared memory ID */
        int numOfReaders =atoi(argv[1]);;
        /*scanf("%d",&numOfReaders);*/
        if(numOfReaders>12 || numOfReaders<1){
        printf("number of readers cannot be more than 12 and less than 1");
        return 0;}
        
        pthread_t	readers[numOfReaders];     /* process id for readers  */
        pthread_t	writer[1];     /* process id for writers */
        pthread_attr_t	attr[1];     /* attribute pointer array */

         counter = (struct shared_dat *) malloc(sizeof(struct shared_dat));
        sem_init(&wrt, 0, 1);
        sem_init(&mutex, 0, 1);
        
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
      k = (int) (numOfReaders/2);
      
      for(i = 0; i < k; i++){
        pthread_create(&readers[i], &attr[0], reader_thread, (void*)(intptr_t) i);
      }
/* Create the writer thread */
pthread_create(&writer[0], &attr[0], writer_thread, NULL);
/* int temp= relaxandspendtime();*/
      
      for(i = k ; i < numOfReaders ; i++) {
        pthread_create(&readers[i], &attr[0], reader_thread, (void*)(intptr_t) i);
      }
        
/* Wait for the threads to finish */
    /*for(i = 0; i < k; i++){
      pthread_join(readers[i], NULL);
      }*/
      
    
    
    for(i = 0 ; i < numOfReaders ; i++) {
        pthread_join(readers[i],NULL);
      }
    pthread_join(writer[0], NULL);
	        /*printf("from parent counter  =  %d\n", counter->value);
		printf("---------------------------------------------------------------------------\n");*/
		printf("\t\t	End of simulation\n");



/* Timing of Program */
getrusage(RUSAGE_SELF, &mytiming);
printf("Time used is sec: %ld, usec %ld\n",mytiming.ru_utime.tv_sec,mytiming.ru_utime.tv_usec);
printf("System Time used is sec: %ld, usec %ld\n",mytiming.ru_stime.tv_sec,mytiming.ru_stime.tv_usec);

		exit(0);
		
}
