#include "header.h"

void hashTable::insert(record* rec, int key) {
      int position = hashFun(key);
      if (table[position]==NULL)
        { table[position]=rec; total_records++; }
      else
       {
        record *tmp,*prev;
        tmp = table[position]->next;
        prev=table[position];
        if (rec->gpa >= prev->gpa)
         {
          table[position]=rec;
          rec->next=prev;
          rec->prev =prev->prev;
         }
        else if (tmp==NULL)
        {
          if (prev->gpa >= rec->gpa)
          { prev->next = rec;
            rec->prev=prev; }
          else
          {
           table[position]=rec;
           table[position]->next=prev;
           rec->prev = prev->prev;
          }
        }
        else
        {
         while(rec->gpa <= tmp->gpa && tmp!=NULL )
         {
          prev=tmp;
          tmp = tmp->next;
          if (tmp==NULL) break;
         }
        rec->next=tmp;
        prev->next=rec;
        rec->prev=prev;
        if (tmp!=NULL) tmp->prev=rec;
        }
        total_records++;
      }
}

void hashTable::print(int key) {
      int position = hashFun(key);
      record* tmp = table[position];
      if(tmp!=NULL)
        {
          while(tmp!=NULL)
           {
            if(tmp->post_number == key)
            tmp->print();
            tmp=tmp->next;
           }
        }
      else
       cout<<"No records for this post number!!"<<endl;
}

float hashTable::average(int key) {
      int position = hashFun(key);
      record* tmp = table[position];
      float gp = 0.0;
      float num = 0.0;
      if(tmp!=NULL)
        {
          while(tmp!=NULL)
           {
            if(tmp->post_number == key)
            {
              gp+=tmp->gpa;
              num++;
            }
            tmp=tmp->next;
           }
         return (gp/num);
        }
      else
       {
         cout<<"No records for this post number"<<endl;
         return 0.0;
       }
}

void hashTable::print_k_first(int key,int k) {
      int position = hashFun(key);
      record *tmp = table[position];
      if (tmp==NULL) { cerr<<"Not found!!"<<endl; return; }
      int num = 0;
      if (k==0)
       cout<<"K is 0!!"<<endl;
      else
       {
         while (tmp!=NULL && num<k)
          {
            if(tmp->post_number==key)
            {
             tmp->print();
             num++;
            }
            tmp=tmp->next;
          }
       }
}


void hashTable::courses_to_take(int key,char *dep) {
      int position = hashFun(key);
      record *tmp = table[position];
      if (tmp==NULL) { cerr<<"Not found!!"<<endl; return; }
      int num = 0;
      while (tmp!=NULL)
       {
         if ( strcmp(tmp->department,dep)==0 && tmp->post_number==key)
          {
            tmp->print();
            num += tmp->number_of_courses;
          }
         tmp=tmp->next;
       }
      if (num==0) cerr<<"Not found"<<endl; else
      cout<<"Total number of courses to take: "<<num<<endl;
}


void hashTable::percentile(int key) {
      int position = hashFun(key);
      record* tmp = table[position];
      if (tmp==NULL) { cerr<<"Not found!!"<<endl; return; }
      int num=0;
      while(tmp!=NULL)
       {
        if (tmp->post_number==key)
        num++;
        tmp=tmp->next;
       }
      printf("%.2f\n",(float)num/total_records);
}


