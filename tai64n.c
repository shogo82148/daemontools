#include "timestamp.h"
#include "buffer.h"
#include "readwrite.h"
#include "exit.h"

int mywrite(int fd,char *buf,int len)
{
  int w;
  w = write(fd,buf,len);
  if (w == -1) _exit(111);
  return w;
}

char outbuf[2048];
buffer out = BUFFER_INIT(mywrite,1,outbuf,sizeof outbuf);

int myread(int fd,char *buf,int len)
{
  int r;
  buffer_flush(&out);
  r = read(fd,buf,len);
  if (r == -1) _exit(111);
  return r;
}

char inbuf[1024];
buffer in = BUFFER_INIT(myread,0,inbuf,sizeof inbuf);

char stamp[TIMESTAMP + 1];

main()
{
  char ch;

  for (;;) {
    if (buffer_GETC(&in,&ch) != 1) _exit(0);

    timestamp(stamp);
    stamp[TIMESTAMP] = ' ';
    buffer_put(&out,stamp,TIMESTAMP + 1);

    for (;;) {
      buffer_PUTC(&out,ch);
      if (ch == '\n') break;
      if (buffer_GETC(&in,&ch) != 1) _exit(0);
    }
  }
}
