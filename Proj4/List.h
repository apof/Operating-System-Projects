#ifndef LIST
#define LIST

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>


using namespace std;

struct dirListNode                                      //komvos listas periexomenwn enos directory 
{
	dirListNode *next;
	int position;                                  //fylame th thesh pou exei to periexomeno file h directory sthn inode list
	dirListNode(int pos) { position = pos; next=NULL; }
	void print(){ cout<<position<<" "; }
};

struct metadata
{
	mode_t perms;                                   //apothikeyoume kai xrhsimopoioume to node id tou systhmatos
	off_t size;                                         //gia na 3exwrizoume kai na katagrafoume swsta ta level twn dirs
	time_t time;                                   //giati yparxoun polla files kai dirs me idio onoma!!
	uid_t 	user;
	gid_t 	group;
	char name[20];
	char type;
	int node_id;
	int block;
	int level;

	metadata(mode_t perm,uid_t us, gid_t gr,off_t siz, time_t tim, char* nam, char typ, int id,int blk,int lvl){
		perms = perm;
		time = tim;
		strcpy(name,nam);
		size = siz;
		type = typ;
		node_id = id;
		user = us;
		group = gr;
		block=blk;
		level=lvl;
	}

	metadata(){}

	void print_permissions()
	{
        cout<<((perms & S_IRUSR) ? "r" : "-")<<"";
        cout<<((perms & S_IWUSR) ? "w" : "-")<<"";
        cout<<((perms & S_IXUSR) ? "x" : "-")<<"";
        cout<<((perms & S_IRGRP) ? "r" : "-")<<"";
        cout<<((perms & S_IWGRP) ? "w" : "-")<<"";
        cout<<((perms & S_IXGRP) ? "x" : "-")<<"";
        cout<<((perms & S_IROTH) ? "r" : "-")<<"";
        cout<<((perms & S_IWOTH) ? "w" : "-")<<"";
        cout<<((perms & S_IXOTH) ? "x" : "-")<<"";
	}

	void print_time()
	{
	struct tm lt;
	char str_modify[100];

	localtime_r(&time,&lt);
	strftime(str_modify, sizeof(str_modify), "%c", &lt);
	cout << str_modify <<" ";
	}


	void print_userName()
	{
    	struct passwd *pwd;
    	pwd = getpwuid(user);
    	cout<< pwd->pw_name << " ";
	}

	void print_groupName()
	{
    	struct group *grp;
    	grp = getgrgid(group);
    	cout << grp->gr_name <<" ";
	}


	void meta_print()
		{
			cout<<type<<"";
			print_permissions();
			cout << " ";
			print_userName();
			print_groupName();
 			printf(" %ld \t", size);
			print_time();
			cout<< name << endl;
		}
};

struct dinode
{
	int level;
	dinode *next;
	int type;                                          //typos arxeiou 0 gia directory 1 gia file
	metadata data;                                     //metadata
	dinode(mode_t perm,uid_t us, gid_t gr,off_t siz, time_t tim, char* nam, char typ, int id,int blk,int lvl)
	:data(perm,us,gr,siz,tim,nam,typ,id,blk,lvl)
		{
			if (typ=='d') type=0;
			else if (typ=='-') type=1;
			next=NULL;
			level=lvl;
		}
	void print() {data.meta_print();}
	void set_block(int blk) { data.block = blk;}
};

class List                                             //lista apo inodes
{
	private:
		dinode* header;
	public:
		List() { header=NULL; }
		~List();
		void insert_list(mode_t,uid_t,gid_t,off_t, time_t, char*,char,int,int,int);
		int find_position(int);
		void print_list();
		dinode* return_node(int);
		dinode* return_header() { return header; }
};

#endif
