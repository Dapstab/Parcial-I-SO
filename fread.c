// C program for reading
// struct from a file
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main ()
{
	FILE *infile;
	infile = fopen ("data.txt", "r");
	if (infile == NULL)
	{
		fprintf(stderr, "\nError opening file\n");
		exit (1);
	}
	char bit[1];
    int af=	0;
	int is=1;
	//empieza a contar tiempo
	struct timeval tval_before, tval_after, tval_result;
    gettimeofday(&tval_before, NULL); 
	while(is=fread(bit, sizeof(bit), 1, infile)){
        af+=is;
    }
	//termina de contar tiempo
    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    printf("Seconds taken: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
	printf("%i:bytes leidos \n ",af);
	fclose (infile);
	return 0;
}
