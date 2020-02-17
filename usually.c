#include "substdio.h"
#include "readwrite.h"
#include "strerr.h"
#include "sig.h"
#include "stralloc.h"
#include "alloc.h"
#include "gen_allocdefs.h"
#include "slurp.h"

#define FATAL "usually: fatal: "

char inbuf[2048];
substdio ssin = SUBSTDIO_FDBUF(read,0,inbuf,sizeof inbuf);

char outbuf[1];
substdio ssout = SUBSTDIO_FDBUF(write,1,outbuf,sizeof outbuf);

char errbuf[1];
substdio sserr = SUBSTDIO_FDBUF(write,2,errbuf,sizeof errbuf);

int match(line,pat)
char *line; /* \n-terminated */
char *pat; /* \n-terminated */
{
  char ch;

  for (;;) {
    ch = *pat++;
    if (ch == '*') {
      ch = *pat++;
      if (ch == '\n') return 1;
      while (*line != ch)
	if (*line++ == '\n')
	  return 0;
      ++line;
      continue;
    }
    if (*line++ != ch) return 0;
    if (ch == '\n') return 1;
  }
}

GEN_ALLOC_typedef(cpalloc,char *,x,len,a)
GEN_ALLOC_ready(cpalloc,char *,x,len,a,i,n,z,30,cpalloc_ready)

int flagreadpats = 1;
char *fn;
stralloc fncontents = {0};
cpalloc pats = {0};

void sigalrm()
{
  flagreadpats = 1;
}

void readpats()
{
  int i;
  int j;
  int k;

  flagreadpats = 0;

  if (!stralloc_copys(&fncontents,""))
    strerr_die2x(111,FATAL,"out of memory");

  switch(slurp(fn,&fncontents,32)) {
    case 0:
      strerr_die4x(111,FATAL,"unable to read ",fn,": file does not exist");
    case -1:
      strerr_die4sys(111,FATAL,"unable to read ",fn,": ");
  }

  if (!stralloc_cats(&fncontents,"\n\n")) /* guarantee blank line */
    strerr_die2x(111,FATAL,"out of memory");
 
  j = 0;
  k = 0;
  for (i = 0;i < fncontents.len;++i)
    if (fncontents.s[i] == '\n') {
      if (!cpalloc_ready(&pats,j + 1))
	strerr_die2x(111,FATAL,"out of memory");
      pats.x[j++] = fncontents.s + k;
      k = i + 1;
    }
  pats.len = j;
}

char line[504];
int linelen = 0; /* always <= 500 */

void doit()
{
  int i;

  sig_hangupblock();
  if (flagreadpats)
    readpats();
  sig_hangupunblock();

  if (linelen == 500) {
    line[500] = '.';
    line[501] = '.';
    line[502] = '.';
    linelen = 503;
  }
  line[linelen] = '\n';

  for (i = 0;i < pats.len;++i)
    if (match(line,pats.x[i]))
      break;
  if (i == pats.len)
    substdio_putflush(&sserr,line,linelen + 1);
}

void main(argc,argv)
int argc;
char **argv;
{
  int n;
  char *x;
  int i;
  char ch;

  fn = argv[1];
  if (!fn)
    strerr_die1x(100,"usually: usage: usually file");

  sig_hangupcatch(sigalrm);
  readpats();

  for (;;) {
    n = substdio_feed(&ssin);
    if (n < 0)
      strerr_die2sys(111,FATAL,"unable to read input: ");
    if (n == 0)
      _exit(0);

    x = substdio_PEEK(&ssin);
    if (substdio_putflush(&ssout,x,n) == -1)
      strerr_die2sys(111,FATAL,"unable to write output: ");

    for (i = 0;i < n;++i) {
      ch = *x++;
      if (ch == '\n') {
	doit();
	linelen = 0;
      }
      else
	if (linelen < 500)
	  line[linelen++] = ch;
    }

    substdio_SEEK(&ssin,n);
  }
}
