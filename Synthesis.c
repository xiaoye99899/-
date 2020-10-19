#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <regex.h>
#include<string.h>
#include<stdlib.h>
#include <pthread.h>
#include<errno.h>
#include<sys/stat.h>
#include<fcntl.h>
//原视频尺寸为1280x720时启用
// #define FILE_1280

//添加法条
// #define FILE_FT


#define PATH "/home/李春莲"

char file_path[]=PATH;
char file_he[]=PATH"/合成";
char file_16_9[]=PATH"/16.9";
char file_yuan[]=PATH"/原视频";
char file_biao[]=PATH"/标题";
char file_mpg[]=PATH"/mpg";
char file_ft[] =PATH"/法条";

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

int FFmpeg(char *name){
  int age;
  char e[1000],f[1000],Rm[1000];
  char *kuozhan =".mov";

  sprintf(f,"%s/xiaoye.txt",file_path);
  FILE *p = fopen(f, "w");
  if (p == NULL){
    printf("error is %s\n", strerror(errno));
  }else{
    printf("success\n");
    #ifdef FILE_FT
    //添加法条合成
    fprintf(p,"file '%s/a.mpg'\nfile '%s/%s_标题.mpg'\nfile '%s/%s_16_9.mpg'\nfile '%s/%s.mpg'\nfile '%s/%s_ft.mpg'\nfile '%s/b.mpg'",file_mpg,file_mpg,name,file_mpg,name,file_mpg,name,file_mpg,name,file_mpg);
    #endif
    fprintf(p,"file '%s/a.mpg'\nfile '%s/%s_标题.mpg'\nfile '%s/%s_16_9.mpg'\nfile '%s/%s.mpg'\nfile '%s/b.mpg'",file_mpg,file_mpg,name,file_mpg,name,file_mpg,name,file_mpg);
    fclose(p);
    sprintf(e,"ffmpeg -f concat -safe 0 -i %s/xiaoye.txt -c:v libx264 -strict -2 %s/%s.mp4",file_path,file_he,name);
    age=system(e);
    if(age==0){
      sprintf(Rm,"rm -rf %s/xiaoye.txt",file_path);
      system(Rm);
    }
  }
  return 0;
}
int FFmpeg_Effect(char *name){
  int age;
  char a[1000],b[1000],d[1000],ft[1000],ftm[1000];
  sprintf(a,"ffmpeg -loop 1 -r 50 -i %s/%s.jpg -i %s/110.mp3 -c:a copy -tune stillimage -shortest %s/%s_16_9.mp4",file_16_9,name,file_mpg,file_mpg,name);
  system(a);
  sprintf(b,"ffmpeg -loop 1 -r 50 -i %s/%s.jpg -i %s/110.mp3 -c:a copy -tune stillimage -shortest %s/%s_标题.mp4",file_biao,name,file_mpg,file_mpg,name);
  system(b);
  #ifdef FILE_FT
  /*处理法条图片*/
  sprintf(ft,"ffmpeg -loop 1 -r 50 -i %s/%s.jpg -i %s/120.mp3 -c:a copy -tune stillimage -shortest %s/%s_ft.mp4",file_ft,name,file_mpg,file_mpg,name);
  system(ft);
  #endif
  sprintf(d,"ffmpeg -i %s/%s_16_9.mp4  -qscale 0 %s/%s_16_9.mpg",file_mpg,name,file_mpg,name);
  system(d);
  sprintf(d,"ffmpeg -i %s/%s_标题.mp4  -qscale 0 %s/%s_标题.mpg",file_mpg,name,file_mpg,name);
  system(d);
  #ifdef FILE_FT
  /*处理法条图片*/
  sprintf(ftm,"ffmpeg -i %s/%s_ft.mp4  -qscale 0 %s/%s_ft.mpg",file_mpg,name,file_mpg,name);
  system(ftm);
  #endif
  FFmpeg(name);
  return 0;
}

int main(){
    Guard();
    DIR *dir;
    struct dirent *ptr;
    dir = opendir(file_yuan);
    char c[1000],c_1[1000],xiao[1000],cc[1000];
    int ret,rat,ret_a;
    char *kuozhan =".mov";
    char biao[1000],file16_9[1000];
    #ifdef FILE_1280
    //原视频尺寸为1280x720
    sprintf(c_1,"ffmpeg -i %s/视频-内封面.png -vf scale=255:720 %s/视频-内封面_720.png",file_path,file_mpg);
    system(c_1);
    #endif
    while((ptr = readdir(dir)) != NULL){
      if (strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0 || strcmp(ptr->d_name,".DS_Store")==0){
      }else{
        sprintf(biao,"%s/%s.jpg",file_biao,strtok(ptr->d_name,kuozhan));
        sprintf(file16_9,"%s/%s.jpg",file_16_9,strtok(ptr->d_name,kuozhan));
        if((access(biao,F_OK))==0 && (access(file16_9,F_OK))==0 ){
          #ifdef FILE_1280
          //原视频尺寸为1280x720
          sprintf(c,"ffmpeg -i %s/%s.mov -i %s/视频-内封面_720.png -filter_complex \"overlay=1055:0\" -qscale:v 1 %s/%s.mpg",file_yuan,strtok(ptr->d_name,kuozhan),file_mpg,file_mpg,strtok(ptr->d_name,kuozhan));
/***********************************************************************************************************************************************************************************************************************/
          #else
          //原视频大小：1920x1080
          sprintf(c,"ffmpeg -i %s/%s.mov -i %s/视频-内封面.png -filter_complex \"overlay=1582:\" -qscale:v 1 %s/ce_%s.mpg",file_yuan,strtok(ptr->d_name,kuozhan),file_path,file_mpg,strtok(ptr->d_name,kuozhan));
          #endif
          ret=system(c);
          if(ret==0){
            sprintf(cc,"ffmpeg -i %s/ce_%s.mpg -i %s/logo.png -filter_complex \"overlay=0:0\" -qscale:v 1 %s/%s.mpg",file_mpg,strtok(ptr->d_name,kuozhan),file_mpg,file_mpg,strtok(ptr->d_name,kuozhan));
            ret_a=system(cc);
            if(ret_a==0){
              rat=FFmpeg_Effect(ptr->d_name);
            }
          }
        }else{
          sprintf(xiao,"%s/synthesis.log",file_path);
          FILE *p = fopen(xiao, "a");
          if (p == NULL){
            printf("error is ");
          }else{
  	         fprintf(p,"%s\n",ptr->d_name);
           }
      }
    }
  }
  closedir(dir);
  return 0;
}
