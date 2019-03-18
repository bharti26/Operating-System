#include <sys/types.h> 
#include <sys/socket.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include<string.h>

#define PORT_NUM 1050

char buf[15];

int main(){

	int sock;
	int i,sin_size;
	struct sockaddr_in  server,client;
	int client_sock;
	int recv_val;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock<0){
		perror("socket not created");
		exit(1);
		}

	server.sin_family = AF_INET;
	server.sin_port = htons(PORT_NUM);
      	server.sin_addr.s_addr = INADDR_ANY;
	/*if (bind(sfd, (struct sockaddr *) &my_addr,sizeof(struct sockaddr_un)) == -1)*/
	
 	if(bind(sock,(struct sockaddr*)&server,sizeof(server))){
                printf("Failed to bind");
                exit (3);
        }
	
	listen(sock,3);
	sin_size=sizeof(server); 
	for(i=0;i<3;i++){

		client_sock=accept(sock,(struct sockaddr*)0,0);
		if(client_sock==-1) {
			perror("accept error");
			//exit(2);
		}
		else memset(buf,0,sizeof(buf));
		
		if(recv_val=recv(client_sock,buf,sizeof(buf),0)<0){
		perror("recv error");
		}
		else printf("message =%s",buf);
		if (send(client_sock, buf, sizeof(buf), 0) == -1)
			{
			perror("Write to Socket Channel");
			//exit(1);
			}
		write(client_sock,buf,0);
	}

close(client_sock);
close(sock);
return 0;
}
