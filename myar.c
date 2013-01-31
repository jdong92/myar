#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SARMAG 8
#define ARMAG "!<arch>\n"
#define HEADERSIZE 61
#define BLOCKSIZE 1024

int main ( int argc, char *argv[])
{

	int inputFd;
	ssize_t numRead;
	char buffer[BLOCKSIZE]; /*File buffer*/
	char ARMAG_BUFFER[SARMAG + 1];

	struct ar_hdr
	{
		char	ar_name[16];
		char	ar_date[12];
		char	ar_uid[6];
		char	ar_gid[6];
		char	ar_mode[8];
		char	ar_size[10];
		char	ar_fmag[2];
	};

	struct ar_hdr ar;


	//char name[16];
	//char date[12];
	//char uid[6];

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

		while((numRead = read(inputFd, buffer, HEADERSIZE) == HEADERSIZE )){
			
			sscanf(buffer, "%s %s %s", ar.ar_name, ar.ar_date, ar.ar_uid);

		}
		printf(ar.ar_name);
		//printf("Filename: %s \n", name);
		//printf("File date: %s \n", date);
		//printf("File Id: %s \n",uid);
		/* Getting file name */

		//numRead = read(inputFd, buffer, 16);
		//buffer[numRead] = '\0';
		//printf("%s \n", buffer);

		//numRead = read(inputFd, buffer, 12);
		//buffer[numRead] = '\0';
		//printf("%s \n", buffer);
	
		

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

