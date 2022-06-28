#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>          /* See NOTES */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>

#define PORT 3535
//#define IP "127.0.0.1"

struct Datos
{
    int idOrigen;
    int idDestino;
    int hora;
};

int idLugar(int *a)
{
    while (*a < 1 || *a > 1160)
    {
        printf("EL id ingreado no es valido, debe ser un valor entre 1 y 1160. Ingrese nuevamente el valor\n");
        scanf("%d", a);
    }
}

int formatoHora(int *a)
{
    while (*a < 0 || *a > 23)
    {
        printf("La hora ingreada no es valida, debe ser un valor entre 0 y 23. Ingrese nuevamente el valor\n");
        scanf("%d", a);
    }
}

void enviar(struct Datos *datos, int clientfd, char serverOutput[100], char* ip){
	if(datos->idOrigen!=99999)
	printf("El mensaje enviado fue %d %d %d\n", datos->idOrigen, datos->idDestino, datos->hora);
	int tiempo;
	while (1) {
		if(send(clientfd, datos, sizeof(struct Datos),0) < 0) {
			perror("Error en send");
			exit(-1);
		}	
		//if (recv(clientfd, serverOutput, 100, 0) == 0) {
		if (recv(clientfd, &tiempo, sizeof(int), 0) == 0) {
			printf("[-]Error en recibir dato");
			break;
		} else {
			//printf("Server:\%s\n",serverOutput);
			
			if(tiempo==-1&&datos->idOrigen!=99999){

				printf("Server: El tiempo de viaje es: NaN \n");
			}else if(datos->idOrigen!=99999){
				printf("Server: El tiempo de viaje es: %d\n", tiempo);
			}
			// Aqui se puede colocar la función bloqueante
			break;
		}
	}
	//close(clientfd);
}



	
int main(int argc, char const *argv[])
{
	//char IP=argv[1];
	char *IP="127.0.0.1";
	int opc = 0;
	int clientfd, r, tiempo;
	struct sockaddr_in client;

	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	if (clientfd < 0) {
		perror("Error en el socket");
		exit(-1);
	}

	// Configurando el socket
	client.sin_family = AF_INET;
	client.sin_port = htons(PORT);
	inet_aton(IP, &client.sin_addr);

	r = connect(clientfd, (struct sockaddr *) &client, (socklen_t) sizeof(struct sockaddr));
	if (r < 0) {
		perror("Error en la conexión");
		exit(-1);
	}

    
	// Crea una estructura que guardara los datos
	struct Datos datos; // No puedo hacer *datos por un segmentation fault ?¿
	do 
	{
		printf("Bienvenido\n\n");
		printf("1. Ingresar origen\n");
		printf("2. Ingresar destino\n");
		printf("3. Ingresar hora\n");
		printf("4. Buscar tiempo de viaje medio\n");
		printf("5. Salir\n");
		scanf("%d", &opc);
		int origen;
		int destino;
		int hora;
		char serverOutput[100];
		float viajeMedio = 0;
		switch (opc)
        {
        case 1:
		printf("Ingrese el ID del origen ");
		scanf("%d", &origen);
		idLugar(&origen);
		printf("El id ingresado fue %d\n", origen);
		datos.idOrigen = origen;
		break;
        case 2:
		printf("Ingrese el ID del destino ");
		scanf("%d", &destino);
		idLugar(&destino);
		printf("El id ingresado fue %d\n", destino);
		datos.idDestino = destino;
		break;

        case 3:
		printf("Ingrese hora del dia ");
		scanf("%d", &hora);
		formatoHora(&hora);
		printf("\nLa hora ingrasada fue %d\n", hora);
		datos.hora = hora;
		break;

        case 4:            
        	printf("\%d\n", clientfd);
            	enviar(&datos, clientfd, serverOutput, IP);
            	// Agregar función bloqueante para que no salga el menu de inmediato
				printf("Press enter to continue\n");
				char enter=0;
				while(enter!='\r'&& enter!='\n'&& r!=EOF){
					enter=getchar();
					getchar();
				}
            	break;
        case 5:
            printf("Adios\n");
			datos.idOrigen=99999;
			datos.idDestino=3;
			datos.hora=2;
			enviar(&datos, clientfd, serverOutput, IP);
            close(clientfd); // Cerramos la conexión
            break;

        default:
            printf("Opcion incorrecta\n");
            break;
        }
    

    } while (opc != 5);
}




