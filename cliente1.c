#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
 
#define PORT 3535
#define IP "127.0.0.1"
#define MAX_BUFFER 104857600

int clientfd, r;
struct sockaddr_in client;
char* buffer;

int main(int argc, char *argv[])
{
    int zize = atoi(argv[1]);
    clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientfd < 0){
        perror("\n-->Error en socket():");
        exit(-1);
    }
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
 
    inet_aton(IP, &client.sin_addr);
    
    r = connect(clientfd, (struct sockaddr *)&client, (socklen_t)sizeof(struct sockaddr));
    if(r < 0){
        perror("\n-->Error en connect(): ");
        exit(-1);
    }
    
    buffer = (char*) malloc(zize);
    int af=0;
    for (int i = 0; i < zize-af; i++)
    {
    r = recv(clientfd, buffer, MAX_BUFFER, 0);
    af+=r;
    }
    
    if(r < 0){
        perror("\n-->Error en recv(): ");
        exit(-1);
    }
    //printf("%i bits\n",af);

     
    close(clientfd);
    free(buffer);
    return 0;
}
