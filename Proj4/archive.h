#ifndef ARCHIVE_H

int zipFile(char *fileName);
int unzipFile(char *fileName);
int deleteFile(char *fileName);

int createArchive(char *archiveName, char **targetName, int targetCount, int gzip);
int extractArchive(char *archiveName, char **targetName, int targetCount);
int searchArchive(char *archiveName, char **targetName, int targetCount);
int printArchive(char *archiveName, char **targetName, int targetCount);
int metaArchive(char *archiveName, char **targetName, int targetCount);


#define ARCHIVE_H
#endif