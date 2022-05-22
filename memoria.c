#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include <sys/time.h>
#include<sys/shm.h>

key_t key = 1234;
int r, shmId, pid;
char *shm;
char* send;


int main(int argc, char *argv[]){
    
    int paquete = atoi(argv[1]);
    //va a mandar una direccion del tamaño del paquete 
    send = (char*) calloc(paquete,sizeof(char));

    shmId = shmget(key, 2*sizeof(char), 0666|IPC_CREAT);
    if (shmId < 0)
    {
        perror("Error: shmget");
        exit(-1);
    }
    shm = (char *)shmat(shmId,0,0);//mem compartida
    if (shm < 0)
    {
        perror("Error en shmat");
        exit(-1);
    }
    
    pid = fork();//crea el hijo
    if (pid < 0)
    {
        perror("Error en fork");
        exit(-1);
    }
    
    struct timeval tval_before, tval_after, tval_result;
    gettimeofday(&tval_before, NULL);

    if (pid == 0)//child manda
    {   
        *shm = *send;
    } else {//padre recibe 
        char buffer[] = "Recibido";
        *(shm+1) = *buffer;
        gettimeofday(&tval_after, NULL);
        timersub(&tval_after, &tval_before, &tval_result);
        printf("Seconds taken: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
    
    }

    r= shmdt(shm);//desvincular
    if (r < 0)
    {
        perror("Error en shmdt");
        exit(-1);
    }
    
    free(send);
    return 0;
}