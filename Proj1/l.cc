#include "header.h"

void List::insert1(record* r) {
     if (header==NULL) { header = new l_node(r); size++; }
     else
      {
       l_node *tmp = header;
       while(tmp->next!=NULL && tmp->rec->post_number != r->post_number)
       tmp = tmp->next;
       if (tmp->next==NULL &&  tmp->rec->post_number != r->post_number)
         tmp->next = new l_node(r);
       else
         tmp->num++;
       size++;
      }
}

void List::insert2(record* r) {
     if (header==NULL) { header= new l_node(r);  size++; }
     else
      {
        l_node* new_node;
        l_node* tmp = header->next;
        l_node* prev = header;
        if (r->gpa <= header->rec->gpa )
         {
           header  = new l_node(r);
           header->next = prev;
         }
        else if (tmp==NULL)
         {
           if (r->gpa >= header->rec->gpa)
           prev->next = new l_node(r);
           else
           {
            header = new l_node(r);
            header->next=prev;
           }
         }
        else
         {
          while(tmp!=NULL && tmp->rec->gpa <= r->gpa)
          { prev=tmp;
          tmp = tmp->next;
          if (tmp==NULL) break; }

          new_node = new l_node(r);
          new_node->next = tmp;
          prev->next = new_node;
         }
        size++;
       }
}

void List::print_bottom(int k) {
     l_node* l = header;
     int num=0;
     while(l!=NULL && num<k)
      {
        if (l->rec!=NULL)
         {
          l->rec->print();
          num++;
         }
        l=l->next;
      }
}

void List::print_percentile(l_node* l) {
       if (l==NULL) cout<<"Empty list"<<endl;
        else
     {
        if (l->next!=NULL)
         {
           cout<< l->rec->post_number<<" - "<<" ";
           printf("%.2f\n",(float)(l->num)/size);
           print_percentile(l->next);
         }
        else
         {
           cout<< l->rec->post_number<<" - "<<" ";
           printf("%.2f\n",(float)(l->num)/size);
           cout<<"Size of list: "<<size<<endl;
           return;
         }
     }
}


  List::~List() {
    l_node* l = header;
    l_node* tmp = header;
    while (tmp!=NULL)
    {
      tmp=l->next;
      delete l;
      l = tmp;
    }
}

void List::delet( int key ) {
   l_node* l = header;
   l_node* tmp = header;
    while (tmp->rec->student_id!=key)
    {
      l=tmp;
      tmp=tmp->next;
    }
    if (l==tmp)
    header=l->next;
    else
    l->next=tmp->next;
    delete tmp;
}

void List::modify(int post_code) {
   l_node* l = header;
   l_node* tmp = header;
    while (tmp->rec->post_number!=post_code)
    {
      l=tmp;
      tmp=tmp->next;
    }
    if ( tmp->num == 1 )
     {
       if (l==tmp)
        header=l->next;
       else
        l->next=tmp->next;
        delete tmp;
     }
   else tmp->num = tmp->num - 1;

}
