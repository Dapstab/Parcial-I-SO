#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <sys/time.h>
 
#define PORT 3535
#define BACKLOG 2

int serverfd, clientfd, r, opt = 1;
struct sockaddr_in server, client;
char* buffer;

int main(int argc, char *argv[]){
    
    socklen_t tamano = sizeof(struct sockaddr_in);
 
    int zize = atoi(argv[1]);
    buffer = (char*) malloc(zize);    
    
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverfd < 0){
        perror("\n-->Error en socket():");
        exit(-1);
    }
    
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(server.sin_zero, 8); 
    
    //setsockopt(serverfd,SOL_SOCKET,SO_REUSEADDR,(const char *)&opt,sizeof(int));
    int yes=1;
//char yes='1'; // use this under Solaris

if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
    perror("setsockopt");
    exit(1);
}
 
    r = bind(serverfd, (struct sockaddr *)&server, tamano);
    if(r < 0){
        perror("\n-->Error en bind() serve: ");
        exit(-1);
    }
    
    r = listen(serverfd, BACKLOG);
    if(r < 0){
        perror("\n-->Error en Listen(): ");
        exit(-1);
    }
    
    clientfd = accept(serverfd, (struct sockaddr *)&client, &tamano);
    if(clientfd < 0)
    {
        perror("\n-->Error en accept: ");
        exit(-1);
    }
    struct timeval tval_before, tval_after, tval_result;
    gettimeofday(&tval_before, NULL); 
    int af=0;
    for (int i = 0; i < zize-af; i++)
    
{
    r = send(clientfd, buffer, zize, 0);
    af+=r;
}

    if(r < 0){
        perror("\n-->Error en send(): ");
        exit(-1);
    }
  //  printf("%i bits\n",af);
     
//termina de contar tiempo
    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    printf("%ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);

    free(buffer);

    close(clientfd);   
    //close(serverfd);
    return 0;
}
