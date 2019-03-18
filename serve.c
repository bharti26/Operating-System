#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>

#define PORT 10000

int main(int arg,char *argc[])
{
        int socketn,acceptn;
        char msg[256]="Hello";

        struct sockaddr_in server;

        printf("Enter Message\n============\n");

        socketn=socket(AF_INET,SOCK_STREAM,0);

        server.sin_port=htons(PORT);
        server.sin_family=AF_INET;                      //Internet not local
        server.sin_addr.s_addr=htonl(INADDR_ANY); //Connect to any address

        if(bind(socketn,(struct sockaddr*)&server,sizeof(server))<0){
                printf("Failed to bind");
                exit -1;
        }
        if(listen(socketn,5)==-1){
                printf("Failed to listen");
                exit -1;
        }


        while(1)
        {
                printf(":>");
                scanf("%s",msg);
                acceptn=accept(socketn,(struct sockaddr*)NULL,NULL);
                write(acceptn,msg,sizeof(msg));
                close(acceptn);
                sleep(1);
        }
}
