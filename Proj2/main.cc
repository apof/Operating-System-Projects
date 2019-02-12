#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>

using namespace std;

int main (int argc , char* argv[] ) {

 FILE *final_out;

 int command_count=0;
 char comm_c[3];
 char gnu_script[20];
 char scriptpath[120];
 char def_script[20];
 strcpy(def_script,"_script.gnuplot");
 char img[20];
 strcpy(img," _image.png");

 FILE *outputfile;
 char handler_file[10];
 char outpath[120];

 float result1;
 float result2;

 DIR* di;
 struct dirent *dir;

 int i,status;
 pid_t pids[15];
 pid_t wpids[20];
 pid_t pid,Id;

 char fifofile[18];
 char handler_no[7];
 char worker_no[3];
 char fifopath[120];
 char **fifoArray;
 int *fdArray;

 int workers;
 char* InputFile;
 char* Directory;
 char buffer[100];

 long size;
 int *workers_offset;
 int points;
 int points_per_worker;
 char points_per_work[10];
 int offset=0;
 char offs[10];
 FILE *fp;

 char* shape=NULL;
 char* a=NULL;
 char* b=NULL;
 char* c=NULL;
 char* d=NULL;
 char *place=NULL;
 char *colour=NULL;
 char *pch=NULL;

 if (argc!=7) { cout<<"You have to give 3 arguments!!"<<endl; return 0; }
 else {
         if (strcmp(argv[1],"-i")==0) { InputFile = new char[strlen(argv[2]) + 1]; strcpy(InputFile,argv[2]); }
         if (strcmp(argv[1],"-d")==0) { Directory = new char[strlen(argv[2]) + 1]; strcpy(Directory,argv[2]); }
         if (strcmp(argv[1],"-w")==0) { workers = atoi(argv[2]); }

         if (strcmp(argv[3],"-i")==0) { InputFile = new char[strlen(argv[4]) + 1]; strcpy(InputFile,argv[4]); }
         if (strcmp(argv[3],"-d")==0) { Directory = new char[strlen(argv[4]) + 1]; strcpy(Directory,argv[4]); }
         if (strcmp(argv[3],"-w")==0) { workers = atoi(argv[4]); }

          if (strcmp(argv[5],"-i")==0) { InputFile = new char[strlen(argv[6]) + 1]; strcpy(InputFile,argv[6]); }
          if (strcmp(argv[5],"-d")==0) { Directory = new char[strlen(argv[6]) + 1]; strcpy(Directory,argv[6]); }
          if (strcmp(argv[5],"-w")==0) { workers = atoi(argv[6]); }
        }

 cout<<"Workers: "<<workers<<endl;
 cout<<"InputFile: "<<InputFile<<endl;
 cout<<"Directory: "<<Directory<<endl;

 fifoArray = new char*[workers];
 fdArray = new int[workers];

 struct stat st = {0};

 if (stat(Directory, &st) == -1) {
    mkdir(Directory, 0700);
  }

 if ((fp=fopen(InputFile,"rb"))==NULL)
 { cout<<"Cannot open input file!"<<endl; return 0; }

 fseek(fp,0,SEEK_END);
 size = ftell(fp);
 fclose(fp);
 cout<<"Size of file: "<<size<<endl;

 workers_offset = new int[workers];
 points = size/(2*sizeof(float));
 points_per_worker = points/workers;
 cout<<"Workers "<<1<<"-"<<workers-1<<" have "<<points_per_worker<<" points"<<" and the "<<workers<<"rd has "<<points-(points_per_worker*(workers-1))<<" points" <<endl;
 for (int j=0; j<workers; j++)
 {
    workers_offset[j] = offset;
    offset+=(points_per_worker*2*sizeof(float));
 }
 for (int k=0; k<workers; k++)
 cout<<"Worker: "<<k+1<<" with offset: "<<workers_offset[k]<<endl;




 while ( strcmp(buffer,"exit")!=0 ) {

  i=0;
  cin.getline(buffer,sizeof(buffer));
  if (strcmp(buffer,"exit")!=0)
  {
   command_count++;
   sprintf(comm_c,"%d",command_count);
   cout<<comm_c<<endl;
   int co=0;
   int v=0;
   while (comm_c[co]!='\0') {gnu_script[v]=comm_c[co]; v++; co++; }
   co=0; while(def_script[co]!='\0') { gnu_script[v]=def_script[co]; co++; v++; } gnu_script[v]='\0';
   strcpy(scriptpath,Directory); v=0; co=0; while(scriptpath[co]!='\0') co++; scriptpath[co]='/'; co++; while(gnu_script[v]!='\0') {scriptpath[co]=gnu_script[v];
   co++; v++;}
   scriptpath[co]='\0';
   cout<<"Command output file :"<<scriptpath<<endl;
   img[0]=comm_c[0];
   cout<<img<<endl;
   final_out = fopen(scriptpath,"a");
   fprintf(final_out,"%s\n%s\n","set terminal png","set size ratio -1");
   fprintf(final_out,"set output \"./%s\"\n",img);
   fprintf(final_out,"plot ");
   fclose(final_out);

   pch = strtok (buffer," ");

    while (pch != NULL)
     {
      shape = new char[strlen(pch) + 1];
      strcpy(shape,pch);

      if (strcmp(shape,"circle")==0) {
       a=strtok(NULL," ");
       b=strtok(NULL," ");
       c=strtok(NULL," ");
       colour=strtok(NULL," ,;");
       cout<<shape<<" "<<a<<" "<<b<<" "<<c<<" "<<colour<<endl;  }

      if (strcmp(shape,"semicircle")==0) {
       a=strtok(NULL," ");
       b=strtok(NULL," ");
       c=strtok(NULL," ");
       place=strtok(NULL," ");
       colour=strtok(NULL," ,;");
       cout<<shape<<" "<<a<<" "<<b<<" "<<c<<" "<<place<<" "<<colour<<endl;  }

       if (strcmp(shape,"square")==0) {
       a=strtok(NULL," ");
       b=strtok(NULL," ");
       c=strtok(NULL," ");
       colour=strtok(NULL," ,;");
       cout<<shape<<" "<<a<<" "<<b<<" "<<c<<" "<<colour<<endl;  }

        if (strcmp(shape,"ring")==0) {
       a=strtok(NULL," ");
       b=strtok(NULL," ");
       c=strtok(NULL," ");
       d=strtok(NULL," ");
       colour=strtok(NULL," ,;");
       cout<<shape<<" "<<a<<" "<<b<<" "<<c<<" "<<d<<" "<<colour<<endl;  }

        if (strcmp(shape,"ellipse")==0) {
       a=strtok(NULL," ");
       b=strtok(NULL," ");
       c=strtok(NULL," ");
       d=strtok(NULL," ");
       colour=strtok(NULL," ,;");
       cout<<shape<<" "<<a<<" "<<b<<" "<<c<<" "<<d<<" "<<colour<<endl;  }

       pch = strtok(NULL," ");

     if ((pids[i]=fork())<0) { cout<<"Error"<<endl;  }
     else if (pids[i]==0) {
      printf("Handler (%d): with ID %d creating a %s\n",i+1,getpid(),shape);
        Id=getpid();
	sprintf(handler_no,"%d",Id);

        for (int p=0; p<workers; p++)
	{
	 sprintf(worker_no,"%d",p);
         int k=0;
         int m=0;
         while (handler_no[k]!='\0') { fifofile[m] = handler_no[k]; k++; m++; }
         fifofile[m]='_'; m++; fifofile[m]='w'; m++;
         k=0;
         while (worker_no[k]!='\0') { fifofile[m] = worker_no[k]; k++; m++; }
         fifofile[m]='.';  m++;  fifofile[m]='f'; m++; fifofile[m]='i'; m++; fifofile[m]='f'; m++; fifofile[m]='o'; m++; fifofile[m]='\0';

         strcpy(fifopath,Directory);
         k=0; while(fifopath[k]!='\0') k++; fifopath[k]='/'; k++; m=0; while(fifofile[m]!='\0') { fifopath[k]=fifofile[m]; k++; m++; } fifopath[k]='\0';
         fifoArray[p]=new char[strlen(fifopath) + 1];
	 strcpy(fifoArray[p],fifopath);
        }

         strcpy(handler_file,handler_no);
         int l=0; while(handler_file[l]!='\0') { l++; } handler_file[l]='.'; l++;  handler_file[l]='t'; l++;  handler_file[l]='x'; l++;  handler_file[l]='t'; l++;
         handler_file[l]='\0';

         strcpy(outpath,Directory);
         int m=0; l=0; while(outpath[l]!='\0') l++; outpath[l]='/'; l++; m=0; while(handler_file[m]!='\0') { outpath[l]=handler_file[m]; l++; m++; } outpath[l]='\0';

         cout<<outpath<<endl;
         outputfile = fopen(outpath,"a");

      for (int j=0; j<workers; j++)
      {
        if ( mkfifo(fifoArray[j],0666)<0 ) cout<<"Error in fifo!!"<<endl; else cout<<"FIFO made"<<endl;
        fdArray[j] = open(fifoArray[j],O_RDONLY | O_NONBLOCK);
        cout<<fifoArray[j]<<" "<<fdArray[j]<<endl;

        if (j==workers-1) sprintf(points_per_work,"%d",0);
        else sprintf(points_per_work,"%d",points_per_worker);

        sprintf(offs,"%d",workers_offset[j]);

       if ((wpids[j]=fork())<0) { cout<<"Error"<<endl;  }
       else if (wpids[j]==0)
       {
        printf("Worker (%d): with ID %d\n",j+1,getpid());

        if (strcmp("circle",shape)==0) execl("./circle",shape,"-i",InputFile,"-o",fifoArray[j],"-a",a,b,c,"-f",offs,"-n",points_per_work,NULL);
        if (strcmp("semicircle",shape)==0) execl("./semicircle",shape,"-i",InputFile,"-o",fifoArray[j],"-a",a,b,c,place,"-f",offs,"-n",points_per_work,NULL);
        if (strcmp("ring",shape)==0) execl("./ring",shape,"-i",InputFile,"-o",fifoArray[j],"-a",a,b,c,d,"-f",offs,"-n",points_per_work,NULL);
        if (strcmp("ellipse",shape)==0) execl("./ellipse",shape,"-i",InputFile,"-o",fifoArray[j],"-a",a,b,c,d,"-f",offs,"-n",points_per_work,NULL);
        if (strcmp("square",shape)==0) execl("./square",shape,"-i",InputFile,"-o",fifoArray[j],"-a",a,b,c,"-f",offs,"-n",points_per_work,NULL);

        exit(0);
       }
       else
        {
         printf("Handler is here waiting...\n");
         pid = wait(&status);
         cout<<fifoArray[j]<<endl;
         while(read(fdArray[j],&result1,sizeof(float))>0 && read(fdArray[j],&result2,sizeof(float))>0 ) { fprintf(outputfile,"%f %f\n",result1,result2);  }
         printf("Worker with PID %ld exited with status 0x%x.\n",(long)pid,status);
        }
      }
      exit(0);
     }
     else
     {
       final_out = fopen(scriptpath,"a+");
       char target[80];
       printf("Master is here waiting...\n");
       pid = wait(&status);
       char h_no[7];
       sprintf(h_no,"%d",pids[i]);
       cout<<pids[i]<<endl;
       strcpy(target,Directory);
       int pos1=0;
       int pos2=0;
       while(target[pos1]!='\0') pos1++; target[pos1]='/'; pos1++; while(h_no[pos2]!='\0') { target[pos1]=h_no[pos2]; pos1++; pos2++; }
       target[pos1]='.'; pos1++; target[pos1]='t'; pos1++; target[pos1]='x'; pos1++; target[pos1]='t'; pos1++;target[pos1]='\0';
       fprintf(final_out,"\\\n");
       fprintf(final_out,"\"%s\" notitle with points pointsize 0.5 linecolour rgb \"%s\",",target,colour);
       fclose(final_out);
       printf("Handler with PID %ld exited with status 0x%x.\n",(long)pid,status);
     }
     i++;
   }
 }
}


 di=opendir(Directory);
 if (di)
  {
   while((dir=readdir(di))!=NULL) { cout<< dir->d_name <<endl; }
   closedir(di);
  }

 delete InputFile;
 delete Directory;

return 0;
}
