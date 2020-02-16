#include <sys/types.h>
#include <sys/time.h>
#include "substdio.h"
#include "fmt.h"
#include "readwrite.h"
#include "exit.h"

int mywrite(fd,buf,len)
int fd;
char *buf;
int len;
{
  int w;
  w = write(fd,buf,len);
  if (w == -1) _exit(111);
  return w;
}

char outbuf[1024];
substdio ssout = SUBSTDIO_FDBUF(mywrite,1,outbuf,sizeof outbuf);

int myread(fd,buf,len) int fd; char *buf; int len;
{
  substdio_flush(&ssout);
  return read(fd,buf,len);
}

char inbuf[1024];
substdio ssin = SUBSTDIO_FDBUF(myread,0,inbuf,sizeof inbuf);

char num[FMT_ULONG];

void main()
{
  struct timeval tv;
  char ch;

  for (;;) {
    if (substdio_get(&ssin,&ch,1) != 1) _exit(0);

    gettimeofday(&tv,(struct timezone *) 0);
    substdio_put(&ssout,num,fmt_ulong(num,(unsigned long) tv.tv_sec));
    substdio_put(&ssout,".",1);
    substdio_put(&ssout,num,fmt_uint0(num,(unsigned int) tv.tv_usec,6));
    substdio_put(&ssout," ",1);

    for (;;) {
      substdio_BPUTC(&ssout,ch);
      if (ch == '\n') break;
      if (substdio_get(&ssin,&ch,1) != 1) _exit(0);
    }
  }
}
