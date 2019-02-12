#include "List.h"

void List::insert_list(mode_t perm,uid_t us, gid_t gr,off_t siz, time_t tim, char* nam, char typ, int id ,int blk, int lvl){

	if (header==NULL) { header=new dinode(perm,us,gr,siz,tim,nam,typ,id,blk,lvl); }
	else
	{
		dinode* temp = header;
		while(temp->next!=NULL) temp =temp->next;
		temp->next = new dinode(perm,us,gr,siz,tim,nam,typ,id,blk,lvl);
	}

}

List::~List()
{
    dinode* l = header;
    dinode* tmp = header;
    while (tmp!=NULL)
    {
      tmp=l->next;
      delete l;
      l = tmp;
    }
}

void List::print_list()
{
	dinode* tmp = header;
    while (tmp!=NULL)
    {
      tmp->print();
	  tmp = tmp->next;
    }
}

int List::find_position(int id)             //gia dedomeno id node vriskei th thesh tou sth lista 
{
	int pos=1;
	int found = 0;

    dinode* tmp = header;
	while (tmp != NULL){
		if(tmp->data.node_id == id){
			found = 1;
			break;
		}
		tmp = tmp->next;
		pos++;
    }
	if(found)
		return pos-1;
	else
		return -1;

}

dinode* List::return_node(int position)     //epistrefei ton komvo ths listas pou vrisketai sth thesh position
{                                          // position 3ekinaei apo 0
	int pos = 0;
	dinode* tmp = header;
	while (position!=pos){
		tmp = tmp->next;
		pos++;
	}
	return tmp;
}

