#include <stdlib.h>
#include <unistd.h> 
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>

#define PORT 3535
#define IP "127.0.0.1"
#define KB 1024

void readData(int fd, int Bnum) {
	int r;
	int numIterations, bytesRd;
	int numBytes = 0;
	char *str = (char *) malloc(Bnum);
	double time_spent;
	
	numIterations =  Bnum / KB;
	printf("Numero de iteraciones: %i\n", numIterations);
	clock_t begin = clock();
	for (int i = 0; i < numIterations; i++) {
		bytesRd = recv(fd, str, KB, 0);
		if (bytesRd == -1) {
			perror("Error en el recv: ");
		}
		numBytes += bytesRd;
	}
	clock_t end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	
	
	printf("Bytes leidos %d\n", numBytes);
	printf("El tiempo de lectura es de: %f\n", time_spent);
	
	free(str);	
}

int main(int argc, char *argv[]) {
	int clientfd, r;
	int numBytes = atoi(argv[1]);
	struct sockaddr_in client;
	
	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	if (clientfd < 0) {
		perror("Error en el socket");
		exit(-1);
	}
	
	client.sin_family = AF_INET;
	client.sin_port = htons(PORT);
	inet_aton(IP, &client.sin_addr);
	
	r = connect(clientfd, (struct sockaddr *) &client, (socklen_t) sizeof(struct sockaddr));
	if (r < 0) {
		perror("Error en la conexión");
		exit(-1);
	}
	
	readData(clientfd, numBytes);	
	close(clientfd);
	
	return 0;
}
