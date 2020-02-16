#include <sys/types.h>
#include <sys/time.h>
#include "substdio.h"
#include "subfd.h"
#include "fmt.h"
#include "exit.h"

void out(buf,len) char *buf; unsigned int len;
{
  if (substdio_put(subfdoutsmall,buf,len) == -1) _exit(111);
}

char num[FMT_ULONG];

void main()
{
  struct timeval tv;
  int flagbeginline;
  char ch;

  flagbeginline = 1;
  while (substdio_get(subfdinsmall,&ch,1) == 1) {
    if (flagbeginline) {
      gettimeofday(&tv,(struct timezone *) 0);
      out(num,fmt_ulong(num,(unsigned long) tv.tv_sec));
      out(".",1);
      out(num,fmt_uint0(num,(unsigned int) tv.tv_usec,6));
      out(" ",1);
      flagbeginline = 0;
    }
    out(&ch,1);
    if (ch == '\n') flagbeginline = 1;
  }
  substdio_flush(subfdoutsmall);
  _exit(0);
}
