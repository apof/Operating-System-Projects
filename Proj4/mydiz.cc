#include "archive.h"
#include "error.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	/*Assisting variables*/
	int i, j;
	int retVal;
	int flagSum;

	/*Command line variables*/
	int cFlag = 0;
	int aFlag = 0;
	int xFlag = 0;
	int jFlag = 0;
	int dFlag = 0;
	int mFlag = 0;
	int qFlag = 0;
	int pFlag = 0;	

	char *archiveName = NULL;
	char **targetName = NULL;
	int targetCount = 0;

	/*Get command line arguments*/
	for(i = 1; i < argc; i++){
		if(argv[i][0] == '-'){
			/*If argument is a flag*/
			for(j = 1; j < strlen(argv[i]); j++){
				if(argv[i][j] == 'c'){
					if(cFlag == 0){
						cFlag = 1;
					}
					else{
						printError(ERROR_USAGE);
						exit(ERROR_USAGE);
					}					
				}
				else if(argv[i][j] == 'a'){
					if(aFlag == 0){
						aFlag = 1;
					}
					else{
						printError(ERROR_USAGE);
						exit(ERROR_USAGE);
					}					
				}
				else if(argv[i][j] == 'x'){
					if(xFlag == 0){
						xFlag = 1;
					}
					else{
						printError(ERROR_USAGE);
						exit(ERROR_USAGE);
					}					
				}
				else if(argv[i][j] == 'j'){
					if(jFlag == 0){
						jFlag = 1;
					}
					else{
						printError(ERROR_USAGE);
						exit(ERROR_USAGE);
					}					
				}
				else if(argv[i][j] == 'd'){
					if(dFlag == 0){
						dFlag = 1;
					}
					else{
						printError(ERROR_USAGE);
						exit(ERROR_USAGE);
					}					
				}
				else if(argv[i][j] == 'm'){
					if(mFlag == 0){
						mFlag = 1;
					}
					else{
						printError(ERROR_USAGE);
						exit(ERROR_USAGE);
					}					
				}
				else if(argv[i][j] == 'q'){
					if(qFlag == 0){
						qFlag = 1;
					}
					else{
						printError(ERROR_USAGE);
						exit(ERROR_USAGE);
					}					
				}
				else if(argv[i][j] == 'p'){
					if(pFlag == 0){
						pFlag = 1;
					}
					else{
						printError(ERROR_USAGE);
						exit(ERROR_USAGE);
					}					
				}
				else{
					printError(ERROR_USAGE);
					exit(ERROR_USAGE);
				}
			}
		}
		else{
			/*If argument is a file/dir*/
			archiveName = argv[i];
			i++;

			if(i < argc){
				targetCount = argc - i;
				targetName = (char**) malloc(targetCount * sizeof(char*));
				if(targetName == NULL){
					printError(ERROR_MALLOC);
					exit(ERROR_MALLOC);
				}

				for(j = 0; i < argc; i++){
					targetName[j] = argv[i];
					j++;
				}
			}
		}
	}

	/*Check if flags are valid*/
	flagSum = cFlag + aFlag + xFlag + jFlag + dFlag + mFlag + qFlag + pFlag;

	if(mFlag || qFlag || pFlag || dFlag || xFlag){
		/*These flags must always be used alone*/
		if(flagSum != 1){
			printError(ERROR_USAGE);
			exit(ERROR_USAGE);
		}
	}
	else if(cFlag || aFlag){
		if(flagSum > 2){
			printError(ERROR_USAGE);
			exit(ERROR_USAGE);			
		}
		else if(flagSum == 2 && jFlag != 1){
			printError(ERROR_USAGE);
			exit(ERROR_USAGE);
		}
	}
	else{
		/*If only -j is used*/
		printError(ERROR_USAGE);
		exit(ERROR_USAGE);
	}

	/*Start appropriate task*/
	if(cFlag){
		retVal = createArchive(archiveName, targetName, targetCount, jFlag);
	}
	else if(xFlag){
		retVal = extractArchive(archiveName, targetName, targetCount);
	}
	else if(qFlag){
		retVal = searchArchive(archiveName, targetName, targetCount);
	}
	else if(pFlag){
		retVal = printArchive(archiveName, targetName, targetCount);
	}
	else if(mFlag){
		retVal = metaArchive(archiveName, targetName, targetCount);
	}

	if(retVal < 0){
		printError(retVal);
	}

	if(targetName != NULL){
		free(targetName);		
	}
	return 0;
}