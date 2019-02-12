#include "error.h"
#include "List.h"
#include "block.h"
#include "dir_funcs.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <utime.h>
#include <sys/types.h>
#include <sys/stat.h>



int writeFile(char *src, int destFd, int destBlk){
	void *buf;
	int i;

	int srcFd;
	struct stat info;

	int blocksNeeded;

	srcFd = open(src, O_RDONLY);
	if(srcFd == -1){
		return ERROR_OPEN;
	}

	if(stat(src, &info) == -1){
		return ERROR_STAT;
	}

	buf = malloc(BLOCK_SIZE);
	if(buf == NULL){
		return ERROR_MALLOC;
	}

	if(lseek(destFd, destBlk * BLOCK_SIZE, SEEK_SET) == (off_t) -1){
		return ERROR_SEEK;
	}

	/*Copy file*/
	blocksNeeded = info.st_size / BLOCK_SIZE + (info.st_size % BLOCK_SIZE > 0 ? 1 : 0);

	for(i = 0; i < blocksNeeded; i++){
		if(read(srcFd, buf, BLOCK_SIZE) == -1){
			return ERROR_READ;
		}

		if(write(destFd, buf, BLOCK_SIZE) == -1){
			return ERROR_WRITE;
		}
	}

	free(buf);

	if(close(srcFd) == -1){
		return ERROR_CLOSE;
	}

	return blocksNeeded;
}

int recoverFile(int srcFd, metadata *data, char *path){
	int i;
	int destFd;
	int blocks;
	void *buf;

	struct utimbuf timebuf;

	char *fullPath;

	/*Allocate buffer*/
	buf = malloc(BLOCK_SIZE);
	if(buf == NULL){
		return ERROR_MALLOC;
	}

	/*Set file descriptor to the starting block of the file*/
	if(lseek(srcFd, data->block * BLOCK_SIZE, SEEK_SET) == (off_t) -1){
		return ERROR_SEEK;
	}

	/*Create output file*/
	// TODO: Check if file with same name already exists

	fullPath = (char*) malloc((strlen(path) + strlen(data->name) + 1) * sizeof(char));
	strcpy(fullPath, path);
	strcat(fullPath, data->name);

	destFd = open(fullPath, O_WRONLY | O_CREAT);
	if(destFd == -1){
		return ERROR_OPEN;
	}


	/*Copy data*/
	blocks = data->size / BLOCK_SIZE;
	for(i = 0; i < blocks; i++){
		if(read(srcFd, buf, BLOCK_SIZE) == -1){
			return ERROR_READ;
		}

		if(write(destFd, buf, BLOCK_SIZE) == -1){
			return ERROR_WRITE;
		}
	}
	/*Copy last (non-full) block*/
	if(data->size % BLOCK_SIZE != 0){
		if(read(srcFd, buf, data->size % BLOCK_SIZE) == -1){
			return ERROR_READ;
		}

		if(write(destFd, buf, data->size % BLOCK_SIZE) == -1){
			return ERROR_WRITE;
		}
	}

	if(close(destFd) == -1){
		return ERROR_CLOSE;
	}

	/*Copy meta-data*/
	if(chmod(fullPath, data->perms) == -1){
		return ERROR_CHMOD;
	}
	if(chown(fullPath, data->user, data->group) == -1){
		return ERROR_CHOWN;
	}

	timebuf.modtime = data->time;
	if(utime(fullPath, &timebuf) == -1){
		return ERROR_CHTIME;
	} 

	free(fullPath);
	free(buf);
	return 1;
}

int writeHeader(int destFd, header *h){
	if(lseek(destFd, 0, SEEK_SET) == (off_t) -1){
		return ERROR_SEEK;
	}

	if(write(destFd, h, sizeof(header)) == -1){
		return ERROR_WRITE;
	}

	return 1;
}

int recoverHeader(int srcFd, header *h){
	if(lseek(srcFd, 0, SEEK_SET) == (off_t) -1){
		return ERROR_SEEK;
	}

	if(read(srcFd, h, sizeof(header)) == -1){
		return ERROR_READ;
	}

	return 1;
}

int writeMetadata(int destFd, int destBlk, metadata *m){
	if(lseek(destFd, destBlk * BLOCK_SIZE, SEEK_SET) == (off_t) -1){
		return ERROR_SEEK;
	}

	if(write(destFd, m, sizeof(metadata)) == -1){
		return ERROR_WRITE;
	}

	return 1;
}

int recoverMetadata(int srcFd, int srcBlk, metadata *m){
	if(lseek(srcFd, srcBlk * BLOCK_SIZE, SEEK_SET) == (off_t) -1){
		return ERROR_SEEK;
	}

	if(read(srcFd, m, sizeof(metadata)) == -1){
		return ERROR_READ;
	}

	return 1;
}

int writeDirList(int destFd, int destBlk, dirListNode *dirList){
	int i;
	int blocksUsed;
	int size = dirListSize(dirList);

	int *dirTable = (int*) malloc((size + 1) * sizeof(int));
	if(dirTable == NULL){
		return ERROR_MALLOC;
	}

	/*Copy the list to a table*/
	for(i = 0; i < size; i++){
		dirTable[i] = dirList->position;
		dirList = dirList->next;
	}
	/*Mark the end with -1*/
	dirTable[i] = -1;

	if(lseek(destFd, destBlk * BLOCK_SIZE, SEEK_SET) == (off_t) -1){
		return ERROR_SEEK;
	}

	if(write(destFd, dirTable, (size + 1) * sizeof(int)) == -1){
		return ERROR_WRITE;
	}

	blocksUsed = ((size + 1) * sizeof(int)) / BLOCK_SIZE;
	blocksUsed += ((size + 1) * sizeof(int)) % BLOCK_SIZE > 0 ? 1 : 0;

	free(dirTable);
	
	return blocksUsed;
}

int recoverDirList(int srcFd, int srcBlk, dirListNode *dirList){
	int p;

	if(lseek(srcFd, srcBlk * BLOCK_SIZE, SEEK_SET) == (off_t) -1){
		return ERROR_SEEK;
	}

	if(read(srcFd, &p, sizeof(int)) == -1){
		return ERROR_READ;
	}
	dirList->position = p;

	if(p > 0){
		if(read(srcFd, &p, sizeof(int)) == -1){
			return ERROR_READ;
		}		
	}
	while(p > 0){
		dirList->next = new dirListNode(p);
		dirList = dirList->next;

		if(read(srcFd, &p, sizeof(int)) == -1){
			return ERROR_READ;
		}
	}

	return 1;
}
