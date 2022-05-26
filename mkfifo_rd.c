#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>

#define KB 1024

int main (int argc, char *argv[]) {
	char *tuberia = "./tuberia";
	int numIterations;
	int numBytes = 0;
	// Abrimos la tuberia. (Conectamos)
	int fd1 = open(tuberia, O_RDONLY);
	if(fd1 == -1) {
        	printf("open error");
        	exit(0);
    }
	int size = atoi(argv[1]);
	double time_spent;
	// Buffer
	char *str = (char *) malloc(size);
	
	if (mkfifo(tuberia, 0777) == -1) {
		if (errno != EEXIST) { // Queremos que nos de los errores solamente de cuando el arhivo no este creado
			perror("Could not create fifo file: \n");			
		}
	}
	numIterations =  size / KB;

	//printf("Numero de iteraciones: %i\n", numIterations);
	clock_t begin = clock();
	for (int i = 0; i < numIterations; i++) {
		// Read from FIFO
		int bytesRd = read(fd1, str, KB);
		if (numBytes == -1) {
			perror("No se pudo leer de la tuberia: ");
		}
		numBytes += bytesRd;
	}
	clock_t end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	
	
	//printf("Bytes leidos %d\n", numBytes);
	printf(" %2.6f\n", time_spent);
	close(fd1);
	free(str);
	return 0;
}

