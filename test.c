
#include "commonlib.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define N 255



int main(void)
{
  char buf[N] = {0};
  off_t pos;

  int fd = open("test.c",O_RDONLY);
  lseek(fd,0,SEEK_END);

  pos = lseek(fd,0,SEEK_CUR);
  

  printf("before: %ld\n",pos);
  if(readline_fd(fd,buf,N,12) == NULL)
  {
    perror("readline_fp");
  }

  pos = lseek(fd,0,SEEK_CUR);
  printf("%s\n",buf);
  printf("after: %ld\n",pos);
}


