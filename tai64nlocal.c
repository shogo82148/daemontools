#include <sys/types.h>
#include <sys/time.h>
#include "substdio.h"
#include "subfd.h"
#include "exit.h"
#include "fmt.h"

char num[FMT_ULONG];

void get(ch)
char *ch;
{
  int r;

  r = substdio_get(subfdin,ch,1);
  if (r == 1) return;
  if (r == 0) _exit(0);
  _exit(111);
}

void out(buf,len)
char *buf;
int len;
{
  if (substdio_put(subfdout,buf,len) == -1)
    _exit(111);
}

time_t secs;
unsigned long nanosecs;
unsigned long u;
struct tm *t;

main()
{
  char ch;

  for (;;) {
    get(&ch);
    if (ch == '@') {
      secs = 0;
      nanosecs = 0;
      for (;;) {
        get(&ch);
        u = ch - '0';
        if (u >= 10) {
          u = ch - 'a';
          if (u >= 6) break;
          u += 10;
        }
        secs <<= 4;
        secs += nanosecs >> 28;
        nanosecs &= 0xfffffff;
        nanosecs <<= 4;
        nanosecs += u;
      }
      secs -= 4611686018427387914ULL;
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
      out(".",1);
      out(num,fmt_uint0(num,(unsigned int) nanosecs,9));
    }
    for (;;) {
      out(&ch,1);
      if (ch == '\n') break;
      get(&ch);
    }
  }
}
