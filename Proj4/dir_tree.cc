#include "List.h"
#include "dir_funcs.h"


int dir_num;                                                //arithmos dirs tou arxeiou
int total_num;                                              //synolikos arithmos dird kai files
int count;                                                  //metavlhth xrhsimh g thn eisagwgh stis ypolistes
int write_block;                                            //arxiko block pou tha graftei h plhroforia enos file

int main(int argc,char* argv[])

{
	dir_num=0;
	total_num=0;
	count=0;
	write_block =1;

	char* name = new char[strlen(argv[1])+1];
	strcpy(name,argv[1]);

	char* name2 = new char[strlen(argv[2])+1];
        strcpy(name2,argv[2]);


	List *directory_list;
	directory_list = new List();                               //lista me inodes


	cout<<"--->Directory tree print!!"<<endl;

        if (is_regular_file(name))
	{
	int blocks;

	//blocks = writeFile(name, /*target file_Dsc*/, write_block);

	if (strchr(name,'/')==NULL)	fix_list(name,directory_list,name,write_block);
	else fix_list(name,directory_list,filename_return(name),write_block);
							                                //prosthetei to file sthn inode list
        write_block += blocks;                                                         //ananewnei to write block gia na graftei allo arxeio
        total_num++;
	}
	else
        {

	if (strchr(name,'/')==NULL)     fix_list(name,directory_list,name,-1);
        else fix_list(name,directory_list,filename_return(name),-1);

	dir_num++;
	total_num++;
	listdir(name,0,directory_list,/*target file_Dsc*/0);
	}
	cout<<"Dir number: "<<dir_num<<" Total elements: "<<total_num<<endl;

	cout<<"--->Directory inode list!!"<<endl;
	directory_list->print_list();

	cout<<"-------------------------------------------------"<<endl;

	///////////
	dirListNode** table;                                                           //ypolistes periexomenwn kathe dir
	table = new dirListNode*[dir_num];
	for(int j=0; j<dir_num; j++) table[j]=NULL;
	fix_level(name,directory_list,table);
	fix_position(name,directory_list,table);

	cout<<"----->Level lists print"<<endl;
	for(int i=0; i<dir_num; i++)                                           //ektypwsh theshs periexomenwn dir sthn inode list
	{
		dirListNode* tmp = table[i];
		cout<<"Elements: "<<list_numberof_elements(table,i)<<"------";
		if (tmp==NULL) cout<<"DIR has not list"<<endl;
		else
		{
			while(tmp!=NULL)
				{
					tmp->print();
					tmp=tmp->next;
				}
				cout<<endl;
		}
	}

	cout<<"----------------------------------------"<<endl;

	cout<<"------>Elements Print:"<<endl;
	dir_print(directory_list,table);

	cout<<"----------------------------------------"<<endl;

	if(search(name2,directory_list,table)==1) cout<<"Found!!"<<endl;
	else cout<<"Not Found!!"<<endl;

    //Delete structures
    delete directory_list;                                                  //diagrafh inode list

     for(int i=0; i<dir_num; i++)                                           //diagrafh twn ypolistwn 
	 {
		dirListNode* l = table[i];
		dirListNode* tmp = table[i];
		while(tmp!=NULL)
		{
			tmp=l->next;
			delete l;
			l=tmp;
		}
	 }

    return 0;
}






