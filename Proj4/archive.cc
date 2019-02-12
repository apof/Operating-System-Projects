#include "List.h"
#include "archive.h"
#include "error.h"
#include "block.h"
#include "dir_funcs.h"

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <utime.h>

#include <iostream>

int dir_num = 0;
int total_num = 0;
int count = 0;
int write_block = 1;

int zipFile(char *fileName){
	char *command = (char*) malloc(3 * (strlen(fileName) + 6) * sizeof(char));
	if(command == NULL){
		return ERROR_MALLOC;
	}

	/*Make temporary copy*/
	sprintf(command, "cp %s %s.copy", fileName, fileName);
	system(command);

	/*Zip copy*/
	sprintf(command, "gzip %s.copy", fileName);
	system(command);

	/*Rename copy*/
	sprintf(command, "mv %s.copy.gz %s.gz", fileName, fileName);
	system(command);

	free(command);
	return 1;
}

int unzipFile(char *fileName){
	char *command = (char*) malloc((strlen(fileName) + 10) * sizeof(char));
	if(command == NULL){
		return ERROR_MALLOC;
	}

	sprintf(command, "gzip -d %s", fileName);
	system(command);

	free(command);

	return 1;
}

int deleteFile(char *fileName){
	char *command = (char*) malloc((strlen(fileName) + 10) * sizeof(char));
	if(command == NULL){
		return ERROR_MALLOC;
	}

	sprintf(command, "rm -f %s", fileName);
	system(command);

	free(command);

	return 1;
}

int getStructure(int archiveFd, List** list, dirListNode*** dirListTable){
	header h;

	int retVal;
	int i, dirIndex;

	*list = new List();
	List dinodeList;
	metadata m;

	retVal = recoverHeader(archiveFd, &h);
	if(retVal < 0){
		return retVal;
	}

	*dirListTable = new dirListNode*[h.dirCount];

	dirIndex = 0;
	for(i = h.metadataBlk; i < h.dirListBlk; i++){
		retVal = recoverMetadata(archiveFd, i, &m);
		if(retVal < 0){
			return retVal;
		}

		(*list)->insert_list(m.perms,m.user,m.group,m.size,m.time,m.name,m.type,m.node_id,m.block,m.level);

		if(m.type == 'd'){
			(*dirListTable)[dirIndex] = new dirListNode(0);
			retVal = recoverDirList(archiveFd, m.block, (*dirListTable)[dirIndex]);
			if(retVal < 0){
				return retVal;
			}
			if((*dirListTable)[dirIndex]->position == -1){
				delete (*dirListTable)[dirIndex];
				(*dirListTable)[dirIndex] = NULL;
			}
			dirIndex++;
		}
	}
}

int deleteStructure(int archiveFd, List* list, dirListNode** dirListTable){
	header h;

	int retVal;
	int i;

	retVal = recoverHeader(archiveFd, &h);
	if(retVal < 0){
		return retVal;
	}

	delete list;

	for(i = 0; i < h.dirCount; i++){
		dirListNode* l = dirListTable[i];
		dirListNode* tmp = dirListTable[i];
		while(tmp!=NULL){
			tmp=l->next;
			delete l;
			l=tmp;
		}
	}

	delete[] dirListTable;

	return 1;
}

int recDir(int archiveFd, List* list, dirListNode **dirListTable, int nodeIndex, char *path){
	struct utimbuf timebuf;
	int i, retVal;
	dirListNode *dirList = dirListTable[dirIndex(list, nodeIndex)];
	
	int listSize = dirListSize(dirList);
	
	dinode *node = list->return_node(nodeIndex);
	
	char *dirName = (char*) malloc((strlen(path) + strlen(node->data.name) + 2) * sizeof(char));
	if(dirName == NULL){
		return ERROR_MALLOC;
	}

	strcpy(dirName, path);
	strcat(dirName, node->data.name);

	/*Create dir with correct metadata*/
	if(mkdir(dirName, node->data.perms) == -1){
		return ERROR_MKDIR;
	}
	if(chown(dirName, node->data.user, node->data.group) == -1){
		return ERROR_CHOWN;
	}

	strcat(dirName, "/");

	for(i = 0; i < listSize; i++){
		node = list->return_node(dirList->position);
		if(node->type == 1){
			/*If node is a file*/
			retVal = recoverFile(archiveFd, &(node->data), dirName);
			if(retVal < 0){
				return retVal;
			}
		}
		else{
			/*If node is a directory*/
			retVal = recDir(archiveFd, list, dirListTable, dirList->position, dirName);
			if(retVal < 0){
				return retVal;
			}
		}

		dirList = dirList->next;
	}

	timebuf.modtime = node->data.time;
	if(utime(dirName, &timebuf) == -1){
		return ERROR_CHTIME;
	}

	free(dirName);
	return 1;
}

int createArchive(char *archiveName, char **targetName, int targetCount, int gzip){
	struct stat statBuf;
	metadata *meta;
	int i, j, dirIndex;

	header h;
	int metadataStart;
	int dirListStart;
	int dirListPoint;

	int archiveFd;
	int targetFd;

	/*Check if archive already exists*/
	if(stat(archiveName, &statBuf) != -1){
		// TODO: Handle already exists error
	}

	/*Check if targets exist*/
	if(targetCount == 0){
		return ERROR_NOTARGET;
	}
	for(i = 0; i < targetCount; i++){
		if(stat(targetName[i], &statBuf) == -1){
			return ERROR_STAT;
		}
	}

	/*Create archive*/
	archiveFd = open(archiveName, O_WRONLY | O_CREAT);
	if(archiveFd == -1){
		return ERROR_OPEN;
	}

	/*Get metadata and save data*/
	dir_num=0;
	total_num=0;
	count=0;
	write_block =1;

	List *directory_list = new List();

	for(i = 0; i < targetCount; i++)
	{
		if (is_regular_file(targetName[i]))	{

			int blocks;

			if (gzip == 1){

				char* path = new char[strlen(targetName[i])+4];
                        	strcpy(path,targetName[i]);

				zipFile(targetName[i]);
				strcat(path,".gz");
				blocks = writeFile(path, archiveFd, write_block);

				if (strchr(targetName[i],'/')==NULL)	fix_list(path,directory_list,path,write_block, gzip);
				else fix_list(path,directory_list,filename_return(path),write_block, gzip);
			}
			else{
				blocks = writeFile(targetName[i], archiveFd, write_block);

				if (strchr(targetName[i],'/')==NULL)	fix_list(targetName[i],directory_list,targetName[i],write_block, gzip);
				else fix_list(targetName[i],directory_list,filename_return(targetName[i]),write_block, gzip);
			}


			write_block += blocks;
			total_num++;
		}
		else{
			if (strchr(targetName[i],'/')==NULL)     fix_list(targetName[i],directory_list,targetName[i],-1, gzip);
			else fix_list(targetName[i],directory_list,filename_return(targetName[i]),-1, gzip);

			dir_num++;
			total_num++;
			listdir(targetName[i],1,directory_list,archiveFd, gzip);
		}
	}

	dirListNode** table;   //ypolistes periexomenwn kathe dir 
	table = new dirListNode*[dir_num];
	for(int j = 0; j < dir_num; j++){
		table[j] = NULL;
	} 

	for(i = 0; i < targetCount; i++){
		// POLY PITHANO NA GINETAI LATHOS
		fix_level(targetName[i],directory_list,table);
		fix_position(targetName[i], directory_list, table);
	}

	/*Save metadata*/
	metadataStart = write_block;
	/*Every node takes one block, so we can caclulate where the dirlists start*/
	dirListStart = metadataStart + total_num;
	dirListPoint = dirListStart;

	dirIndex = 0;
	for(i = 0; i < total_num; i++){
		meta = &(directory_list->return_node(i)->data);

		/*If node is a dir add the dirlist block location*/
		if(meta->type == 'd'){
			meta->block = dirListPoint;
			/*Write dirList and add the blocks used*/
			dirListPoint += writeDirList(archiveFd, meta->block, table[dirIndex]);
			dirIndex++;
		}

		writeMetadata(archiveFd, write_block, meta);
		write_block++;
	}

	cout<<"Dir number: "<<dir_num<<" Total elements: "<<total_num<<endl;
	cout<<"--->Directory inode list!!"<<endl;
	directory_list->print_list();

	cout<<"----->Level lists print"<<endl;
	for(i = 0; i < dir_num; i++){
		//ektypwsh theshs periexomenwn dir sthn inode list
		dirListNode* tmp = table[i];
		cout<<"Elements: "<<list_numberof_elements(table,i)<<"------";
		if (tmp==NULL) cout<<"DIR has not list"<<endl;
		else{
			while(tmp!=NULL){
					tmp->print();
					tmp=tmp->next;
			}
			cout<<endl;
		}
	}

	/*Save header*/
	h.metadataBlk = metadataStart;
	h.dirListBlk = dirListStart;
	h.dirCount = dir_num;
	h.gzip = gzip;
	writeHeader(archiveFd, &h);

	/*Delete structures*/
	delete directory_list;

	for(i = 0; i < dir_num; i++){
		dirListNode* l = table[i];
		dirListNode* tmp = table[i];
		while(tmp!=NULL){
			tmp=l->next;
			delete l;
			l=tmp;
		}
	}

	delete[] table;

	if(close(archiveFd) == -1){
		return ERROR_CLOSE;
	}

	return 1;
}

int extractArchive(char *archiveName, char **targetName, int targetCount){
	int archiveFd;
	int retVal;
	header h;
	int i;
	int nodeCount;

	List* list;
	dirListNode **dirListTable;
	dinode *node;
	
	archiveFd = open(archiveName, O_RDONLY);
	if(archiveFd == -1){
		return ERROR_OPEN;
	}

	/*Recover all metadata*/
	retVal = getStructure(archiveFd, &list, &dirListTable);
	if(retVal < 0){
		return retVal;
	}

	retVal = recoverHeader(archiveFd, &h);
	if(retVal < 0){
		return retVal;
	}

	nodeCount = h.dirListBlk - h.metadataBlk;

	/*Recover files*/
	for(i = 0; i < nodeCount; i++){
		node = list->return_node(i);
		if(node->level == 0){
			if(node->type == 1){
				/*If node is a file*/
				retVal = recoverFile(archiveFd, &(node->data), (char*) "./");
				if(retVal < 0){
					return retVal;
				}
			}
			else{
				/*If node is a directory*/
				retVal = recDir(archiveFd, list, dirListTable, i, (char*) "./");
				if(retVal < 0){
					return retVal;
				}
			}
		}
	}

	retVal = deleteStructure(archiveFd, list, dirListTable);
	if(retVal < 0){
		return retVal;
	}

	if(close(archiveFd) == -1){
		return ERROR_CLOSE;
	}

	return 1;
}

int searchArchive(char *archiveName, char **targetName, int targetCount){
	int i;
	int archiveFd;
	int retVal;

	List* list;
	dirListNode **dirListTable;

	archiveFd = open(archiveName, O_RDONLY);
	if(archiveFd == -1){
		return ERROR_OPEN;
	}

	/*Recover all metadata*/
	retVal = getStructure(archiveFd, &list, &dirListTable);
	if(retVal < 0){
		return retVal;
	}

	for(i = 0; i < targetCount; i++){
		if(search(targetName[i], list,  dirListTable)){
			printf("%s: Found\n", targetName[i]);
		}
		else{
			printf("%s: Not found\n", targetName[i]);
		}		
	}

	retVal = deleteStructure(archiveFd, list, dirListTable);
	if(retVal < 0){
		return retVal;
	}

	if(close(archiveFd) == -1){
		return ERROR_CLOSE;
	}

	return 1;
}

int printArchive(char *archiveName, char **targetName, int targetCount){
	int archiveFd;
	int retVal;

	List* list;
	dirListNode **dirListTable;

	archiveFd = open(archiveName, O_RDONLY);
	if(archiveFd == -1){
		return ERROR_OPEN;
	}

	/*Recover all metadata*/
	retVal = getStructure(archiveFd, &list, &dirListTable);
	if(retVal < 0){
		return retVal;
	}

	print_levels(list);

	retVal = deleteStructure(archiveFd, list, dirListTable);
	if(retVal < 0){
		return retVal;
	}

	if(close(archiveFd) == -1){
		return ERROR_CLOSE;
	}

	return 1;
}

int metaArchive(char *archiveName, char **targetName, int targetCount){
	int archiveFd;
	int retVal;

	List* list;
	dirListNode **dirListTable;

	archiveFd = open(archiveName, O_RDONLY);
	if(archiveFd == -1){
		return ERROR_OPEN;
	}

	/*Recover all metadata*/
	retVal = getStructure(archiveFd, &list, &dirListTable);
	if(retVal < 0){
		return retVal;
	}

	list->print_list();

	retVal = deleteStructure(archiveFd, list, dirListTable);
	if(retVal < 0){
		return retVal;
	}

	if(close(archiveFd) == -1){
		return ERROR_CLOSE;
	}

	return 1;
}