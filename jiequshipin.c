#include <stdio.h>
#include <stdlib.h>
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/avutil.h"
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <regex.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

//编译命令： gcc xiaoye1.c -I/usr/include/ffmpeg/ -L/usr/lib64/ -lavcodec -lavformat -lswscale -lavutil 


char file_path[] ="/home/chenlong";


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

int Time_vode(char *fname){


  int64_t duration;
  int tns, thh, tmm, tss;
  char Time_name[1000];
  float xxiao;
  // CString timelong;

  //封装格式上下文，统领全局的结构体，保存了视频文件封装格式的相关信息
  AVFormatContext * avf_ctx = avformat_alloc_context();

  int video_index = -1;
  //设置日志级别
  av_log_set_level(AV_LOG_INFO);
   //1.注册所有组件
  av_register_all();
  //2.打开视频文件
  int ret = avformat_open_input(&avf_ctx,fname,NULL,NULL);
  if(ret < 0){
      av_log(NULL,AV_LOG_ERROR,"Can't open the file %s \n",fname);
      return -1;
  }
  //3.获取视频文件信息
  ret = avformat_find_stream_info(avf_ctx,NULL);
  if(ret < 0){
      av_log(NULL,AV_LOG_ERROR,"can't find the stream");

  }

  //视频的时长可以转换成HH:MM:SS的形式
  //duration是以微秒为单位
  //转换成hh:mm:ss形式

  tns  = (avf_ctx->duration)/1000000;
  // thh  = tns / 3600;
  // tmm  = (tns % 3600) / 60;
  // tss  = (tns % 60);

  return tns;
}



int main(int argc, char const *argv[]) {
  Guard();
  int Time_age;
  DIR *dir;
  struct dirent *ptr;
  dir = opendir(file_path);
  char cc[100000],yy[1000];

  while ((ptr = readdir(dir)) != NULL) {
    if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0){
    }else{

      sprintf(yy,"/home/chenlong/%s",ptr->d_name);
      Time_age = Time_vode(yy)-4;
      sprintf(cc,"ffmpeg -y -vn -ss 00:00:00.00 -t %d -i /home/chenlong/%s -acodec copy /home/jiequ_mpg/%s",Time_age,ptr->d_name,ptr->d_name);
      system(cc);

    }
  }

  return 0;
}
