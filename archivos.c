#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#define KB 1024

int main (int argc, char *argv[]) 
{
	int size = atoi(argv[1]);
	FILE *outfile;
	char * ptr;
	char * ptrs;
	outfile = fopen ("data.txt", "wb");
	if (outfile == NULL)
	{
		fprintf(stderr, "\nError opened file\n");
		exit (1);
	}
   
	struct timeval tval_before, tval_after, tval_result;
    	gettimeofday(&tval_before, NULL); 

	ptr = (char*)calloc(KB , sizeof(char));
    	int tw;
	int af = 0;
	while (af!=size) {
		tw = fwrite (ptr, 1, KB, outfile);
		af += tw;

	}

	//printf("\n%i tamanio write\n",af);      
	if(fwrite == 0){
		printf("error writing file !\n");
    	}
	fclose (outfile);
    
    	//Empieza a leer el archivo
    	FILE *infile;
	infile = fopen ("data.txt", "rb");
	if (infile == NULL) {
		fprintf(stderr, "\nError opening file\n");
		exit (1);
	}
	char bit[1];
	int  is;
	ptrs = (char*)malloc(size* sizeof(char));
	
	// Leemos de a bit size veces.
	is=fread(ptrs, sizeof(bit), size, infile);
	
	
	gettimeofday(&tval_after, NULL);
	timersub(&tval_after, &tval_before, &tval_result);
	printf(" %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
	//printf("%i:bytes leidos \n ",is);
	fclose (infile);

	return 0;
}
