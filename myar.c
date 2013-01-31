#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SARMAG 8
#define ARMAG "!<arch>\n"
#define BLOCKSIZE 1024

int main ( int argc, char *argv[])
{

	int inputFd;
	ssize_t numRead;
	char buffer[BLOCKSIZE + 1];
	char ARMAG_BUFFER[SARMAG + 1];

	if (argc <= 2){

		printf("Usage: %s key afile name ... \n", argv[0]);

	}

	/*Prints out the name of the file*/
	if ((argc == 3) && strcmp(argv[1], "-t") == 0){

		inputFd = open(argv[2], O_RDONLY);

		if (inputFd == -1){
			printf("Error opening file %s", argv[1]);
			exit(-1);
		}
		/* ARMAG is the buffer */
		numRead = read(inputFd, ARMAG_BUFFER, SARMAG);
		ARMAG_BUFFER[numRead] = '\0';
		if ((strcmp(ARMAG_BUFFER, ARMAG)) == -1){

			perror("Not an archive file \n");

		}

		numRead = read(inputFd, buffer, 16);
		buffer[numRead] = '\0';
		printf("%s \n", buffer);

		

		//while((numRead = read(inputFd, buffer, BLOCKSIZE)) > 0) {

			//buffer[numRead] = '\0';
			//printf("%s", buffer);

		//}

		if (numRead == -1){

			perror("Read Error");
			exit(-1);

		}

		if (close(inputFd) == -1){

			perror("Error closing file");
			exit(-1);

		}

		exit(EXIT_SUCCESS);




	}

}

