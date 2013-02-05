#define _BSD_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <dirent.h>
#define STR_SIZE sizeof("rwxrwxrwx")
#define FP_SPECIAL 1

#define SARMAG 8
#define ARMAG "!<arch>\n"
#define ARFMAG "`\n"
#define HEADER_SIZE 61
#define HEADER_READ_SIZE 60
#define BLOCKSIZE 1024

char *file_perm_string(mode_t perm, int flags);

int main ( int argc, char *argv[])
{
	int inputFd, archiveFd, outputFd, numWritten = 0;
	ssize_t numRead;
	char buffer[BLOCKSIZE], header[HEADER_SIZE]; 
	char fileBuffer[BLOCKSIZE];
	unsigned long filesize;
	long date;
	struct tm timestruct;
	static char timestr[100];
	struct stat *fileStat;
	char name[16];
	char *permission;
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
	if ((argc == 3) && (strcmp(argv[1], "t") == 0){

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

			permission = file_perm_string(strtoul(ar->ar_mode, NULL, 10), 0); 
			printf("%s ", permission);
			
			printf("%s/%s     %s ", ar->ar_uid, ar->ar_gid, ar->ar_size);
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

	if ((argc >= 3) && strcmp(argv[1], "q") == 0){
		
		if (argc == 3){

			outputFd = open(argv[2], O_WRONLY); /* The actual archive file */

			if (errno == ENOENT){
				outputFd = open(argv[2], O_WRONLY | O_CREAT, 0666);
				if (outputFd == -1){
					perror("Error creating archive file");
					exit(-1);
				}
				numWritten = write(outputFd, ARMAG, SARMAG);
				printf("myar: creating %s \n", argv[2]);
			
			}
		}else{	
			int i = 0;

			for (i = 3; i < argc; i++){
		
				inputFd = open(argv[i], O_RDONLY); /* Regular file you want to append */
			

				if (stat(argv[i], fileStat) < 0){
					printf("Error Reading Stat\n");
					exit(-1);
				}
				outputFd = open(argv[2], O_WRONLY | O_RDONLY); /* The actual archive file */

				/* TO DO */
				if (errno == ENOENT){
					outputFd = open(argv[2], O_WRONLY | O_CREAT, 0666);
					if (outputFd == -1){
						perror("Error creating archive file");
						exit(-1);
					}
					numWritten = write(outputFd, ARMAG, SARMAG);
					printf("myar: creating %s \n", argv[2]);
			
				}

				/*CHECK ARCHIVE*/
				/*
				lseek(outputFd, 0, SEEK_SET);
				numRead = read(outputFd, buffer, SARMAG);
				//lseek(outputFd, 0, SEEK_SET);
				printf(buffer);
				if (strncmp(buffer, ARMAG, SARMAG) != 0){
					perror("Unknown archive file \n");
					exit(-1);
				}
				*/
				
				lseek(outputFd, 0, SEEK_END);

				memset(name, 0, 16);
				int j = 0;
				for (j = 0; j < 16; j++){
						if (argv[i][j] == '\0'){
							name[j] = '/';
							break;
						}else{
						name[j] = argv[i][j];
						}
				}

				snprintf(header, HEADER_SIZE, "%-16s%-12ld%-6d%-6d%-8o%-10ld%-2s", name, fileStat->st_mtime, fileStat->st_uid, fileStat->st_gid, fileStat->st_mode, fileStat->st_size,ARFMAG);

				numWritten = write(outputFd, header, HEADER_SIZE);

				if (numWritten == -1){

					perror("Error writing file header.");
					unlink(argv[2]);
					exit(-1);
				}
	
				lseek(outputFd, -1, SEEK_CUR);

				while ((numRead = read(inputFd, fileBuffer, BLOCKSIZE)) > 0) {

					numWritten = write(outputFd, fileBuffer, numRead);

				}
	
				if (inputFd == -1){
					exit(-1);
				}

				close(inputFd);
				close(archiveFd);
			}//end arg loop
		}//end if statement

	}/* End Q OPTION */

	if ((argc >= 4) && strcmp(argv[1], "x") == 0){

		inputFd = open(argv[2], O_RDONLY);

		if (inputFd == -1){
			perror("Error opening file.");
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
	if ((argc == 4) && strcmp(argv[1], "d") == 0){

			inputFd = open(argv[2], O_RDONLY);
			outputFd = open("temp.a", O_WRONLY | O_CREAT);

			/*

			if (inputFd == -1){
				perror("Error opening file.");
				exit(-1);
			}

			if (outputFd == -1){
				perror("Error creating file.");
				exit(-1);
			}

			numRead = read(inputFd, buffer, SARMAG);
			if (strncmp(buffer, ARMAG, SARMAG) != 0){
				perror("Unknown archive file \n");
				exit(-1);
			}

			lseek(outputFd, 0, SEEK_SET);
			*/

			//int len = strlen(argv[i]);

			/* FILLING IN THE NAME VARIABLE USED TO COMPARE */


			if (stat(argv[2], fileStat) < 0){
				printf("Error Reading Stat\n");
				exit(-1);
			}

			off_t archive_size = fileStat->st_size;

			while((numRead = read(inputFd, buffer, archive_size)) == archive_size){

				printf(buffer);

				/*
				memset(name, 0, 16);
				int j = 0;
				for (j = 0; j < 16; j++){
						if (argv[3][j] == '\0'){
							name[j] = '/';
							break;
						}else{
						name[j] = argv[3][j];
						}
					}

				snprintf(header, HEADER_SIZE, "%-16s%-12ld%-6d%-6d%-8o%-10ld%-2s", name, fileStat->st_mtime, fileStat->st_uid, fileStat->st_gid, fileStat->st_mode, fileStat->st_size,ARFMAG);
				

				if (strcmp(name, argv[3]) == 0){
			
					lseek(inputFd, filesize, SEEK_CUR)
				}else{
					numWritten = write(outputFd, header, HEADER_SIZE);
					numRead = read(inputFd, fileBuffer, filesize);
					numWritten = write(outputFd, fileBuffer, filesize);
					lseek(inputFd, filesize, SEEK_CUR);
			
					if (filesize % 2 != 0){
						lseek(inputFd, 1, SEEK_CUR);
						close(inputFd);
						close(archiveFd);

					}
				}
				*/
				
			}//end while
	}//end d option
	if ((argc == 3) && strcmp(argv[1], "A") == 0)	{

		DIR *dir = opendir(".");
		struct dirent *entry = NULL;
		outputFd = open(argv[2] , O_WRONLY);
		lseek(outputFd, 0, SEEK_END);

		while((entry = readdir(dir)) != NULL)
		{
			if (stat(entry->d_name, fileStat) < 0)
			{
				printf("Error reading stats");
				exit(-1);
			}

			if (S_ISDIR (fileStat->st_mode)){
				continue;
			}
			if(strncmp(entry->d_name, ".", 1) == 0){
				continue;
			}	
			if(strncmp(entry->d_name, argv[2], strlen(argv[2])) == 0){
				continue;
			}

			memset(name, 0, 16);
			int j = 0;
			for (j = 0; j < 16; j++){
				if (entry->d_name[j] == '\0'){
					name[j] = '/';
					break;
				}else{
					name[j] = entry->d_name[j];
				}
			}
				
			inputFd = open(entry->d_name, O_RDONLY);
			snprintf(header, HEADER_SIZE, "%-16s%-12ld%-6d%-6d%-8o%-10ld%-2s", name, fileStat->st_mtime, fileStat->st_uid, fileStat->st_gid, fileStat->st_mode, fileStat->st_size,ARFMAG);

			numWritten = write(outputFd, header, HEADER_SIZE);

			if (numWritten == -1){
				perror("Error writing file header.");
				exit(-1);
			}

			lseek(outputFd, -1, SEEK_CUR);

			while((numRead = read(inputFd, fileBuffer, BLOCKSIZE)) > 0){
				numWritten = write(outputFd, fileBuffer, numRead);
					
				if (numWritten == -1){
					perror("Error writing file.");
					exit(-1);
				}
			}

			if (fileStat->st_size % 2 != 0){
				numWritten = write(outputFd, "\n", 1);
			}
			
		}//while
		
	}



}/* End Main */

char * /* Return ls(1)-style string for file permissions mask from "The Linux Programming Interface"*/
file_perm_string(mode_t perm, int flags)
{
	static char str[STR_SIZE];
	snprintf(str, STR_SIZE, "%c%c%c%c%c%c%c%c%c",
	         (perm & S_IRUSR) ? 'r' : '-', (perm & S_IWUSR) ? 'w' : '-',
	         (perm & S_IXUSR) ?
	         (((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 's' : 'x') :
	         (((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 'S' : '-'),
	         (perm & S_IRGRP) ? 'r' : '-', (perm & S_IWGRP) ? 'w' : '-',
	         (perm & S_IXGRP) ?
	         (((perm & S_ISGID) && (flags & FP_SPECIAL)) ? 's' : 'x') :
	         (((perm & S_ISGID) && (flags & FP_SPECIAL)) ? 'S' : '-'),
	         (perm & S_IROTH) ? 'r' : '-', (perm & S_IWOTH) ? 'w' : '-',
	         (perm & S_IXOTH) ?
	         (((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 't' : 'x') :
	         (((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 'T' : '-'));
	return str;
}

