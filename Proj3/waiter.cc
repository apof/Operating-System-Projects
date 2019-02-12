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
    int max_money = atoi(argv[6]);
    int money;
    int seg2 = atoi(argv[7]);
    struct shared_mem* s_m;
    struct statistic* s;
    void* mem;
    int i;

     mem = (struct shared_mem*) shmat (segment_id,0,0);
     if (mem==(void*)(-1)) perror("err");
     s_m = (shared_mem*)mem;
     Table* table = (Table*)(s_m + sizeof(shared_mem));

     mem = (struct statistic*) shmat (seg2,0,0);
     if (mem==(void*)(-1)) perror("err");
     s = (statistic*)mem;

    /*printf("waiter time-segment_id: %d - %d\n",time,segment_id);
    printf ("shared memory attached at address %p\n", s_m);
    cout<<"-----------------Waiter-Shared memory initialization check----------------------------------"<<endl;
    printf("Tables and bar status\n");
    cout<<"table and bar size and counter: "<< s_m->table_space<<" "<< s_m->max_bar_space<<" "<< s_m->bar_counter<<endl;
    printf("Table status\n");
    for(i=0; i < s_m->table_space; i++)
      {
       printf("capacity-status-serve-pay-waiter id: %d,%d,%d,%d,%d\n",table[i].space,table[i].status,table[i].serve,table[i].pay,table[i].waiter_id);
      }
    cout<<"----------------------------------------------------------------------------------"<<endl;*/


  sem_t *sem1 = sem_open ("/doorman" , O_RDWR );
  if (sem1 == SEM_FAILED) {
        perror("sem_open doorman failed");
        exit(EXIT_FAILURE);
    }

  sem_t *sem2 = sem_open ("/door_guard" , O_RDWR );
  if (sem2 == SEM_FAILED) {
        perror("sem_open door_guard failed");
        exit(EXIT_FAILURE);
    }

   while(s_m->end) {
	   ////trapezi gia paraggelia

	   for(i=0; i< s_m->table_space; i++)                                          //psaxnei ta trapezia
	   {
           sem_wait(&(table[i].busy));                            //desmeyei to trapezi gia elegxo servirismatos
           if (table[i].serve==1)                                   //ama to trapezi thelei servirisma
            {
	       cout<<"O "<<getpid()<<" waiter vrhke to "<<i<<" trapezi gia servirisma"<<endl;
               table[i].serve=0;                                      //tou servirei
               table[i].waiter_id=getpid();                           //grafei to id tou gia na e3yphrethsei to idio kata thn plhrwmh
               money = rand()%max_money+1;
               s->total_money+=money;
               table[i].money+=money;
               sleep(rand()%time+1);
	       sem_post(&(table[i].table));                           //3emplokarei ton customer wste sth synexeia na faei kai na plhrwsei
	    }
           sem_post(&(table[i].busy));                           //apodesmeyei to trapezi apo ton elegxo servirismatos

	   }

	   ////trapezi plhrwmh
	   for(i=0; i < s_m->table_space; i++)                                                           //psaxnei ta trapezia
	   {
                   sem_wait(&(table[i].busy));                                                //desmeyei to trapezi gia elegxo plhrwmhs
		   if (table[i].pay==1 && table[i].waiter_id == getpid() )                     //ama vrei diko tou trapezi pou thelei na plhrwsei
		   {
			 cout<<"O "<<getpid()<<" waiter vrhke to "<<i<<" trapezi gia plhrwmh"<<endl;
			 table[i].status=0;                                                         //katharizei to status kai to pay  kai to waiter id tou
			 table[i].pay=0;
			 table[i].waiter_id=-1;
                         s->groups_serviced++;
                         sleep(rand()%time+1);
			 sem_post(&(table[i].table));                              //3ypnaei ton customer pou perimenei plhrwmh na fygei

                         sem_wait(sem2);                                                //epikoinwnia me doorman
                         s_m->waiter_request=1;                                         //grafei 1 sto waiter_request
                         sem_post(sem1);                                                //3ypnaei ton doorman gia na volepsei pithanws kapoion apo to bar
                                                                                        //sto trapezi pou adeiase molis
		   }
                   sem_post(&(table[i].busy));                                    //apodesmeyei apo ton elegxo servirismatos to trapezi

	   }

   }

   shmdt(s_m);
   shmdt(s);
   sem_close(sem1);
   sem_close(sem2);

   exit(1);
}
