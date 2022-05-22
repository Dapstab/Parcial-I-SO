#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>

#define KB 1024
#define 10KB 10240
#define 100KB 102400
#define MB 1048576
#define 10MB 10485760
#define 100MB 104857600
#define MAX_SEND 65536

void sendData(char *tuberia, int Bnum, int mode) {	
	int fd = open(tuberia, O_WRONLY);
	int numIterations;
	int numBytes = 0;
	char *str = (char *) calloc(Bnum, sizeof(char));
	double time_spent;
	if (mode == 0) {
		printf("Entro a KB\n");
		numIterations = Bnum / KB;
		printf("Numero de iteraciones %i\n", numIterations);
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
	} else {
		printf("Entro a MB\n");
		numIterations = Bnum / MAX_SEND;
		printf("Numero de iteraciones %i\n", numIterations);
		clock_t begin = clock();
		for (int i = 0; i < numIterations; i++) {
			//printf("Estamos en la iteracion: %i\n", i);
			int bytesWr = write(fd, str, MAX_SEND);
			numBytes += bytesWr;
			if(bytesWr == -1) {
				perror("No se pudo escribir en la tuberia: \n");
				exit(-1);
			}
		}
		clock_t end = clock();
		time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	}
	printf("Bytes escritos %d\n", numBytes);
	printf("El tiempo de escritura es de: %f\n\n", time_spent);
	close(fd);
	free(str);
}


int main () {
	int opc = 0;
	char *tuberia = "./tuberia";
	    	
	if (mkfifo(tuberia, 0777) == -1) {
		if (errno != EEXIST) { // Queremos que nos de los errores solamente de cuando el arhivo no este creado
			perror("Could not create fifo file: \n");			
		}
	}
		
	srand(time(NULL));
	
	do
	{
		printf("Bienvenido\n\n");
		printf("1. Enviar 1KB\n");
		printf("2. Enviar 10KB\n");
		printf("3. Enviar 100KB\n");
		printf("4. Enviar 1MB\n");
		printf("5. Enviar 10MB\n");
		printf("6. Enviar 100MB\n");
		printf("7. Salir\n");
		scanf("%d", &opc);
		switch (opc)
		{
		case 1:
			sendData(tuberia, KB, 0);
			break;

		case 2:
			sendData(tuberia, 10KB, 0);
			break;
			
		case 3:
			sendData(tuberia, 100KB, 0);
			break;
			
		case 4:	
			sendData(tuberia, MB, 1);
			break;
			
		case 5:
			sendData(tuberia, 10MB, 1);
			break;
		
		case 6:
			sendData(tuberia, 100MB, 1);
			break;
		   
		case 7:
			printf("Adios\n");
			// ELimina el archivo FIFO
			unlink(tuberia);
			break;
		    
		default:
		    printf("Opcion incorrecta\n");
		    break;
		}
	} while (opc != 7);

	
	return 0;
}
