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

#define MAXTIME 10

using namespace std;

int main(int argc,char* argv[])
{

    srand(getpid()+time(NULL));

    timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);

    int i,table_num,err=1;
    int group_number = atoi(argv[2]);
    int num;
    int time = atoi(argv[4]);
    int segment_id = atoi(argv[6]);
    int seg2=atoi(argv[7]);
    struct statistic* s;
    struct shared_mem* s_m;
    void* mem;

    mem = (struct shared_mem*) shmat (segment_id,0, 0);
    if (mem==(void*)(-1)) perror("err");
    s_m = (shared_mem*)mem;
    Table *table = (Table*)(s_m + sizeof(shared_mem));

     mem = (struct statistic*) shmat (seg2,0, 0);
    if (mem==(void*)(-1)) perror("err");
    s = (statistic*)mem;

    int want_bar;

   /* printf("time-segment_id: %d - %d\n",time,segment_id);
    printf ("shared memory attached at address %p\n", s_m);
    cout<<"-----------------Customer-Shared memory initialization check----------------------------------"<<endl;
    printf("Tables and bar status\n");
    cout<<"table and bar size and counter: "<< s_m->table_space<<" "<< s_m->max_bar_space<<" "<< s_m->bar_counter<<endl;
     printf("Table status\n");
    for(i=0; i < s_m->table_space; i++)
      {
       printf("capacity-status-serve-pay-waiter id: %d,%d,%d,%d,%d\n",table[i].space,table[i].status,table[i].serve,table[i].pay,table[i].waiter_id);
      }

    printf("waiter_request, customer_request are: %d %d\n",s_m->waiter_request,s_m->customer_request);
    cout<<"----------------------------------------------------------------------------------"<<endl; */

	sem_t *sem1 = sem_open ("/doorman" , O_RDWR );
	sem_t *sem2 = sem_open ("/customer" , O_RDWR );
	sem_t *sem3 = sem_open ("/queue" , O_RDWR );
        sem_t *sem4 = sem_open ("/door_guard" , O_RDWR );

        sem_t *sem5 = sem_open ("/bar_2" , O_RDWR );
        sem_t *sem6 = sem_open ("/bar_4" , O_RDWR );
        sem_t *sem7 = sem_open ("/bar_6" , O_RDWR );
        sem_t *sem8 = sem_open ("/bar_8" , O_RDWR );

        sem_t *sem9 = sem_open ("/bar_guard" , O_RDWR );


        int value;
        int ex=0;

        want_bar = rand()%2;
        cout<<"O customer "<<getpid()<<" eftase kai want_bar "<<want_bar<<endl;

	sem_wait(sem3);

        cout<<"O Customer "<<getpid()<<" arxizei na epikoinwnei me doorman"<<endl;

        sem_wait(sem4);

	s_m->customer_request=1;
        s_m->group_size=group_number;
        cout<<"epikoinwnia me doorman request-atoma: "<< s_m->customer_request<<" "<< s_m->group_size<<endl;
        sem_post(sem1);
        sem_wait(sem2);

	if (s_m->door_answer==1)
	{
	cout<<"Vrethhke trapezi gia ton pelath: "<<getpid()<<endl;
	for(i=0; i < s_m->table_space; i++)
         {
          if (table[i].space >= group_number && table[i].status==0)
          {
	         table_num=i;
                 table[i].status=1;
	         cout<<"O pelatis "<<getpid()<<"  kathise sto trapezi!"<<endl;
             break;
          }
         }
		 s_m->door_answer=0;
		 s_m->group_size=0;
		 table[table_num].serve=1;
		 sem_post(sem3);
		 sem_wait(&(table[table_num].table));
                 cout<<"O pelatis "<<getpid()<<"  serviristike!"<<endl;
	}
	else
	{
		if (want_bar==1)
                 {
                   cout<<"O Pelaths  "<<getpid()<<" thelei na kathisei sto  bar!"<<endl;
                    s_m->door_answer=0;
                    s_m->group_size=0;
                    sem_post(sem3);
	            if (s_m->max_bar_space >= s_m->bar_counter + group_number)   ///ama to group xwraei sto bar
	             {
                      ts.tv_nsec += MAXTIME;
                      ts.tv_sec += ts.tv_nsec / 1000000000;
                      ts.tv_nsec %= 1000000000;

                    if (group_number==1 || group_number==2)
                     {
                       sem_wait(sem9);
                       s_m->bar2++;
                       s_m->bar_counter += group_number;
                       sem_post(sem9);
                       err = sem_timedwait(sem5,&ts);
                       if (err == -1 )
                       {
                        cout<<"O Pelaths "<<getpid()<<" varethike na kathetai sto bar_2 kai feygei!"<<endl;

                        sem_wait(sem9);
                        s_m->bar2--;
                        s_m->bar_counter -= group_number;
                        s->groups_left_after_bar++;
                        sem_post(sem9);
                        ex=1;
                       }
                              sem_wait(sem9);
                              num = s_m->table_answer;
                              s_m->bar2--;
                              s_m->bar_counter -= group_number;
                              table[s_m->table_answer].status=1;
                              table[s_m->table_answer].serve=1;
                              cout<<"O pelaths "<<getpid()<<" metakinhthhke apo to bar sto trapezi "<< s_m->table_answer <<endl;
                              sem_wait(sem9);
                              sem_wait(&(table[num].table));
                     }
                    else if (group_number==3 || group_number==4)
                     {
                       sem_wait(sem9);
                       s_m->bar4++;
                       s_m->bar_counter += group_number;
                       sem_post(sem9);
                       err = sem_timedwait(sem6,&ts);
                       if (err == -1 )
                        {
			   cout<<"O Pelaths "<<getpid()<<" varethike na kathetai sto bar_4 kai feygei!"<<endl; 
	                   sem_wait(sem9);
                           s_m->bar4--;
                           s_m->bar_counter -= group_number;
                           s->groups_left_after_bar++;
                           sem_post(sem9);
		           ex=1;
                        }

			      sem_wait(sem9);
                              num = s_m->table_answer;
                              s_m->bar4--;
                              s_m->bar_counter -= group_number;
                              table[s_m->table_answer].status=1;
                              table[s_m->table_answer].serve=1;
                              cout<<"O pelaths "<<getpid()<<" metakinhthhke apo to bar sto trapezi "<< s_m->table_answer <<endl;
                              sem_wait(sem9);
                              sem_wait(&(table[num].table));
                     }
                    else if (group_number==5 || group_number==6)
                     {
                       sem_wait(sem9);
                       s_m->bar6++;
                       s_m->bar_counter += group_number;
                       sem_post(sem9);
                       err = sem_timedwait(sem7,&ts);
                       if (err == -1 )
                       {
                        cout<<"O Pelaths "<<getpid()<<" varethike na kathetai sto bar_6 kai feygei!"<<endl;
                        sem_wait(sem9);
                        s_m->bar6--;
                        s_m->bar_counter -= group_number;
                        s->groups_left_after_bar++;
                        sem_post(sem9);
                        ex=1;
                        }
		          sem_wait(sem9);
                          num = s_m->table_answer;
                          s_m->bar6--;
                          s_m->bar_counter -= group_number;
                          table[s_m->table_answer].status=1;
                          table[s_m->table_answer].serve=1;
                          cout<<"O pelaths "<<getpid()<<" metakinhthhke apo to bar_6 sto trapezi "<< s_m->table_answer <<endl;
                          sem_wait(sem9);
                          sem_wait(&(table[num].table));
	               }
                    else
                     {
                       sem_wait(sem9);
                       s_m->bar8++;
                       s_m->bar_counter += group_number;
		       sem_post(sem9);
                       err = sem_timedwait(sem8,&ts);
                       if (err == -1 )
                       {
		       cout<<"O Pelaths "<<getpid()<<" varethike na kathetai sto bar_8 kai feygei!"<<endl;
		       sem_wait(sem9);
                       s_m->bar8--;
                       s_m->bar_counter -= group_number;
                       s->groups_left_after_bar++;
                       sem_post(sem9);
		       ex=1;
                       }
                              sem_wait(sem9);
                              num = s_m->table_answer;
                              s_m->bar8--;
                              s_m->bar_counter -= group_number;
                              table[s_m->table_answer].status=1;
                              table[s_m->table_answer].serve=1;
                              cout<<"O pelaths "<<getpid()<<" metakinhthhke apo to bar_8 sto trapezi "<< s_m->table_answer <<endl;
                              sem_wait(sem9);
                              sem_wait(&(table[num].table));
		     }
                }
		else { cout<<"Den yparxei xwros oute sto bar ara o pelatis "<<getpid()<<" feygei"<<endl; s->groups_left++; ex=1; }
        }
        else { cout<<"Den yparxei xwros sto estiatorio kai o pelaths "<<getpid()<<" feygei"<<endl;  sem_post(sem3); s->groups_left++; ex=1; }
      }
       if (ex==0)
        {
          sleep(rand()%time+1);
	  sem_wait(&(table[table_num].busy));
	  table[table_num].pay=1;
          cout<<"O pelatis "<<getpid()<<"  thelei na plhrwsei!"<<endl;
	  sem_post(&(table[table_num].busy));
	  sem_wait(&(table[table_num].table));
          cout<<"O pelatis "<<getpid()<<"  plhrwse kai feygei apo to estiatorio!"<<endl;
        }

   shmdt (s_m);
   shmdt(s);

   sem_close (sem1);
   sem_close(sem2);
   sem_close(sem3);
   sem_close(sem4);
   sem_close(sem5);
   sem_close(sem6);
   sem_close(sem7);
   sem_close(sem8);
   sem_close(sem9);

   exit(1);
}
