#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<arpa/inet.h>

#define PORT 10000

struct sockaddr_in client;
char msg[256];
struct sockaddr_in client;
int socketn,n;

void receive();

int main()
{
        client.sin_family=AF_INET;
        client.sin_port=htons(PORT);
        client.sin_addr.s_addr=inet_addr("127.0.0.1");

        while(1)
        {
                receive();
                sleep(1);
        }
}

void receive()
{
        if((socketn=socket(AF_INET,SOCK_STREAM,0))<0){
                printf("Cannot create socket.");
                exit -1;
        }

        bind(socketn,(struct sockaddr*)&client,sizeof(client));

        if((connect(socketn,(struct sockaddr*)&client,sizeof(client)))<0){
                printf("Cannot create connection");
                exit -1;
        }

        read(socketn,msg,sizeof(msg));
        printf(":>%s\n",msg);
        sleep(1);

}
