#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>

#define KB 1024

void sendData(int fd[2], int Bnum, int mode) {
	int numIterations;
	int numBytes = 0;
	char *str = (char *) calloc(Bnum, sizeof(char));
	double time_spent;
	
	numIterations = Bnum / KB;
	printf("Numero de iteraciones %i\n", numIterations);
	clock_t begin = clock();
	for (int i = 0; i < numIterations; i++) {
		//printf("Estamos en la iteracion: %i\n", i);
		int bytesWr = write(fd[1], str, KB);
		if(bytesWr == -1) {
			perror("No se pudo escribir en la tuberia: \n");
			exit(-1);
		}
		numBytes += bytesWr;
	}
	clock_t end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("Bytes escritos %d\n", numBytes);
	printf("El tiempo de escritura es de: %f\n\n", time_spent);
	close(fd[1]);
	free(str);
}

void readData(int fd[2], int Bnum, int mode) {		
	int numIterations;
	int numBytes = 0;
	char *str = (char *) malloc(Bnum);
	double time_spent;
	

	numIterations =  Bnum / KB;
	//printf("Numero de iteraciones: %i\n", numIterations);
	clock_t begin = clock();
	for (int i = 0; i < numIterations; i++) {
		// Read from FIFO
		int bytesRd = read(fd[0], str, KB);
		if (numBytes == -1) {
			perror("No se pudo leer de la tuberia: ");
		}
		numBytes += bytesRd;
	}
	clock_t end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	
		
	printf("Bytes leidos %d\n", numBytes);
	printf("El tiempo de lectura es de: %f\n", time_spent);
	close(fd[0]);
	free(str);
}	



int main() {
	int fd[2];
	int ret = pipe(fd);
	int bytes;
	fputs("Ingrese el número de bytes\n", stdout);
	scanf("%d", &bytes);
	if (ret == -1) {
		perror("pipe");
		exit(1);
	}
		
	int id = fork();
	if (id == 0) {
		close(fd[0]);
		sendData(fd, bytes, 1);
	} else {
		close(fd[1]);
		readData(fd, bytes, 1);
	}
	return 0;
}
