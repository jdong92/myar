#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define SARMAG 8
#define ARMAG "!<arch>\n"
#define ARFMAG "`\n"
#define HEADER_SIZE 61
#define HEADER_READ_SIZE 60
#define BLOCKSIZE 1024

int main ( int argc, char *argv[])
{

	int inputFd, archiveFd, outputFd, numWritten = 0;
	ssize_t numRead;
	char buffer[BLOCKSIZE], header[HEADER_SIZE]; /*File buffer*/
	char fileBuffer[BLOCKSIZE];
	unsigned long filesize;
	long date;
	struct tm timestruct;
	static char timestr[100];
	struct stat *fileStat;
	char name[16];
	fileStat = (struct stat *)malloc(sizeof(struct stat));

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

	struct ar_hdr *ar;
	ar = (struct ar_hdr *)malloc(sizeof(struct ar_hdr));
	
	if (argc <= 2){
		printf("Usage: %s key afile name ... \n", argv[0]);
	}

	/*Prints out the name of the file*/
	if ((argc == 3) && strcmp(argv[1], "t") == 0){

		inputFd = open(argv[2], O_RDONLY);

		if (inputFd == -1){
			printf("Error opening file %s", argv[1]);
			exit(-1);
		}

		numRead = read(inputFd, buffer, 8);
		if (strncmp(buffer, ARMAG, 8) != 0){

			perror("Unknown archive file \n");
			exit(-1);

		}
		int i = 0;
		while((numRead = read(inputFd, buffer, HEADER_READ_SIZE)) == HEADER_READ_SIZE){
			
			sscanf(buffer, "%s %s %s %s %s %s %s", ar->ar_name, ar->ar_date, ar->ar_uid, ar->ar_gid, ar->ar_mode, ar->ar_size, ar->ar_fmag);
			buffer[numRead] = '\0';
			filesize = strtoul(ar->ar_size, NULL, 10);

			int i = 0;
			while(ar->ar_name[i] != '/'){
				printf("%c", ar->ar_name[i]);
				i++;
			}
			printf("\n");
		
			lseek(inputFd, filesize, SEEK_CUR);
			
			if (filesize % 2 != 0){
				lseek(inputFd, 1, SEEK_CUR);
			}
	
		}	

		exit(EXIT_SUCCESS);

	}/* -t option */

	/* VERBOSE OPTION */
	if ((argc == 3) && strcmp(argv[1], "vt") == 0){

		inputFd = open(argv[2], O_RDONLY);

		if (inputFd == -1){
			printf("Error opening file %s", argv[1]);
			exit(-1);
		}

		numRead = read(inputFd, buffer, 8);
		if (strncmp(buffer, ARMAG, 8) != 0){

			perror("Unknown archive file \n");
			exit(-1);
		}
		int i = 0;
		while((numRead = read(inputFd, buffer, HEADER_READ_SIZE)) == HEADER_READ_SIZE){
			
			sscanf(buffer, "%s %s %s %s %s %s %s", ar->ar_name, ar->ar_date, ar->ar_uid, ar->ar_gid, ar->ar_mode, ar->ar_size, ar->ar_fmag);
			buffer[numRead] = '\0';
			filesize = strtoul(ar->ar_size, NULL, 10);
			
			printf("%s/%s   %s ", ar->ar_uid, ar->ar_gid, ar->ar_size);
			//printf("%s ", ar.ar_date);

			date = strtol(ar->ar_date, NULL, 10);
			timestruct = *localtime(&date);
			strftime(timestr, sizeof(timestr), "%b %e %H:%M %Y", &timestruct);
			printf("%s ", timestr);
		
			int i = 0;
			while(ar->ar_name[i] != '/'){
				printf("%c", ar->ar_name[i]);
				i++;
			}
			printf("\n");
			
			lseek(inputFd, filesize, SEEK_CUR);
			
			if (filesize % 2 != 0){
				lseek(inputFd, 1, SEEK_CUR);
			}
	
		}	

		exit(EXIT_SUCCESS);
	}/* End V OPTION */

	if ((argc == 4) && strcmp(argv[1], "q") == 0){
		inputFd = open(argv[3], O_RDONLY);
		archiveFd = open(argv[2], O_WRONLY);

		if (stat(argv[3], fileStat) < 0){
			printf("Error opening file \n");
			exit(-1);
		
		}
		
		lseek(archiveFd, 0, SEEK_END);

		int len = strlen(argv[3]);
		memset(name, 0, 16);
		int i = 0;
		for (i = 0; i < 16; i++){
				if (argv[3][i] == '\0'){
					name[i] = '/';
					break;
				}else{
				name[i] = argv[3][i];
				}
			}

		snprintf(header, HEADER_SIZE, "%-16s%-12ld%-6d%-6d%-8o%-10ld%-2s", name, fileStat->st_mtime, fileStat->st_uid, fileStat->st_gid, fileStat->st_mode, fileStat->st_size,ARFMAG);

			numWritten = write(archiveFd, header, HEADER_SIZE);

		if (numWritten == -1){

			perror("Error writing file header.");
			unlink(argv[2]);
			exit(-1);
		}
	
		lseek(archiveFd, -1, SEEK_CUR);

		while ((numRead = read(inputFd, fileBuffer, BLOCKSIZE)) > 0) {

			numWritten = write(archiveFd, fileBuffer, numRead);

		}
	
		if (inputFd == -1){
			exit(-1);
		}

		close(inputFd);
		close(archiveFd);

	}/* End Q OPTION */

	if ((argc == 4) && strcmp(argv[1], "x") == 0){
		
		inputFd = open(argv[2], O_RDONLY);

		if (inputFd == -1){
			perror("Error creating file.");
			exit(-1);
		}

		numRead = read(inputFd, buffer, SARMAG);
		if (strncmp(buffer, ARMAG, SARMAG) != 0){
			perror("Unknown archive file \n");
			exit(-1);
		}

		while((numRead = read(inputFd, buffer, HEADER_READ_SIZE)) == HEADER_READ_SIZE)
		{
			sscanf(buffer, "%s %s %s %s %s %s %s", ar->ar_name, ar->ar_date, ar->ar_uid, ar->ar_gid,ar->ar_mode, ar->ar_size, ar->ar_fmag);
			memset(name, 0, 16);

			int i = 0;
			while(ar->ar_name[i] != '/'){

				name[i] = ar->ar_name[i];
				i++;

			}

			filesize = strtoul(ar->ar_size, NULL, 10);

			if (strcmp(name, argv[3]) == 0){
				
				outputFd = open(argv[3], O_WRONLY | O_CREAT, strtoul(ar->ar_mode, NULL, 8));
				numRead = read(inputFd, fileBuffer, filesize);
				numWritten = write(outputFd, fileBuffer, filesize);

			}
	
			lseek(inputFd, filesize, SEEK_CUR);

			if (filesize % 2 != 0){
				lseek(inputFd, 1, SEEK_CUR);
				printf("No entry %s in archive \n",argv[3]);
			}

		}

	}/* End X OPTION */


}/* End Main */

