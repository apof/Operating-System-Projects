#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "header.h"
#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>

using namespace std;

int main(int argc,char* argv[])
{
    int time = atoi(argv[2]);
    int segment_id = atoi(argv[4]);
    struct shared_mem* s_m;
    void* mem;
    int i;

     mem  = (struct shared_mem*) shmat (segment_id,0,0);
     if (mem==(void*)(-1)) perror("err");
     s_m = (shared_mem*)mem;

     Table* table = (Table*)(s_m + sizeof(shared_mem));

    /*printf("door time-segment_id: %d - %d\n",time,segment_id);
    printf ("shared memory attached at address %p\n", s_m);
    cout<<"-----------------Doorman-Shared memory initialization check------------------"<<endl;
    printf("Tables and bar status\n");
    cout<<"table and bar size and counter: "<< s_m->table_space<<" "<< s_m->max_bar_space<<" "<< s_m->bar_counter<<endl;
    for(i=0; i < s_m->table_space; i++)
      {
       printf("capacity-status-serve-pay-waiter id: %d,%d,%d,%d,%d\n",table[i].space,table[i].status,table[i].serve,table[i].pay,table[i].waiter_id);
      }

    printf("waiter_request, customer_request are: %d %d\n",s_m->waiter_request,s_m->customer_request);
    cout<<"----------------------------------------------------------------------------------"<<endl;*/

  sem_t *sem1 = sem_open ("/doorman" , O_RDWR );
  if (sem1 == SEM_FAILED) {
        perror("sem_open doorman failed");
        exit(EXIT_FAILURE);
    }

  sem_t *sem2 = sem_open ("/customer" , O_RDWR );
  if (sem2 == SEM_FAILED) {
        perror("sem_open customer  failed");
        exit(EXIT_FAILURE);
    }

  sem_t *sem3 = sem_open ("/door_guard" , O_RDWR );
  if (sem3 == SEM_FAILED) {
        perror("sem_open door_guard failed");
        exit(EXIT_FAILURE); }

   sem_t *sem4 = sem_open ("/bar_guard" , O_RDWR );
  if (sem4 == SEM_FAILED) {
        perror("sem_open bar_guard failed");
        exit(EXIT_FAILURE); }

  sem_t *sem5 = sem_open ("/bar_2" , O_RDWR );
  if (sem5 == SEM_FAILED) {
        perror("sem_open bar_2 failed");
        exit(EXIT_FAILURE); }

 sem_t *sem6 = sem_open ("/bar_4" , O_RDWR );
  if (sem6 == SEM_FAILED) {
        perror("sem_open bar_4 failed");
        exit(EXIT_FAILURE); }

  sem_t *sem7 = sem_open ("/bar_6" , O_RDWR );
  if (sem7 == SEM_FAILED) {
        perror("sem_open bar_6 failed");
        exit(EXIT_FAILURE); }

  sem_t *sem8 = sem_open ("/bar_8" , O_RDWR );
  if (sem8 == SEM_FAILED) {
        perror("sem_open bar_8 failed");
        exit(EXIT_FAILURE); }


   int value;


    while(s_m->end) {

   sem_wait(sem1);

   cout<<"Doorman check"<<endl;
   if (s_m->waiter_request==1)          //periptwsh pou adeiase trapezi
    {
       cout<<"O doorman tsekarei gia na volepsei ama mporei pelates apo to bar se trapezi"<<endl;
       for(i=0; i < s_m->table_space; i++)
        {
          if (table[i].status==0)
		  {       sleep(rand()%time+1);
			  if (table[i].space==2)
			  {
				 sem_wait(sem4);
                                 if (s_m->bar2!=0) { sem_post(sem5); s_m->table_answer = i; }
                                 else if  (s_m->bar4!=0) { sem_post(sem6); s_m->table_answer = i; }
				 else if  (s_m->bar6!=0) { sem_post(sem7); s_m->table_answer = i; }
				 else if  (s_m->bar8!=0) { sem_post(sem8); s_m->table_answer = i; }
				 sem_post(sem4);
			  }
			  else if (table[i].space ==4)
			  {
				 sem_wait(sem4);
                                 if  (s_m->bar4!=0) { sem_post(sem6); s_m->table_answer = i; }
				 else if  (s_m->bar6!=0) { sem_post(sem7); s_m->table_answer = i; }
				 else if  (s_m->bar8!=0) { sem_post(sem8); s_m->table_answer = i; }
				 sem_post(sem4);
			  }
			  else if (table[i].space ==6)
			  {
				 sem_wait(sem4);
				 if  (s_m->bar6!=0) { sem_post(sem7); s_m->table_answer = i; }
				 else if  (s_m->bar8!=0) { sem_post(sem8); s_m->table_answer = i; }
				 sem_post(sem4);
			  }
			  else
			  {
				 sem_wait(sem4);
                                 if  (s_m->bar8!=0) { sem_post(sem8); s_m->table_answer = i; }
				 sem_post(sem4);
			  }
		  }
        }
     s_m->waiter_request=-1;
    }
   else if (s_m->customer_request==1) //periptwsh pelath sthn oura
    {
       sleep(rand()%time+1);
       cout<<"o doorman tsekarei gia trapezi "<< s_m->group_size <<" atomwn"<<endl;

       for(i=0; i< s_m->table_space; i++)
       {
          cout<<"------>"<<table[i].space<<" "<<table[i].status<<endl;

        if (table[i].space >= s_m->group_size && table[i].status==0)
          {
             cout<<"o doorman vrhke trapezi"<<endl;
             s_m->door_answer=1;
             break;
          }
       }
    s_m->customer_request=-1;

    cout<<"3ypnaei ton customer gia na ton enhmerwsei  "<<endl;

    sem_post(sem2);
    }
    sem_post(sem3);

   }

   shmdt (s_m);
   sem_close (sem1);
   sem_close(sem2);
   sem_close(sem3);
   sem_close(sem4);
   sem_close(sem5);
   sem_close(sem6);
   sem_close(sem7);
   sem_close(sem8);
}
