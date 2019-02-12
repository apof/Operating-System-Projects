#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "header.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <semaphore.h>
#include <sys/ipc.h>
#include <fcntl.h>

#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

using namespace std;

int main (int argc, char* argv[]) {

int table_capacity[4];
table_capacity[0]=2;
table_capacity[1]=4;
table_capacity[2]=6;
table_capacity[3]=8;

pid_t pids_door[1];

////////////////////SEMAPHORES///////////////////////////
sem_t *sem1 = sem_open ("/queue" , O_CREAT|O_EXCL, SEM_PERMS , 1);     //h oura ths eisodou kathe fora mono enas customer mpainei

if (sem1 == SEM_FAILED) {
      cout<<"sem queue exists"<<endl;
       sem_close(sem1);
       sem_unlink("/queue");
       sem1 = sem_open ("/queue" ,O_CREAT , SEM_PERMS , 1);
    }
////////////

sem_t *sem2 = sem_open ("/doorman" , O_CREAT|O_EXCL , SEM_PERMS , 0);  //o doorman 3ypnaei eite apo customer st eisodo eite apo customer pou feygei apo trapezi

if (sem2 == SEM_FAILED) {
       cout<<"sem doorman exists"<<endl;
       sem_close(sem2);
       sem_unlink("/doorman");
       sem2 = sem_open ("/doorman" ,O_CREAT , SEM_PERMS , 0);
    }
///////////
sem_t *sem3 = sem_open ("/customer" , O_CREAT|O_EXCL , SEM_PERMS  , 0);  //pagwnei to customer wste na perimenei thn apanthsh tou doorman opou tha ton 3ypnhsei

if (sem3 == SEM_FAILED) {
      cout<<"sem customer exists"<<endl;
       sem_close(sem3);
       sem_unlink("/customer");
       sem3 = sem_open ("/customer" ,O_CREAT , SEM_PERMS , 0);
    }
/////////

sem_t *sem4 = sem_open ("/door_guard" , O_CREAT|O_EXCL , SEM_PERMS  , 1);  //prostateyei thn epikoinwnia doorman me enan apo tous waiter h doorman kathe fora
                                                                         //wste o doorman na parei to swsto request
if (sem4 == SEM_FAILED) {
      cout<<"door_guard exists"<<endl;
       sem_close(sem4);
       sem_unlink("/door_guard");
       sem4 = sem_open ("/door_guard" ,O_CREAT , SEM_PERMS , 1);
    }
/////////////////////BAR SEMAPHORES////////////////////////////

sem_t *sem5 = sem_open ("/bar_2" , O_CREAT|O_EXCL , SEM_PERMS  , 0);
if (sem5 == SEM_FAILED) {
      cout<<"bar_2 exists"<<endl;
       sem_close(sem5);
       sem_unlink("/bar_2");
       sem5 = sem_open ("/bar_2" ,O_CREAT , SEM_PERMS , 0); }

sem_t *sem6 = sem_open ("/bar_4" , O_CREAT|O_EXCL , SEM_PERMS  , 0);
if (sem6 == SEM_FAILED) {
      cout<<"bar_4 exists"<<endl;
       sem_close(sem6);
       sem_unlink("/bar_4");
       sem6 = sem_open ("/bar_4" ,O_CREAT , SEM_PERMS , 0); }

sem_t *sem7 = sem_open ("/bar_6" , O_CREAT|O_EXCL , SEM_PERMS  , 0);
if (sem7 == SEM_FAILED) {
      cout<<"bar_6 exists"<<endl;
       sem_close(sem7);
       sem_unlink("/bar_6");
       sem7 = sem_open ("/bar_6" ,O_CREAT , SEM_PERMS , 0); }

sem_t *sem8 = sem_open ("/bar_8" , O_CREAT|O_EXCL , SEM_PERMS  , 0);
if (sem8 == SEM_FAILED) {
      cout<<"bar_8 exists"<<endl;
       sem_close(sem8);
       sem_unlink("/bar_8");
       sem8 = sem_open ("/bar_8" ,O_CREAT , SEM_PERMS , 0); }

////////////////////////////////////////////////////////////////////

sem_t *sem9 = sem_open ("/bar_guard" , O_CREAT|O_EXCL , SEM_PERMS  , 1);
if (sem9 == SEM_FAILED) {
      cout<<"bar_guard exists"<<endl;
       sem_close(sem9);
       sem_unlink("/bar_guard");
       sem9 = sem_open ("/bar_guard" ,O_CREAT , SEM_PERMS , 1); }


///////////////////////////////////////////////////////////////////





int val;
cout<<"SEMAPHORES INITIALIZATION"<<endl;
sem_getvalue(sem2,&val);
cout<<"doorman "<<val<<endl;
sem_getvalue(sem3,&val);
cout<<"customer "<<val<<endl;
sem_getvalue(sem1,&val);
cout<<"queue "<<val<<endl;
sem_getvalue(sem4,&val);
cout<<"door_guard "<<val<<endl;
sem_getvalue(sem5,&val);
cout<<"bar_2 "<<val<<endl;
sem_getvalue(sem6,&val);
cout<<"bar_4 "<<val<<endl;
sem_getvalue(sem7,&val);
cout<<"bar_6 "<<val<<endl;
sem_getvalue(sem8,&val);
cout<<"bar_8 "<<val<<endl;
sem_getvalue(sem9,&val);
cout<<"bar_guard "<<val<<endl;
cout<<"-------------"<<endl;




sem_close(sem1);   //close tous semaphotes afou den xrisimopoiountai ston parent
sem_close(sem2);
sem_close(sem3);
sem_close(sem4);
sem_close(sem5);
sem_close(sem6);
sem_close(sem7);
sem_close(sem8);
sem_close(sem9);

/////////////////////////////////////////////////////////

 int group_number;

 int k;
 char* ConfigFile;
 int Time;
 int Customers;

 if (argc!=7) { cout<<"You have to give 3 arguments!!"<<endl;  return 0; }
 else {
	 for (k=1; k<=6; k=k+2)
         {
          if (strcmp(argv[k],"-n")==0) { Customers=atoi(argv[k+1]); }
          if (strcmp(argv[k],"-l")==0) { ConfigFile = new char[strlen(argv[k+1]) + 1]; strcpy(ConfigFile,argv[k+1]); }
          if (strcmp(argv[k],"-d")==0) { Time = atoi(argv[k+1]); }
	 }
      }
 cout<<"customers-configfile-time for the restaurant: "<<Customers<<"-"<<ConfigFile<<"-"<<Time<<endl;

 pid_t pids_cust[Customers];

 int table_num;
 int max_table_capacity;
 int bar_num;
 int waiters;
 char buffer[15];

 ifstream myfile (ConfigFile,ios::in);
 myfile.getline(buffer,sizeof(buffer));

 table_num = atoi(strtok(buffer, "-"));
 max_table_capacity = atoi(strtok(NULL, "-"));
 bar_num = atoi(strtok(NULL, "-"));
 waiters = atoi(strtok(NULL, ";"));
 cout<<"-----Restaurant Information"<<endl;
 cout<<"Table number,max table capacity,bar capacity and number of waiters are: "<<table_num<<"-"<<max_table_capacity<<"-"<<bar_num<<"-"<<waiters<<endl;

  pid_t pids_waiter[waiters];

  srand(time(NULL));

  int segment_id;
  int seg2;

  struct shared_mem* shared_memory;
  struct statistic* s;
  void *mem;
  int shared_segment_size = sizeof(*shared_memory) + table_num*sizeof(Table);
  printf ("segment size: %d\n", shared_segment_size);
  int time = 4;

  /* Allocate a shared memory segment.  */
  segment_id = shmget (IPC_PRIVATE, shared_segment_size,IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
  cout<<"Segment id: "<<segment_id<<endl;

  seg2 = shmget (IPC_PRIVATE, sizeof(*s),IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

  /* Attach the shared memory segment.  */
  mem = (struct shared_mem*) shmat (segment_id, 0, 0);
  shared_memory = (shared_mem*)mem;

  mem = (struct statistic*) shmat(seg2,0,0);
  s = (statistic*)mem;

  printf ("shared memory attached at address for the restaurant %p\n", shared_memory);

  //putting table pointer correctly
  shared_memory->table = (Table*)(shared_memory + sizeof(shared_mem));

  /* Initializing shared memory */
  int i;
  for(i=0; i< table_num; i++)
  {
     shared_memory->table[i].space=table_capacity[rand()%4];
     shared_memory->table[i].status=0;
     shared_memory->table[i].serve=0;
     shared_memory->table[i].pay=0;
     shared_memory->table[i].waiter_id=-1;
     sem_init(&(shared_memory->table[i].busy),1,1);
     sem_init(&(shared_memory->table[i].table),1,0);
     shared_memory->table[i].money=0;
  }
  shared_memory->waiter_request = - 1;
  shared_memory->customer_request = -1;
  shared_memory->max_bar_space = bar_num;
  shared_memory->bar_counter=0;
  shared_memory->table_space = table_num;
  shared_memory->door_answer = 0;
  shared_memory->group_size = 0;
  shared_memory->table_answer = -1;
  shared_memory->bar2 = 0;
  shared_memory->bar4 = 0;
  shared_memory->bar6 = 0;
  shared_memory->bar8 = 0;
  shared_memory->end = 1;
  ////////statistic segment
  s->total_money=0;
  s->groups_serviced=0;
  s->groups_left_after_bar=0;
  s->groups_left=0;
  /////////statistics arxika////////////////////
  cout<<"STATISTICS ARXIKA"<<endl;
  cout<<"total_money: "<< s->total_money << endl;
  cout<<"groups_serviced: "<< s->groups_serviced<<endl;
  cout<<"groups_left: "<< s->groups_left<<endl;
  cout<<"groups_left_after_bar: " << s->groups_left_after_bar<<endl;
  cout<<"bar_counter: " << shared_memory->bar_counter<<endl;
  ////////////////////////////////////////////////

 cout<<"TABLE SEMAPHORES INITIALIZATION"<<endl;
 for (i=0; i<table_num; i++) { sem_getvalue( &(shared_memory->table[i].busy),&val); cout<<val<<"-";  sem_getvalue(&(shared_memory->table[i]).table,&val); cout<<val<<endl; }

 cout<<"TABLE INITIALIZATION"<<endl;
 for (i=0; i<table_num; i++)
 cout<<"space-status-money: "<< shared_memory->table[i].space<<" "<< shared_memory->table[i].status<<" "<< shared_memory->table[i].money<<endl;

 /////////////DOORMAN CREATION////////////////////////
  if ( (pids_door[0]=fork())<0) { printf("fork error"); return 0; }
  else if(pids_door[0]==0)
  {
    char seg_id[15];
    char tim[10];
    sprintf(seg_id,"%d",segment_id);
    sprintf(tim,"%d",time);
    execl("./doorman","doorman","-d",tim,"-s",seg_id,NULL);
    exit(0);
  }
 ///////////////WAITERS CREATION////////////////////

 for(i=0; i<waiters; i++)
  {
   if ((pids_waiter[i]=fork())<0) { printf("fork error"); return 0; }
   else if (pids_waiter[i]==0)
     {
    char mon[7];
    char seg_id[15];
    char s[15];
    char tim[10];
    sprintf(seg_id,"%d",segment_id);
    sprintf(s,"%d",seg2);
    sprintf(tim,"%d",time);
    sprintf(mon,"%d",rand()%50+5);
    execl("./waiter","waiter","-d",tim,"-s",seg_id,"-m",mon,s,NULL);
    exit(0);
    }
  }

  ///////////////////CUSTOMERS CREATION//////////////////
   int num[Customers];

  for(i=0; i<Customers; i++)
  {
   if ((pids_cust[i]=fork())<0) { printf("fork error"); return 0; }
   else if (pids_cust[i]==0)
     {
    char seg_id[15];
    char tim[10];
    char num[5];
    char s[15];
    sprintf(seg_id,"%d",segment_id);
     sprintf(s,"%d",seg2);
    sprintf(tim,"%d",time);
    srand(getpid());
    sprintf(num,"%d",rand()%max_table_capacity+1);
    execl("./customer","customer","-n",num,"-d",tim,"-s",seg_id,s,NULL);
    exit(0);
    }
  }

int p;
for(p=0; p<Customers; p++)  wait(NULL);
shared_memory->end=0;
////////////////////////////////////////////////////////////////


   cout<<"STATS"<<endl;
   cout<<"status-money per table: "<<endl;
   for(i=0; i< shared_memory->table_space; i++) cout<<"-->"<< shared_memory->table[i].status <<"-"<< shared_memory->table[i].money <<endl;
   cout<<"Bar status: "<< shared_memory->bar_counter<<endl;
   cout<<"Groups serviced: "<< s->groups_serviced<<endl;
   cout<<"Groups left: "<< s->groups_left<<endl;
   cout<<"Groups left after bar: "<< s->groups_left_after_bar<<endl;
   cout<<"Total money: "<< s->total_money<<endl;

   for(int k=0; k<table_num; k++)
   {
	   sem_destroy(&(shared_memory->table[i].busy));
	   sem_destroy(&(shared_memory->table[i].table));
   }//destroy semaphores of the table

   /* Detach the shared memory segment.  */
  shmdt (shared_memory);
  shmdt(s);

  /* Deallocate the shared memory segment.  */
  shmctl (segment_id, IPC_RMID, 0);
  shmctl (seg2, IPC_RMID, 0);


  sem_unlink ( "/queue" ); // remove named semaphores from system
  sem_unlink ( "/doorman" );
  sem_unlink ( "/customer" );
  sem_unlink("/door_guard");
  sem_unlink("/bar_2");
  sem_unlink("/bar_4");
  sem_unlink("/bar_6");
  sem_unlink("/bar_8");
  sem_unlink("/bar_guard");


  cout<<"TELOS"<<endl;

  return 0;
}
