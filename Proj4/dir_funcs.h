#ifndef DIR_FUNCS
#define DIR_FUNCS

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "List.h"

using namespace std;

extern int dir_num;                                                //arithmos dirs tou arxeiou
extern int total_num;                                              //synolikos arithmos dird kai files
extern int count;                                                  //metavlhth xrhsimh g thn eisagwgh stis ypolistes
extern int write_block;                                            //arxiko block pou tha graftei h plhroforia enos file

void print_time(char*);                                     //ektypwnei modification and access time
//////
void fix_list(char*,List*,char*,int,int);                       //eisagei sth lista ena file h dir
void listdir(const char*,int,List*,int,int);                    //kalei anadromika ose fores xreiazetai th fix list
//////                                                      //wste na apothikeysoume ola t file kai dirs
void fix_level(char*,List*,dirListNode**);                  //ftiaxnei th mikrh lista gia ta periexomena kathe dir
void fix_position(const char*,List*,dirListNode**);         //kaleitai anadromika gia ta levels kathe dir
//////
int list_numberof_elements(dirListNode**,int);              //gyrnaei to megethos mias mikrhs ypolistas
//////
int is_regular_file(const char *path);				//elegxei ama ena orisma einai file h directory
//////
char* filename_return(char* path);				//epistrefei se periptwsh pou dwsoume path to onoma tou telikou file h dir
/////
void dir_print(List* list, dirListNode** table);		//ektypwnei ola ta files kai dirs deixnotas tis syndeseis tous
/////
int find_name_position(List*,dirListNode*,char*);
/////
int search(const char*,List*,dirListNode**);
/////
void print_levels(List* list);
/////
int dirIndex(List *list, int nodeIndex);
/////
int dirListSize(dirListNode* l);


#endif