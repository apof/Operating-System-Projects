#include "dir_funcs.h"
#include "block.h"
#include "archive.h"

void print_time(char* name)
{
 struct stat statbuf2;
 struct tm lt;

 char str_access[100];
 char str_modify[100];

 if ( stat( name, &statbuf2 ) == -1) { perror ( " Failed to get file status " ); exit(2); }      //access and modification time

    localtime_r(&statbuf2.st_atime,&lt);
    strftime(str_access, sizeof(str_access), "%c", &lt);

    localtime_r(&statbuf2.st_mtime,&lt);
    strftime(str_modify, sizeof(str_modify), "%c", &lt);

    printf("File: %s\naccessed : %s\nmodified : %s\n" , name, str_access, str_modify);
}

void fix_list(char name[], List* list ,char name2[],int start_block,int lvl){         //e3agei ta metadata enos file h dir 
                                                                                    //kai to eisagei sthn inode list


	const char *modes[]={"---","--x","-w-","-wx","r--","r-x","rw-","rwx"};

	struct stat mybuf;
	char type, perms[10];
	int i,j;

	//print_time(name);

	stat(name, &mybuf);
	switch (mybuf.st_mode & S_IFMT)
	{
		case S_IFREG: type = '-'; break;
		case S_IFDIR: type = 'd'; break;
		default:      type = '?'; break;
  	}

	*perms='\0';

	for(i=2; i>=0; i--)
	{
		j = (mybuf.st_mode >> (i*3)) & 07;
   		strcat(perms,modes[j]);
	}

	//printf("%c%s%3d %5d/%-5d %7d %.12s %s \n",type, perms, mybuf.st_nlink, mybuf.st_uid, mybuf.st_gid,(int)mybuf.st_size, ctime(&mybuf.st_mtime)+4, name);
	//perm,us,gr,siz,tim,nam,typ,id,blk
	list->insert_list(mybuf.st_mode,mybuf.st_uid,mybuf.st_gid,mybuf.st_size,mybuf.st_mtime,name2,type,(int)mybuf.st_ino,start_block,lvl);
}


void fix_level(char *name, List* list, dirListNode** table){         //gia kathe periexomeno enos dir
                                                                    //vriskei th thesh t sth inode list
DIR *dp;                                                            //kai thn eisagei sthn yplolista tou dir
struct dirent *dir;
struct stat mybuf;
char path[1024];
int position;

	if ((dp=opendir(name))== NULL ) { perror("opendir"); return; }

	while ((dir = readdir(dp)) != NULL )
		{
			if (strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0)
			{
				int len = snprintf(path, sizeof(path)-1, "%s/%s", name, dir->d_name);
				path[len] = 0;
				stat(path,&mybuf);
				position=(list->find_position(mybuf.st_ino));
				if (position!=-1){
					if (table[count]==NULL)
						table[count]=new dirListNode(position);
					else{
						dirListNode* tmp = table[count];
						table[count] = new dirListNode(position);
						table[count]->next = tmp;
					}
				}

			}
  		}
		count++;
	//close(dp);
}

void listdir(const char *name, int level, List* list,int file_Dsc,int zip)        //diatrexei olo to directory tree anadromika
{                                                                                 //kai eisagagei sthn inode list ola ta stoixeia tou
    char path[1024];

    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;
    if (!(entry = readdir(dir)))
        return;

   do
	{
        if (entry->d_type == DT_DIR)
		{
			int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
			path[len] = 0;
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)  continue;
			printf("%*s[%s]\n", level*2, "", entry->d_name);
			fix_list(path,list,entry->d_name,-1,level);                               //prosthetei to directory sthn inode list
			dir_num++;
			total_num++;
			listdir(path, level + 1,list,file_Dsc,zip);
        	}
        else
		{
			int blocks;
			int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
			path[len] = 0;
			printf("%*s- %s\n", level*2, "", entry->d_name);
			if (zip == 1)
			{
				zipFile(path);
				strcat(path,".gz");
				blocks = writeFile(path, file_Dsc, write_block);                  //grafei sto tar ta periexomena tou arxeiou
				fix_list(path,list,filename_return(path),write_block,level);	  //prosthetei to file sthn inode list
			}
			else
			{
				blocks = writeFile(path, file_Dsc, write_block);                  //grafei sto tar ta periexomena tou arxeiou
				fix_list(path,list,entry->d_name,write_block,level);

			}
			write_block += blocks;                                       //ananewnei to write block gia na graftei allo arxeio
			total_num++;
		}
    } while (entry = readdir(dir));
    closedir(dir);
}

void fix_position(const char *name,List* list,dirListNode** table)          //diatrexei anadromika ola ta dirs
{                                                                           //gia kathe ena kalei th fix level
	char path[1024];                                                        //gia na dhmiourghthei h yplista tou

	DIR *dir;
	struct dirent *entry;

	if (!(dir = opendir(name)))
	return;
	if (!(entry = readdir(dir)))
        return;

	do
	{
        if (entry->d_type == DT_DIR)
		{
 			int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
			path[len] = 0;
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)  continue;
			fix_level(path,list,table);
			fix_position(path,list,table);
        	}
    	} while (entry = readdir(dir));
    closedir(dir);
}


int list_numberof_elements(dirListNode** table,int position)
{
	dirListNode* tmp = table[position];
	int number=0;
	if (tmp==NULL) return 0;
	else
		{
			while(tmp!=NULL)
				{
					number++;
					tmp=tmp->next;
				}
		}
	return number;
}


int is_regular_file(const char *path)
{
	struct stat path_stat;
	stat(path, &path_stat);
	return S_ISREG(path_stat.st_mode);
}


char* filename_return(char* path) {
	char *ssc;
	int l = 0;
	ssc = strstr(path, "/");
	do{
    		l = strlen(ssc) + 1;
    		path = &path[strlen(path)-l+2];
    		ssc = strstr(path, "/");
	}while(ssc);

	return path;
}

void dir_print(List* list, dirListNode** table)
{
        int position=0;
	dinode* temp1 = list->return_header();
	while(temp1!=NULL)
	{
		if (temp1->type==0)
			{
				dirListNode* tmp = table[position];
				if (tmp==NULL) cout<< temp1->data.name<<" ";
				else
				{
				cout<< temp1->data.name <<"--->  ";
				while(tmp!=NULL)
                                	{
						cout<< (list->return_node(tmp->position))->data.name<<" ";
                                        	tmp=tmp->next;
                                	}
                		}
			position++;
			cout<<endl;
			}
	temp1=temp1->next;
	}

}

int find_name_position(List* list,dirListNode* table,char* nam)
{
	dirListNode* temp = table;
	while(temp!=NULL)
		{
			if (strcmp((list->return_node(temp->position))->data.name,nam)==0)
			return temp->position;

			temp=temp->next;
		}
	return -1;
}


int search(const char* path,List* list,dirListNode** table)
{
	int num=-1;
        int position;
	int flag=0;
	char  buffer[1024];
	strcpy(buffer,path);

	dinode* temp1 = list->return_header();
	char* pch;
	pch = strtok(buffer,"/");

	while(temp1!=NULL && flag!=1)
		{
			if (strcmp(temp1->data.name,pch)==0) { flag=1;}
			temp1=temp1->next;
			num++;
		}
	if (temp1==NULL && flag==0) {return 0; }

	pch = strtok(NULL,"/");

	while(pch!=NULL)
		{
			position = find_name_position(list,table[num],pch);
			num = 0;
			flag=0;
			temp1 = list->return_header();
			while(num<position)
				{
					if (temp1->type==0)
					flag++;
					temp1=temp1->next;
					num++;
				}
			if (position==-1) {return 0; }
			num=flag;
			pch = strtok(NULL,"/");
		}
	return 1;

}


int return_dir_pos(List* list,int id)
{
	dinode* l = list->return_header();
	int pos=0;
	while(l->data.node_id!=id) { pos++; l=l->next; }

	return pos;

}

void print_levels(List* list)
{
	dinode* tmp = list->return_header();
	while(tmp!=NULL)
	{
		if (tmp->type==0) printf("%*s[%s]\n",2*(tmp->level),"",tmp->data.name);
		else printf("%*s-%s\n",2*(tmp->level),"",tmp->data.name);

		tmp=tmp->next;
	}

}

int dirIndex(List *list, int nodeIndex){
	int dirCount = 0;
	int i;
	dinode *node = list->return_node(0);

	for(i = 0; i < nodeIndex; i++){
		if(node->type == 0){
			dirCount++;
		}
		node = node->next;
	}

	return dirCount;
}

int dirListSize(dirListNode* l){
	int size = 0;

	while(l != NULL){
		size++;
		l = l->next;
	}

	return size;	
}