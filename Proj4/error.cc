#include "error.h"
#include <stdio.h>

void printError(int errorVal){
	switch(errorVal){
		case ERROR_OPEN:
			perror("Error opening file");
			break;
		case ERROR_CLOSE:
			perror("Error closing file");
			break;
		case ERROR_STAT:
			perror("Error stat-ing file");
			break;
		case ERROR_MALLOC:
			perror("Error allocating memory");
			break;
		case ERROR_SEEK:
			perror("Error seeking in file");
			break;
		case ERROR_READ:
			perror("Error reading from file");
			break;
		case ERROR_WRITE:
			perror("Error writing file");
			break;
		case ERROR_NOTARGET:
			fprintf(stderr, "Error: No target given\n");
			break;
		case ERROR_CHMOD:
			perror("Error changing permissions");
			break;
		case ERROR_CHOWN:
			perror("Error changing owner");
			break;
		case ERROR_CHTIME:
			perror("Error changing time");
			break;
		case ERROR_MKDIR:
			perror("Error creating directory");
			break;
		case ERROR_USAGE:
			fprintf(stderr, "Error: Incorrect usage\n");
			break;
		default:
			fprintf(stderr, "Error not recognised!\n");
	}
}