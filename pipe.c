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
	numIterations = Bnum / KB;
	

	for (int i = 0; i < numIterations; i++) {
		
		int bytesWr = write(fd[1], str, KB);
		if(bytesWr == -1) {
			perror("No se pudo escribir en la tuberia: \n");
			exit(-1);
		}
		numBytes += bytesWr;
	}
	free(str);
}

void readData(int fd[2], int Bnum, int mode) {		
	int numIterations;
	int numBytes = 0;
	char *str = (char *) malloc(Bnum);

	numIterations =  Bnum / KB;


	for (int i = 0; i < numIterations; i++) {
		// Read from FIFO
		int bytesRd = read(fd[0], str, KB);
		if (numBytes == -1) {
			perror("No se pudo leer de la tuberia: ");
		}
		numBytes += bytesRd;
	}
	free(str);
}	


int main(int argc, char *argv[]) {
	int fd[2];
	int ret = pipe(fd);
	int bytes= atoi(argv[1]);
	double time_spent;
        clock_t begin = clock();
	
	if (ret == -1) {
		perror("pipe");
		exit(1);
	}
		
	int id = fork();
	if (pid < 0){
		perror("Error en fork");
		exit(1);
        }
	if (id == 0) {
		close(fd[0]);
		sendData(fd, bytes, 1);
		close(fd[1]);
	} else {
		close(fd[1]);
		readData(fd, bytes, 1);
		close(fd[0]);
			
	clock_t end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	
	printf("%2.6f\n", time_spent);

	}

	

	return 0;
}
