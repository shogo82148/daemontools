#include <sys/types.h>
#include <time.h>
#include "substdio.h"
#include "subfd.h"
#include "exit.h"
#include "fmt.h"

char num[FMT_ULONG];

void get(ch)
char *ch;
{
  if (substdio_get(subfdin,ch,1) != 1) {
    substdio_flush(subfdout);
    _exit(0);
  }
}

void out(buf,len)
char *buf;
int len;
{
  if (substdio_put(subfdout,buf,len) == -1)
    _exit(111);
}

time_t secs;
unsigned long u;
struct tm *t;

void main()
{
  char ch;

  for (;;) {
    secs = 0;
    for (;;) {
      get(&ch);
      u = ch - '0';
      if (u >= 10) break;
      secs = secs * 10 + u;
    }
    t = localtime(&secs);
    out(num,fmt_ulong(num,(unsigned long) (1900 + t->tm_year)));
    out("-",1);
    out(num,fmt_uint0(num,(unsigned int) (1 + t->tm_mon),2));
    out("-",1);
    out(num,fmt_uint0(num,(unsigned int) t->tm_mday,2));
    out(" ",1);
    out(num,fmt_uint0(num,(unsigned int) t->tm_hour,2));
    out(":",1);
    out(num,fmt_uint0(num,(unsigned int) t->tm_min,2));
    out(":",1);
    out(num,fmt_uint0(num,(unsigned int) t->tm_sec,2));
    for (;;) {
      out(&ch,1);
      if (ch == '\n') break;
      get(&ch);
    }
  }
}
