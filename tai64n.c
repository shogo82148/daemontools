#include "timestamp.h"
#include "substdio.h"
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
  int r;
  substdio_flush(&ssout);
  r = read(fd,buf,len);
  if (r == -1) _exit(111);
  return r;
}

char inbuf[1024];
substdio ssin = SUBSTDIO_FDBUF(myread,0,inbuf,sizeof inbuf);

char stamp[25];

main()
{
  char ch;
  int i;

  for (;;) {
    if (substdio_get(&ssin,&ch,1) != 1) _exit(0);

    timestamp(stamp);
    substdio_put(&ssout,stamp,sizeof stamp);
    substdio_put(&ssout," ",1);

    for (;;) {
      substdio_BPUTC(&ssout,ch);
      if (ch == '\n') break;
      if (substdio_get(&ssin,&ch,1) != 1) _exit(0);
    }
  }
}
