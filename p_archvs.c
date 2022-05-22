#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#define KB 1024
#define _1kb 1024
#define _10kb 10240
#define _100kb 102400
#define _1Mb 1048576
#define _10Mb 10485760
#define _100Mb 104857600

int main (int argc, char *argv[]) 
{
	int size = atoi(argv[1]);
	FILE *outfile;
	char * ptr;
	outfile = fopen ("data.txt", "w");
	if (outfile == NULL)
	{
		fprintf(stderr, "\nError opened file\n");
		exit (1);
	}
   
	struct timeval tval_before, tval_after, tval_result;
    gettimeofday(&tval_before, NULL); 

	ptr = (char*)calloc(KB , sizeof(char));
    int tw;
	int af=	0;
	while (af!=size)
		{
			tw = fwrite (ptr, 1, KB, outfile);
			af+=tw;
	 
		}

	printf("\n%i tamanio write\n",af);      
	if(fwrite == 0){

		printf("error writing file !\n");
    }
	fclose (outfile);
    //Empiesa a leer el archivo
    FILE *infile;
	infile = fopen ("data.txt", "r");
	if (infile == NULL)
	{
		fprintf(stderr, "\nError opening file\n");
		exit (1);
	}
	char bit[1];
    int afs, is;
	
	while(is=fread(bit, sizeof(bit), 1, infile)){
        afs+=is;
    }
	//termina de contar tiempo
    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    printf("Seconds taken: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
	printf("%i:bytes leidos \n ",afs);
	fclose (infile);
	return 0;
}
