#include <stdlib.h> 
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

#define KB 1024
#define PORT 3535
#define BACKLOG 8

void sendData(int fd, int Bnum) {
      	int numIterations, bytesSend;
	int numBytes = 0;
	char *str = (char *) calloc(Bnum, sizeof(char));
	double time_spent;
	
	numIterations = Bnum / KB;

	
	
	clock_t begin = clock();
	for (int i = 0; i < numIterations; i++) {
		
		bytesSend = send(fd, str, KB, 0);
		if (bytesSend < 0 ){
			perror("Error en send");
			exit(-1);
		}
		numBytes += bytesSend;
	}
	clock_t end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	//printf("Bytes escritos %d\n", numBytes);
	printf("%f\n\n", time_spent);
	
	free(str);
}

int main(int argc, char *argv[]) {
	int serverfd, clientfd, r;
	int opt = 1;
	int numBytes = atoi(argv[1]);
	struct sockaddr_in server, client;
	socklen_t size = sizeof(client);
	
	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverfd < 0) {
		perror("Error en el socket");
		exit(-1);
	}

	if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR| SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
	    perror("setsockopt");
	    exit(1);
	}
	
    	// Configurando el socket
    	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;
	
	bzero(server.sin_zero, 8);
	
	r = bind(serverfd,(struct sockaddr *)&server,sizeof(struct sockaddr));
	
	if (r < 0) {
		perror("Error en bind");
		exit(-1);
	}
	
	r = listen(serverfd, BACKLOG);
	if (r < 0) {
		perror("Error en listen");
		exit(-1);
	}
	size = 0;
	clientfd = accept(serverfd, (struct sockaddr *) &client, &size);
	if (clientfd < 0) {
		perror("Error en el accept");
		exit(-1);
	}
	sendData(clientfd, numBytes);
	
	close(clientfd);
	//close(serverfd);
}
