
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <stdio.h>

using namespace std;

class List;

struct record {
     record* next;
     record* prev;
     int student_id;
     char *name;
     char *surname;
     float gpa;
     int number_of_courses;
     char *department;
     int post_number;

     record(int s_id,char *nam,char *sunam,float g,int noc,char *dep,int pn) : student_id(s_id),gpa(g),number_of_courses(noc),post_number(pn) {
        next=NULL;
        prev=NULL;
        name = new char[strlen(nam) + 1];
        surname = new char[strlen(sunam) + 1];
        department = new char[strlen(dep) + 1];
        strcpy(name,nam);
        strcpy(surname,sunam);
        strcpy(department,dep);
}

     ~record() { delete [] name; delete [] surname; delete [] department; }

     void modify(float g, int noc) { gpa=g; number_of_courses=noc; print(); }

     void print() {
          cout<<student_id<<" ";
          cout<<name<<" ";
          cout<<surname<<" ";
          cout<<number_of_courses<<" ";
          cout<<gpa<<" ";
          cout<<department<<" ";
          cout<<post_number<<" ";
          cout<<endl; }
};


struct list_node {
     int key;
     record* rec;
     list_node** forward;
     list_node(int level,int k,record* reco) { rec=reco; key=k; forward = new list_node*[level]; for(int i=0;i<level;i++) forward[i]=NULL; }
     ~list_node() { delete [] forward; delete rec;  }
 };

struct l_node {
     l_node* next;
     record* rec;
     int num;
     l_node(record* r) {num=1; next=NULL; rec=r; }
};
class hashTable {
     private:
      int size;
      int total_records;
      record** table;
     public:
      hashTable(int siz) { total_records=0; size=siz;  table = new record*[size]; for(int i=0; i<size; i++) table[i]=NULL; }
      ~hashTable() { delete [] table; }
      int hashFun(int key) {  return (key%size); }
      void insert(record* rec, int key);
      void print(int key);
      float average(int key);
      void print_k_first(int,int);
      void courses_to_take(int,char*);
      void percentile(int);
      record*& return_header(int position) { return table[position]; }
};

class Skip_list {
    private:
     hashTable *table;
     int max_level;
     int max_key;
     int list_size;
     int max_courses;
     list_node* header;
     list_node* term_node;
     void insert(int key,list_node *l1,record* rec);
     void delet(list_node *l,int k,List*,List*);
     record* search(int key,list_node *l);
     void destroy_list(list_node *l);
     void print(list_node *l);
     float average(int id1,int id2,list_node* l);
   public:
     Skip_list(int m_l,int m_k, hashTable* ht);
     ~Skip_list() { destroy_list(header); }
     void insert(int key,record *rec) { insert(key,header,rec); }
     void delet(int key,List *list1, List *list2) { delet(header,key,list1,list2); }
     record* search(int key) { return search(key,header); }
     int randomLevel() { return rand()%max_level+1; }
     list_node* makeNode(int lvl,int key,record* rec) { return new list_node(lvl,key,rec); }
     void print() { print(header); }
     float average(int id1,int id2) { return average(id1,id2,header); }
     void find(float);
  };

class List {
     private:
      l_node *header;
      int size;
      void print_percentile(l_node*);
     public:
      List() { header=NULL; size=0; }
      ~List();
      void print_size() { cout<<size<<endl; }
      void insert1(record*);
      void insert2(record*);
      void print_percentile() { print_percentile(header); }
      void print_bottom(int k);
      void delet(int);
      void modify(int);
};

