#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BLOCKSIZE 1024

int main ( int argc, char *argv[])
{
	int inputFd;
	ssize_t numRead;
	char buffer[BLOCKSIZE + 1];
	
	if (argc <= 2) {
		printf("Usage: %s key afile name ... \n", argv[0]);	
		
	}
	if ((argc == 3) && strcmp(argv[1], "-v") == 0){
		printf("-v option \n");
		
	}
	else if ((argc == 3) && strcmp(argv[1], "-t") == 0){
		printf("Hello World! \n");	
	}

}


