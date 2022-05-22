#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>

#define KB 1024

int main (int argc, char *argv[]) {
	char *tuberia = "./tuberia";
	int numIterations, bytesRd;
	int numBytes = 0;
	int fd1 = open(tuberia, O_RDONLY);
	int size = atoi(argv[1]);
	char *str = (char *) malloc(size);
	double time_spent;
	
	if (mkfifo(tuberia, 0777) == -1) {
		if (errno != EEXIST) { // Queremos que nos de los errores solamente de cuando el arhivo no este creado
			perror("Could not create fifo file: \n");			
		}
	}
	numIterations =  size / KB;

	printf("Numero de iteraciones: %i\n", numIterations);
	clock_t begin = clock();
	for (int i = 0; i < numIterations; i++) {
		// Read from FIFO
		bytesRd = read(fd1, str, KB);
		if (numBytes == -1) {
			perror("No se pudo leer de la tuberia: ");
		}
		numBytes += bytesRd;
	}
	clock_t end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	
	
	printf("Bytes leidos %d\n", numBytes);
	printf("El tiempo de lectura es de: %f\n", time_spent);
	close(fd1);
	free(str);
	return 0;
}

