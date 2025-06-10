/* COMMONLIB.C
 *
 * Author: Isonguyo John <isongjohn014@gmail.com>
 * Created on 14/05/2024.
 *
 * Modified by: Isonguyo John
 * Last modified: 30/05/2024.
 *
 *
 * C library of common and useful functions.
 */

#include <ctype.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "commonlib.h"


char *str_concat(const char *str1,const char *str2)
{
  size_t len = strlen(str1)+strlen(str2)+1;
  char *newstr = NULL;
  if((newstr = calloc(len,sizeof(char))) == NULL)
    return NULL;
  strcpy(newstr,str1) && strcat(newstr,str2);
  return newstr;
}


ssize_t readfile(const char *fp,void *buf, size_t c)
{
  int fd = -1;
  ssize_t nbytes =  -1;
  sigset_t mask,oldmask;

  sigemptyset(&mask);
  sigfillset(&mask);
  if(sigprocmask(SIG_BLOCK,&mask,&oldmask) <0)
    return -1;

  if((fd = open(fp,O_RDONLY|O_NOCTTY)) < 0)
    goto out;
  nbytes = read(fd,buf,c);

out:
  close(fd);
  sigprocmask(SIG_SETMASK,&oldmask,NULL);
  return nbytes;
}


ssize_t writefile(const char *path,void *buffer,size_t count)
{
  int fd = -1;
  ssize_t nbytes =  -1;
  sigset_t mask,oldmask;

  sigemptyset(&mask);
  sigfillset(&mask);
  if(sigprocmask(SIG_BLOCK,&mask,&oldmask) <0)
    return -1;

  if((fd = open(path, O_WRONLY|O_TRUNC|O_NOCTTY|O_CREAT, S_IRWXU|S_IRGRP|S_IROTH)) < 0)
      goto out;

  nbytes  = write(fd,buffer,count);
  

out:
  close(fd);
  sigprocmask(SIG_SETMASK,&oldmask,NULL);
  return nbytes;
}


ssize_t traversdirr(const char *dirpath,DIRENTRY buf[],size_t count,int flag)
{
  ssize_t c  = 0;
  DIR *dirp;
  struct dirent *entr;
  struct stat sbuf;
  char pathname[PATH_MAX] = {0};

  if((dirp = opendir(dirpath)) == NULL)
    goto out;

  errno = 0;
  while((entr = readdir(dirp)) != NULL && c < count)
  {
    if(strcmp(entr->d_name,".") == 0 || strcmp(entr->d_name,"..") == 0)
      continue;

    if(flag && (entr->d_name[0] == '.'))
      continue;

    if((PATH_MAX - strlen(dirpath)) <= 0)
    {
      c = -3;
      errno = ENAMETOOLONG;
      goto out;
    }// endif comparison 

    strcpy(pathname,dirpath);
    if(pathname[strlen(pathname)-1] != '/')
      strcat(pathname,"/");

    strcat(pathname,entr->d_name);
    memset(&sbuf,0,sizeof(sbuf));

    if(lstat(pathname,&sbuf) < 0)
    {
      if(errno == EACCES)
	continue;
      c = -4;
      goto out;
    }// endif stat

    if(S_ISDIR(sbuf.st_mode))
    { 
      ssize_t tmpc;
      if((tmpc = traversdirr(pathname,&buf[c],count -c,flag)) < 0)
      {
	if( c != 0)
	  continue;
	c = tmpc;
	goto out;
      }//endif scandir_r

      c += tmpc;
    }//endif S_ISDIR
    else if(S_ISREG(sbuf.st_mode))
    {
      DIRENTRY *dptr = &buf[c];
      dptr->mode = sbuf.st_mode;
      dptr->entry  = *entr;
      strncpy(dptr->path,pathname,PATH_MAX);
      ++c;
    }// endif S_ISREG

  }// endif while

out:
  closedir(dirp);
  return c;

}


int64_t gcd(int64_t a,int64_t b)
{
  int64_t sm,lg,rem;

  a = ABS(a),b=ABS(b);
  sm = (lg = MAX(a,b)) == a?b:a;

  if((lg == sm) || (lg % sm == 0))
    return sm;

  while((rem = lg % sm) != 0)
  {
    lg = sm;
    sm = rem;
  }


  return sm;
}


char *str_toupper(char *str,size_t len)
{
  char *ptr = str;
  for(size_t j = 0; j < len && ptr != NULL; ++j,++ptr)
  {
    if(isalpha(*ptr))
      str[j] = toupper(*ptr);
  }

  return str;
}

char *str_tolower(char *str, size_t len)
{
  char *ptr = str;
  for(size_t j = 0; j < len && ptr != NULL; ++ptr,++j)
  {
    if(isalpha(*ptr))
      str[j] = tolower(*ptr);
  }

  return str;
}


char *str_rev(char *str)
{
  if(str == NULL)
    return NULL;

  size_t len = strlen(str);

  char *p = str , *q = str + len - 1;

  while(q>p)
  {
    char tmp = *p;;
    *p++  = *q;
    *q-- = tmp;
  }

  return str;
}


size_t str_slice(char dst[],const char *str,int start,int end)
{
  
  if(start < 0 || end < 0)
    return 0;
  
  int len = end - start,i;

  if(len < 0)
    return 0;

  const char *p = str+start;

  for(i=0; i < len && p != NULL; ++i)
    dst[i] = *p++;

  dst[i] = '\0';

  return (size_t) len;
}


char *str_trim(char str[])
{
  char *p = str, *q = str + strlen(str)-1;

  while(isblank(*p))
    ++p;
  while(q > p && isblank(*q))
    --q;

  size_t len = q - p + 1;
  memmove(str,p,len);
  str[len] = '\0';
  return str;
}



char *readline_fp(const char *path, char buf[],size_t bufsz, int lineno)
{
  FILE *fp;
  int ch;
  int curr_line = 0;
  long offset = 0;

  if((fp = fopen(path, "rb")) == NULL)
    return NULL;

  while((ch = getc(fp)) != EOF && curr_line != lineno)
  {
    if(ch == EOL)
      ++curr_line;

    ++offset;
  }

  if(curr_line != lineno)
    return NULL;

  if(fseek(fp,offset,SEEK_SET) != 0)
    return NULL;

  if(fgets(buf,bufsz,fp) == NULL)
    return NULL;

  fclose(fp);
  return buf;
}


char *readline_fd(int fd, char buf[],size_t bufsz, int lineno)
{
  FILE *fp;
  int ch;
  fpos_t fpos; 
  int curr_line = 0;
  long offset = 0;

  if((fp = fdopen(fd, "rb")) == NULL)
    return NULL;

  fgetpos(fp,&fpos);
  rewind(fp);
  while((ch = getc(fp)) != EOF && curr_line != lineno)
  {
    if(ch == EOL)
      ++curr_line;

    ++offset;
  }

  if(curr_line != lineno)
    return NULL;

  if(fseek(fp,offset,SEEK_SET) != 0)
    return NULL;

  if(fgets(buf,bufsz,fp) == NULL)
    return NULL;

  fflush(fp);
  fsetpos(fp,&fpos);
  return buf;

}



ssize_t itob(int n, char *s, int b)
{
  if(b < 2 || b > 36)
    return -1;

  int sign;
  char digitchar[36] = {'0','1','2','3','4',
    '5','6','7','8','9','A','B','C','D','E',
    'F','G','H','I','J','K','L','M','N','O',
    'P','Q','R','S','T','U','V','W','X','Y',
    'Z'};
  
  sign = n < 0?-1:1;
  if(sign < 0)
    n *= -1;

  ssize_t len =0;
  if(n == 0)
  {
    if(s)
      s[len++] = digitchar[n];
    else
      ++len;
  }

  while(n > 0)
  {
    int rem = n % b;
    if(s)
      s[len++] = digitchar[rem];
    else
      ++len;
    n /= b;
  }

  if(sign == -1)
  {
    if(s)
      s[len++] = '-';
    else
      ++len;
  }

  if(s)
  {
    //Reverse s in place;
    char c;
    for(ssize_t j=len,i=0; i < j; --j,++i)
      c = s[i], s[i] = s[j-1], s[j-1] = c;

    // s is NUL terminated
    s[len] = '\0';
  }


  //length of result string including
  // the NUL byte
  return ++len;
}
