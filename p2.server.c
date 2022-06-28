#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>

#include <stdlib.h> 
#include <arpa/inet.h>

#define PORT 3535
#define BACKLOG 8

struct Datos
{
    int idOrigen;
    int idDestino;
    int hora;
};

struct hash
{
    int origen;
    int ref;
};

struct registro
{
    int origen;
    int destino;
    int hora;
    int tiempo;
    int ref;
};

int Buscar(struct Datos *datos){
	// Read File
	//◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄◄
	// Read file name
	FILE *review = fopen("archivo.txt", "rb");
	FILE *ph = fopen("archivo_hash.txt", "rb");
	// Create receive object   
    
	if (review == NULL)
	{
		printf("\nError to open the file\n");
		exit(1);
	}
	
	int origen = datos->idOrigen;
	int destino = datos->idDestino;
	int hora = datos->hora;

	int aux1;
	struct registro busc;

	//consulta referencia del hash
	fseek(ph, 8 * origen , SEEK_SET);
	fseek(ph, 4, SEEK_CUR);
	fread(&aux1, sizeof(int), 1, ph);

	//entra a la primera posicion proporcionada por el hash
	fseek(review, 20 * aux1, SEEK_SET);
	fread(&busc, sizeof(struct registro), 1, review);

	while (busc.destino !=  destino || busc.hora != hora) {
		if (busc.ref==-1) {
		 //   printf("No se encontro\n");
		    break;
		}
		fseek(review, 20 * busc.ref, SEEK_SET);
		fread(&busc, sizeof(struct registro), 1, review);
	}
	if (busc.ref!=-1)
//	printf("\nTiempo: %i\n", busc.tiempo); bndra de tiempo

	fclose(review);
	fclose(ph);
	if (busc.destino !=  destino || busc.hora != hora){
		return -1;
	}else{
		return busc.tiempo;
	}
}

void recibir(socklen_t size, struct sockaddr_in  client,pid_t childpid,int clientfd,int serverfd, char sendToClient[100]) {
	 FILE* fichero;
    fichero = fopen("log.txt", "wt");
   
	printf("Hola desde recibir:\n ");
	struct Datos datos;
	//char busq[9];
	int tiempo;
	while (1) {
		clientfd = accept(serverfd, (struct sockaddr *) &client, &size);
		if (clientfd < 0) {
			//perror("Error en el accept");
			exit(-1);
		}

			time_t timrpo;
    		time(&timrpo);
   			struct tm *timpo=localtime(&timrpo);
//bandera de impresion	printf("[Fecha %d%02d%dT%d%d%d]",timpo->tm_year+1900,timpo->tm_mon,timpo->tm_mday,timpo->tm_hour,timpo->tm_min,timpo->tm_sec);
//bndra'conecion 	printf("Conexion aceptada de %s:%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
    
		if ((childpid = fork()) == 0) {       
			close(serverfd);
			while (1) {
				recv(clientfd,&datos,sizeof(datos),0);
				if (datos.idOrigen == 99999) {
//bandera de deconexion	printf("Desconectando de %s:%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
					send(clientfd, &tiempo, sizeof(int), 0);
  					  fclose(fichero);

                			break;
				} else {
					//se imprime en el archivo
					char busq[9];
					tiempo = Buscar(&datos);
					if(tiempo!=-1){
						sprintf(busq, "%d", tiempo); 
					}else{
						strcpy(busq,"Nan");
					}
				    fprintf(fichero,"[Fecha %d %02d %02d T %02d %02d %02d]Cliente[%s][%s-%d-%d]\n",timpo->tm_year+1900,timpo->tm_mon,timpo->tm_mday,timpo->tm_hour,timpo->tm_min,timpo->tm_sec,inet_ntoa(client.sin_addr),busq,datos.idOrigen, datos.idDestino );
					//sleep(3);
					sendToClient = "Los datos se recibieron con éxito";
					//send(clientfd, sendToClient, strlen(sendToClient),0);
					send(clientfd, &tiempo, sizeof(int), 0);
					//bzero(sendToClient, sizeof(sendToClient));
				}
			}
	    	}
	}
	
}


int main() {  
	int serverfd, clientfd, r;
	int opt = 1;
	char sendToClient[100];
	struct sockaddr_in server, client;
	socklen_t size = sizeof(client);
	pid_t childpid;
	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (serverfd < 0) {
		perror("Error en el socket");
		exit(-1);
	}
	if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR| SO_REUSEPORT, &opt, sizeof(opt))) {
	    perror("setsockopt");
	    exit(EXIT_FAILURE);
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
	
    	//primero lee luego escribe
     	recibir(size, client,  childpid,  clientfd,  serverfd, sendToClient);
   
	//sendData(clientfd, numBytes);
	
	close(clientfd);
	close(serverfd);
	
	return 0;
}

    
