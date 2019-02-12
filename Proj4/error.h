#ifndef ERROR_H
#define ERROR_H

#define ERROR_OPEN		-1
#define ERROR_CLOSE		-2
#define ERROR_STAT		-3
#define ERROR_MALLOC	-4
#define ERROR_SEEK		-5
#define ERROR_READ		-6
#define ERROR_WRITE		-7
#define ERROR_NOTARGET	-8
#define ERROR_CHMOD		-9
#define ERROR_CHOWN		-10
#define ERROR_CHTIME	-11
#define ERROR_USAGE		-12
#define ERROR_MKDIR		-13

void printError(int errorVal);

#endif