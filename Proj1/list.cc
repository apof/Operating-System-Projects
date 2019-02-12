#include "header.h"

Skip_list::Skip_list(int m_l,int m_k,hashTable* ht) {
     table=ht;
     max_level=m_l;
     max_key=m_k;
     max_courses=0;
     header = new list_node(max_level,-1,NULL);
     term_node = new list_node(max_level,max_key,NULL);
     for(int i=0; i<max_level; i++)
     header->forward[i]=term_node;
}

void Skip_list::print(list_node* l) {
     if (list_size==0)
      cout<<"List is empty!!"<<endl;
     else
      {
       if (l->forward[0]!=NULL)
        {
          if(l!=header)
           {
            l->rec->print();
           }
            print(l->forward[0]);
        }

       else
        {
          cout<<"End of list!!"<<endl;
          cout<<endl;
          return;
        }
      }
}

void Skip_list::destroy_list(list_node *l) {
     if (l->forward[0]!=NULL)
      {
       list_node* tmp;
       tmp=l->forward[0];
       delete l;
       destroy_list(tmp);
      }
     else
      {
       delete l;
      }
}


void Skip_list::insert(int key,list_node* l,record* reco) {
       list_node** update;
       update = new list_node*[max_level];
       for(int i=0; i<max_level; i++)
       update[i]=NULL;
       for(int i=max_level-1; i>=0; i--)
        {
         while (l->forward[i]->key < key)
          l=l->forward[i];
         update[i]=l;
        }
       l=l->forward[0];
       if (l->key == key)
       l->rec=reco;
       else
       {
        int lvl=randomLevel();
        l=makeNode(lvl,key,reco);
        list_size++;
        for(int i=0; i<lvl; i++)
         {
          l->forward[i]=update[i]->forward[i];
          update[i]->forward[i]=l;
         }
       }
    if (reco->number_of_courses > max_courses)
    max_courses = reco->number_of_courses;
    delete [] update;
}

void Skip_list::delet(list_node* l,int key,List *list,List *list2)  {
       list_node** update;
       update = new list_node*[max_level];
       for(int i=0; i<max_level; i++)
       update[i]=NULL;
       for(int i=max_level-1; i>=0; i--)
       {
        while (l->forward[i]->key < key)
         l=l->forward[i];
       update[i]=l;
       }
       l=l->forward[0];
       if(l->key==key)
        {
         for(int i=0; i<max_level; i++)
          {
           if (update[i]->forward[i]!=l)
           break;
           update[i]->forward[i]=l->forward[i];
          }
         l->rec->print();
         if (l->rec->prev==NULL) { int position = table->hashFun(l->rec->post_number); table->return_header(position)=l->rec->next; }
         else if(l->rec->next!=NULL) { l->rec->prev->next=l->rec->next; l->rec->next->prev = l->rec->prev; }
         else { l->rec->prev->next=l->rec->next; }
         list2->modify(l->rec->post_number);
         delete l;
         list->delet(key);
       }
       else cerr<<"Not found"<<endl;

     delete [] update;
}


record* Skip_list::search(int key,list_node *l) {
      for(int i=max_level-1; i>=0; i--)
       while (l->forward[i]->key < key)
       l=l->forward[i];
      l=l->forward[0];
      if(l->key==key)
       return (l->rec);
      else
       return NULL;
}


float Skip_list::average(int id1,int id2,list_node* l) {
     int max,min;

      if(id1>id2)
      {max=id1; min=id2; }
      else if  (id1<id2) { max=id2; min=id1; }
      else min=id1;

       for(int i=max_level-1; i>=0; i--)
       while (l->forward[i]->key < min)
        l=l->forward[i];
        l=l->forward[0];

      float sum=0.0;
      float num=0.0;

        while (l->key<=max)
         {
           sum+=l->rec->gpa;
           num++;
           l=l->forward[0];
         }
           return (sum/num);

}
void Skip_list::find(float g) {
   list_node* l;
   l=header->forward[0];
   int flag=0;
   while (l!=term_node)
    {
     if (l->rec->gpa > g && l->rec->number_of_courses == max_courses)
     { l->rec->print(); flag=1; }
     l=l->forward[0];
     if (l==term_node && flag==0) cerr<<"Not found"<<endl;
    }
}
