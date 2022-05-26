#include <stdio.h>
#include <stdlib.h> // calloc, exit, free, srand
#include <fcntl.h> // open
#include <unistd.h> // write, close
#include <errno.h> // errno (verificar existencia con EEXIST).
#include <sys/stat.h> // mkfifo, 
#include <time.h> // clock_t, CLOCKS_PER_SEC

#define KB 1024

void sendData(char *tuberia, int Bnum) {	
	int fd = open(tuberia, O_WRONLY);
	if(fdw == -1){
        	printf("open error");
        	exit(0);
   	}
	int numIterations;
	int numBytes = 0;
	char *str = (char *) calloc(Bnum, sizeof(char));
	double time_spent;

	numIterations = Bnum / KB;
	//printf("Numero de iteraciones %i\n", numIterations);
	clock_t begin = clock();
	for (int i = 0; i < numIterations; i++) {
		//printf("Estamos en la iteracion: %i\n", i);
		int bytesWr = write(fd, str, KB);
		if(bytesWr == -1) {
			perror("No se pudo escribir en la tuberia: \n");
			exit(-1);
		}
		numBytes += bytesWr;
	}
	clock_t end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	
	//printf("Bytes escritos %d\n", numBytes);
	printf("%2.6f \n", time_spent);
	close(fd);
	free(str);
}


int main (int argc, char *argv[]) {
	char *tuberia = "./tuberia";
	int size = atoi(argv[1]);
	    	
	if (mkfifo(tuberia, 0777) == -1) {
		if (errno != EEXIST) { // Queremos que nos de los errores solamente de cuando el arhivo no este creado
			perror("Could not create fifo file: \n");			
		}
	}
	
	srand(time(NULL));
	sendData(tuberia, size);
	
	return 0;
}
