#include <sys/types.h> 
#include <sys/socket.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include<semaphore.h>
#include<sys/resource.h>
#include<unistd.h>
#include<string.h>


sem_t sem;

#define PORT_NUM 1050
char buf[15];


void *rthread(void *arg)
{

int new_fd= (intptr_t) arg;
int nbytes,len,f;
sem_wait(&sem);
memset(buf,0,15);
if((len=recv(new_fd,buf,15,0))<0){
perror("msg recvd");}
f=read(new_fd,buf,15);

//sleep(2);
printf("thread");
buf[len]='\0';
printf("%s",buf);
f=write(new_fd, buf, 15);

printf("%s",buf);
sem_post(&sem);
close(new_fd);

}


int main(int arg,char *argc[])
{
	int len,j,i;  
	int sin_size;      
	int sock,client;
	pthread_t	tid[3];     /* process id for thread 1 */
        pthread_attr_t	attr[1];     /* attribute pointer array */
        
        struct sockaddr_in server;
	struct sockaddr_in cli;

	sem_init(&sem,0,1);
	pthread_attr_init(&attr[0]);
        pthread_attr_setscope(&attr[0], PTHREAD_SCOPE_SYSTEM); 

	memset((char*)&server,0,sizeof(server));
        printf("Server Started..\n============\n");

        //if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	if ( (sock= socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Opening Stream Socket");
		exit(1);
	}
//printf("initing...");
        server.sin_port=htons(PORT_NUM);
        server.sin_family=AF_INET;                      /*Internet not local*/
        server.sin_addr.s_addr=htonl(INADDR_ANY); /*Connect to any address*/
//printf("bindting...");

        if(bind(sock,(struct sockaddr*)&server,sizeof(server))<0){
                printf("Failed to bind");
                exit -1;
        }
//printf("listenting..."); 

        /*if(listen(sock,5)==-1){
                printf("Failed to listen");
                exit -1;
        }*/

	listen(sock,5);
//sem_init(&sem,0,1);

       for(i=0;i<3;i++)
        {
                
		sin_size=sizeof(cli);   
//printf("accepting..."); 
		
                if((client=accept(sock,(struct sockaddr*)&cli,&sin_size))<0)
			{ perror("accept error");
			}

printf("threadcreated...");
		pthread_create(&tid[i],&attr[0],rthread, (void*)client);
printf("threadcreated");
}
	
	for(j=0;j<3;j++){
		pthread_join(tid[j],NULL);
		}

                //write(client,buf,sizeof(buf));
                
                close(client);
                sleep(1);
        
close(sock);
return 0;
}
