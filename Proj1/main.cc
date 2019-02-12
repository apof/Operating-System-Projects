#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include "header.h"
using namespace std;

int main (int argc,char *argv[]) {

srand((unsigned int)time(NULL));

int hashentries;
char *file;

 if (argc==1) {  cout<<"Please give the hashentries!"<<endl; return 0; }
 else if (argc==3)
 {
   if (strcmp(argv[1],"-b")==0) hashentries = atoi(argv[2]);
   else { cout<<"Please give the hashentries"<<endl; return 0; }
 }
 else if (argc==5)
  {
    if (strcmp(argv[1],"-b")==0) { hashentries = atoi(argv[2]); file = new char[strlen(argv[4])+1]; strcpy(file,argv[4]); }
    else  if (strcmp(argv[1],"-l")==0 || strcmp(argv[1],"<")==0 ) {  hashentries = atoi(argv[4]); file = new char[strlen(argv[2])+1]; strcpy(file,argv[2]); }
  }
 else { cout<<"Incorrect parametrs, give only the hashentries and the file to read!"<<endl; return 0; }

char buffer[80];
char *k;
int K;
char *command;
char *id;
int Id;
char *id2;
int Id2;
char *name;
char *surname;
char *gp;
float gpa;
char *courses;
int courses_to_take;
char *department;
char *post_code;
int   post_number;

  record *rec;
  record *r;


  hashTable* table;
  table = new hashTable(hashentries);
  Skip_list list(4,1000000,table);
  cout<<"Hashentries: "<<hashentries<<endl;

  List list_1;
  List list_2;


    ifstream myfile (file,ios::in);

    while ( myfile.getline(buffer,sizeof(buffer)) || cin.getline(buffer,sizeof(buffer)) )
    {
        if(buffer[0]=='i')
         {
           cout<<buffer<<endl;
           command = strtok(buffer, " ");
           id = strtok(NULL, " ");
           Id = atoi(id);
           name = strtok(NULL, " ");
           surname = strtok(NULL, " ");
           gp = strtok(NULL, " ");
           gpa=atof(gp);
           courses = strtok(NULL, " ");
           courses_to_take=atoi(courses);
           department = strtok(NULL, " ");
           post_code = strtok(NULL, " ");
           post_number = atoi(post_code);

          rec = new record(Id,name,surname,gpa,courses_to_take,department,post_number);
          list.insert(Id,rec);
          table->insert(rec,post_number);
          list_1.insert1(rec);
          list_2.insert2(rec);
        }

       else if (buffer[0]=='q')
       {
         cerr<<buffer<<endl;
         command = strtok(buffer, " ");
         id = strtok(NULL, " ");
         Id = atoi(id);
         r=list.search(Id);
         if (r==NULL) cerr<<"Not found!"<<endl;
         else r->print();
        }

       else if (buffer[0] == 'm')
        {
          cerr<<buffer<<endl;
          command = strtok(buffer, " ");
          id = strtok(NULL, " ");
          Id = atoi(id);
          gp = strtok(NULL, " ");
          gpa=atof(gp);
          courses = strtok(NULL, " ");
          courses_to_take = atoi(courses);

          r=list.search(Id);
          if (r==NULL) cerr<<"Not found!"<<endl;
          else { r->modify(gpa,courses_to_take); list_2.delet(r->student_id); list_2.insert2(r); }
        }

       else if (buffer[0] == 'd')
       {
          cerr<<buffer<<endl;
          command = strtok(buffer, " ");
          id = strtok(NULL, " ");
          Id = atoi(id);
          list.delet(Id,&list_2,&list_1);
       }
       else if (buffer[0] == 'r' && buffer[1]=='a')
       {
          cerr<<buffer<<endl;
          command = strtok(buffer, " ");
          id = strtok(NULL, " ");
          Id = atoi(id);
          id2= strtok(NULL," ");
          Id2 = atoi(id2);
          printf("%0.2f\n",list.average(Id,Id2));
       }
        else if (buffer[0] == 'a')
       {
          cerr<<buffer<<endl;
          command = strtok(buffer, " ");
          post_code = strtok(NULL, " ");
          post_number = atoi(post_code);
          printf("%0.2f\n",table->average(post_number));
       }
       else if (buffer[0]=='t' && buffer[1]=='a')
       {
          cerr<<buffer<<endl;
          command = strtok(buffer, " ");
          k = strtok(NULL, " ");
          K = atoi(k);
          post_code = strtok(NULL, " ");
          post_number = atoi(post_code);
          table->print_k_first(post_number,K);
       }
       else if (buffer[0]=='b')
       {
          cerr<<buffer<<endl;
          command = strtok(buffer, " ");
          k = strtok(NULL, " ");
          K = atoi(k);
          list_2.print_bottom(K);
       }
       else if (buffer[0]=='c' && buffer[1]=='t')
        {
          cerr<<buffer<<endl;
          command = strtok(buffer, " ");
          post_code = strtok(NULL, " ");
          post_number = atoi(post_code);
          department = strtok(NULL, " ");
          table->courses_to_take(post_number,department);
        }
        else if(buffer[0] == 'f')
        {
          cerr<<buffer<<endl;
          command = strtok(buffer, " ");
          gp = strtok(NULL, " ");
          gpa = atof(gp);
          list.find(gpa);
        }
        else if (buffer[0] == 'p' && buffer[1]!='e')
        {
          cerr<<buffer<<endl;
          command = strtok(buffer, " ");
          post_code = strtok(NULL, " ");
          post_number = atoi(post_code);
          table->percentile(post_number);
        }
        else if (buffer[0] == 'p' && buffer[1] == 'e')
        { cerr<<buffer<<endl; list_1.print_percentile();  }
        else if (buffer[0]=='e') { cerr<<buffer<<endl;  delete table; cout<<"Exittt!!"<<endl; return 0; }
      }
}
