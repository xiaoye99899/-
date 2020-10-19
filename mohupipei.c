#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>


//创建守护进程
void Guard() {
  int i;
  pid_t pid = 0;
  pid = fork();
  if(pid < 0){
    perror("fork");
    exit(-1);
  }
  //停止父进程
  if(pid >0){
    exit(0);
  }
  pid = setsid();
  if(pid < 0){
    perror("setsid");
    exit(-1);
  }
  chdir("/");
  umask(0);
  int cnt = sysconf(_SC_OPEN_MAX);
  for(i=0;i<cnt;i++){
    close(i);
  }
  open("/dev/null",O_RDWR);
  open("/dev/null",O_RDWR);
  open("/dev/null",O_RDWR);
}


int tonji(char *name){
  int a=1,b;
  char f[1000];
  DIR *fdir;
  struct dirent *fptr;
  sprintf(f,"/home/xiaoye.txt");
  FILE *p =fopen(f,"a");
  char f_name[1000];
  for(b=1;b<=4;b++){
    sprintf(f_name,"%s%d_ft.mp4",name,b);
    fdir = opendir("/home/mp4_file");
    while((fptr = readdir(fdir)) != NULL){
      if (strcmp(fptr->d_name,".")==0 || strcmp(fptr->d_name,"..")==0 || strcmp(fptr->d_name," ")==0){
      }else{
        if(strcmp(f_name,fptr->d_name) == 0){
          if (p == NULL){
            printf("error is %s\n", strerror(errno));
          }else{
            fprintf(p,"file '/home/mp4_file/%s'\n",fptr->d_name);
          }
        }
      }
    }
    closedir(fdir);
  }
  fclose(p);
  return 0;
}


int main(int argc, char const *argv[]) {
  Guard();
  int b,age,a=1;
  DIR *dir;
  struct dirent *ptr;
  dir = opendir("/home/jiequ_mpg");
  char cc[1000],dd[1000],yy[1000];
  char *fkuozhan=".mpg";
  while((ptr = readdir(dir)) != NULL){
      if (strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0 || strcmp(ptr->d_name,".DS_Store")==0){
      }else{
      b=tonji(strtok(ptr->d_name,fkuozhan));
      if(b==0){
        sprintf(dd,"ffmpeg -f concat -safe 0 -i /home/xiaoye.txt -c:v libx264 -strict -2 /home/法条/%s.mp4",strtok(ptr->d_name,fkuozhan));
        age=system(dd);
        if(age==0){
          sprintf(yy,"rm -rf /home/xiaoye.txt");
          system(yy);
        }
      }
    }
  }
  closedir(dir);
  return 0;
}
