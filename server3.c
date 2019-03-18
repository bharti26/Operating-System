#include <sys/types.h> 
#include <sys/socket.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netinet/in.h>
/*Name Bharti Goel #U26803088*/
/* I have used code given in socket slides and used some part from wrriten.c.
https://www.youtube.com/watch?v=pFLQkmnmD0o this is the youtube video used by me as refernce.
Plus i have used some part of threads from my reader-writer assignment code.
*/
#include <arpa/inet.h>
#include <stdio.h>
#define _REENTRANT
#include <pthread.h>

#include<semaphore.h>
#include<sys/resource.h>
#include<unistd.h>
#include<string.h>


sem_t sem;
#define PORT_NUM 1050
int n=3; /*number of threads*/
char buf[15];

void *rthread(void *arg){
	int client_sock= (intptr_t) arg;
	int f,recv_val;
	sem_wait(&sem);
	memset(buf,0,sizeof(buf));
	/*printf("inthread");*/
	if(recv_val=recv(client_sock,buf,sizeof(buf),0)<0){
		perror("recv error");
		}
	
	else {
		/*buf[recv_val]='\0';*/
		printf("message received =%s",buf);
	}

	sleep(2);

	if (send(client_sock, buf, sizeof(buf), 0) == -1)
		{
		perror("Write to Socket Channel");
		
		}
	write(client_sock,buf,recv_val);

	sem_post(&sem);
	close(client_sock);
}


int main(){

	int sock;
	int i,sin_size,j,r=0;
	struct sockaddr_in  server,client;
	int client_sock;
	int recv_val;
	
	pthread_t	tid[3];     /* process id for thread */
        pthread_attr_t	attr[1];     /* attribute pointer array */
	
	sem_init(&sem,0,1);
	printf("1 - I am here %d in pid %d\n",r,getpid());
    
	fflush(stdout);
	pthread_attr_init(&attr[0]);
        pthread_attr_setscope(&attr[0], PTHREAD_SCOPE_SYSTEM); 

	printf("creating socket.......\n");
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock<0){
		perror("socket not created");
		exit(1);
		}

	server.sin_family = AF_INET;
	server.sin_port = htons(PORT_NUM);
      	server.sin_addr.s_addr = INADDR_ANY;
	/*if (bind(sfd, (struct sockaddr *) &my_addr,sizeof(struct sockaddr_un)) == -1)*/

	printf("binding socket.......\n");
 	if(bind(sock,(struct sockaddr*)&server,sizeof(server))){
                printf("Failed to bind");
                exit (3);
        }

	printf("listening socket.......\n");
	listen(sock,n);
	sin_size=sizeof(struct sockaddr_in); 
	printf("accepting socket.......\n");

	for(i=0;i<n;i++){
		
		client_sock=accept(sock,(struct sockaddr*)&client,&sin_size);
		if(client_sock==-1) {
			perror("accept error");
			//exit(2);
		}/*sleep(2);*/
		/*else memset(buf,0,sizeof(buf));*/

		/*printf("threadcreating...");*/
		pthread_create(&tid[i],&attr[0],rthread, (void*)(intptr_t)client_sock);
		/*printf("threadcreated");*/
		
	}
	for(j=0;j<n;j++){
		pthread_join(tid[j],NULL);
	}
if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)/*http://stackoverflow.com/questions/24194961/how-do-i-use-setsockoptso-reuseaddr*/
    perror("setsockopt(SO_REUSEADDR) failed");
close(client_sock);
close(sock);
return 0;
}
