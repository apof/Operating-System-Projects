#ifndef BLOCK_H
#define BLOCK_H

#define BLOCK_SIZE 512

typedef struct dinode dinode;

typedef struct header{
	int metadataBlk;
	int dirListBlk;
	int dirCount;
	int gzip;
}header;

int writeFile(char *src, int destFd, int destBlk);
int recoverFile(int srcFd, metadata *data, char *path);

int writeMetadata(int destFd, int destBlk, metadata *m);
int recoverMetadata(int srcFd, int srcBlk, metadata *m);

int writeDirList(int destFd, int destBlk, dirListNode *dirList);
int recoverDirList(int srcFd, int srcBlk, dirListNode *dirList);

int writeHeader(int destFd, header *h);
int recoverHeader(int srcFd, header *h);

#endif