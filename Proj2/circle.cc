#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

using namespace std;

int main(int argc , char *argv[]) {

char* InputFile;
char* OutputFile;
int  Offset=0;
int  Points=0;
float x;
float y;
float r;
int i=1;
FILE *fp1;
int fd;
float f,f1,f2;
int bytes_to_read;
int count=0;
int check;

if (argc==1) { cout<<"You have to give parametres!!"<<endl; return 0; }
 else if ( argc==9 || argc==11 || argc==13  )
   {

           if (strcmp(argv[i],"-a")==0)  { x=atof(argv[i+1]);  y=atof(argv[i+2]);  r=atof(argv[i+3]);  i=i+4;  }
           else
         {
           if (strcmp(argv[i],"-i")==0 )  { InputFile = new char[strlen(argv[i+1])+1]; strcpy(InputFile,argv[i+1]);  }
           if (strcmp(argv[i],"-o")==0 )  { OutputFile = new char[strlen(argv[i+1])+1]; strcpy(OutputFile,argv[i+1]); }
           if (strcmp(argv[i],"-f")==0 )  { Offset = atoi(argv[i+1]);  }
           if (strcmp(argv[i],"-n")==0 )  { Points = atoi(argv[i+1]);  }
           i=i+2;;
         }


    if (strcmp(argv[i],"-a")==0)  { x=atof(argv[i+1]);  y=atof(argv[i+2]);  r=atof(argv[i+3]);   i=i+4;  }
    else {
           if (strcmp(argv[i],"-i")==0 )  { InputFile = new char[strlen(argv[i+1])+1]; strcpy(InputFile,argv[i+1]);   }
           if (strcmp(argv[i],"-o")==0 )  { OutputFile = new char[strlen(argv[i+1])+1]; strcpy(OutputFile,argv[i+1]); }
           if (strcmp(argv[i],"-f")==0 )  { Offset = atoi(argv[i+1]);  }
           if (strcmp(argv[i],"-n")==0 )  { Points = atoi(argv[i+1]);  }
           i=i+2;
         }

    if (strcmp(argv[i],"-a")==0)  { x=atof(argv[i+1]);  y=atof(argv[i+2]);  r=atof(argv[i+3]);  i=i+4; }
    else  {
           if (strcmp(argv[i],"-i")==0)  { InputFile = new char[strlen(argv[i+1])+1]; strcpy(InputFile,argv[i+1]);   }
           if (strcmp(argv[i],"-o")==0)  { OutputFile = new char[strlen(argv[i+1])+1]; strcpy(OutputFile,argv[i+1]);  }
           if (strcmp(argv[i],"-f")==0)  { Offset = atoi(argv[i+1]);  }
           if (strcmp(argv[i],"-n")==0)  { Points = atoi(argv[i+1]);  }
           i=i+2;
          }

     if (argc==11 || argc==13)
      {
        if (strcmp(argv[i],"-a")==0)  { x=atof(argv[i+1]);  y=atof(argv[i+2]);  r=atof(argv[i+3]); i=i+4;  }
        else {
              if (strcmp(argv[i],"-i")==0 )  { InputFile = new char[strlen(argv[i+1])+1]; strcpy(InputFile,argv[i+1]);  }
              if (strcmp(argv[i],"-o")==0 )  { OutputFile = new char[strlen(argv[i+1])+1]; strcpy(OutputFile,argv[i+1]);  }
              if (strcmp(argv[i],"-f")==0 )  { Offset = atoi(argv[i+1]); }
              if (strcmp(argv[i],"-n")==0 )  { Points = atoi(argv[i+1]);  }
              i=i+2;
             }
     if (argc==13)
      {
       if (strcmp(argv[i],"-i")==0)  { InputFile = new char[strlen(argv[i+1])+1]; strcpy(InputFile,argv[i+1]); }
       if (strcmp(argv[i],"-o")==0)  { OutputFile = new char[strlen(argv[i+1])+1]; strcpy(OutputFile,argv[i+1]);  }
       if (strcmp(argv[i],"-a")==0)  { x=atof(argv[i+1]);  y=atof(argv[i+2]);  r=atof(argv[i+3]); }
       if (strcmp(argv[i],"-f")==0)  { Offset = atoi(argv[i+1]); }
       if (strcmp(argv[i],"-n")==0)  { Points = atoi(argv[i+1]); }
      }
     }
   }
   else cout<<"Incorrect Parametres!!"<<endl;

   cout<<"Searching for points in a circle "<<x<<" "<<y<<" "<<r<<" Output file: "<<OutputFile<<endl;

   if ((fp1=fopen(InputFile,"rb"))==NULL)
   { cout<<"Cannot open the input  file!!"<<endl;  return 0; }

   fseek(fp1,0,SEEK_END);
   long size = ftell(fp1);
   fseek(fp1,0, SEEK_SET);
   fseek(fp1,Offset, SEEK_SET);

   fd=open(OutputFile,O_WRONLY | O_NONBLOCK);

   check=1;
   if (Points!=0)
    bytes_to_read = Points*2*sizeof(float);
   else
   {
    if (Offset==0) bytes_to_read = size;
    else bytes_to_read = size-Offset;
   }

   while (count!=bytes_to_read)
   {
    fread(&f,1, sizeof(float),fp1);
    if (check==1) { f1=f; check++; }
    else if (check==2) { f2=f; check=1; if ((x-f1)*(x-f1) + (y-f2)*(y-f2) <= r*r) { cout<<f1<<" "<<f2<<endl; write(fd,&f1,sizeof(float));
    write(fd,&f2,sizeof(float)); }   }
    count+=sizeof(float);
   }

   fclose(fp1);
   //close(fd);

   delete InputFile;
   delete OutputFile;

  return 0;

}
