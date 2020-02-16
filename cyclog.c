#include <sys/types.h>
#include <sys/time.h>
#include "direntry.h"
#include "substdio.h"
#include "sgetopt.h"
#include "strerr.h"
#include "scan.h"
#include "fmt.h"
#include "now.h"

#define FATAL "cyclog: fatal: "
#define WARNING "cyclog: warning: "
void die_usage()
{
  strerr_die1x(100,"cyclog: usage: cyclog [ -ssize ] [ -nnum ] [ -mmargin ] dir");
}

unsigned long size = 104000;
unsigned long num = 10;
unsigned long margin = 4000;

void chop()
{
  DIR *dir;
  direntry *d;
  int count;
  unsigned long stamp;
  unsigned long oldest;

  for (;;) {
    for (;;) {
      dir = opendir(".");
      if (dir) break;
      strerr_warn2(WARNING,"unable to read directory, pausing: ",&strerr_sys);
      sleep(60);
    }
  
    oldest = 0;
    count = 0;
    while (d = readdir(dir))
      if (d->d_name[0] == '@') {
        ++count;
	scan_ulong(d->d_name + 1,&stamp);
	if (!oldest || (stamp < oldest))
	  oldest = stamp;
      }
  
    closedir(dir);
  
    if (count < num) return;

    for (;;) {
      dir = opendir(".");
      if (dir) break;
      strerr_warn2(WARNING,"unable to read directory, pausing: ",&strerr_sys);
      sleep(60);
    }
  
    while (d = readdir(dir))
      if (d->d_name[0] == '@') {
	scan_ulong(d->d_name + 1,&stamp);
	if (stamp == oldest) {
	  if (unlink(d->d_name) == -1) {
            strerr_warn4(WARNING,"unable to remove ",d->d_name,", pausing: ",&strerr_sys);
	    sleep(60);
	  }
	  break;
	}
      }
  
    closedir(dir);
  }
}

char fn[10 + FMT_ULONG];

int safewrite(fd,buf,len)
int fd;
char *buf;
int len;
{
  int w;
  for (;;) {
    w = write(fd,buf,len);
    if (w > 0) return w;
    strerr_warn4(WARNING,"unable to write to ",fn,", pausing: ",&strerr_sys);
    sleep(60);
  }
}

char outbuf[1024];
substdio ssout;

int flushread(fd,buf,len) int fd; char *buf; int len;
{
  substdio_flush(&ssout);
  return read(fd,buf,len);
}

char inbuf[1024];
substdio ssin = SUBSTDIO_FDBUF(flushread,0,inbuf,sizeof inbuf);

void main(argc,argv)
int argc;
char **argv;
{
  int opt;
  char *dir;
  unsigned long bytes;
  char ch;
  int flageof;
  unsigned long lastnow;
  int fd;

  umask(022);

  while ((opt = getopt(argc,argv,"s:n:m:")) != opteof)
    switch(opt) {
      case 's':
	scan_ulong(optarg,&size);
	if (size < 512) size = 512;
	break;
      case 'n':
	scan_ulong(optarg,&num);
	if (num < 1) num = 1;
	break;
      case 'm':
	scan_ulong(optarg,&margin);
	if (margin >= size) margin = size;
	break;
      default:
	die_usage();
    }

  argv += optind;
  dir = *argv;
  if (!dir)
    die_usage();

  if (chdir(dir) == -1)
    strerr_die4sys(111,FATAL,"cyclog: unable to chdir to ",dir,": ");

  lastnow = now();
  flageof = 0;
  while (!flageof) {
    while (now() == lastnow)
      sleep(1);
    chop();

    for (;;) {
      lastnow = now();
      fn[0] = '@';
      fn[1 + fmt_ulong(fn + 1,lastnow)] = 0;
      fd = open_excl(fn);
      if (fd != -1) break;
      strerr_warn4(WARNING,"unable to create ",fn,", pausing: ",&strerr_sys);
      sleep(60);
    }

    substdio_fdbuf(&ssout,safewrite,fd,outbuf,sizeof outbuf);
    for (bytes = 0;bytes < size;++bytes) {
      if (substdio_get(&ssin,&ch,1) < 1) { flageof = 1; break; }
      substdio_BPUTC(&ssout,ch);
      if ((ch == '\n') && (bytes >= size - margin)) break;
    }
    substdio_flush(&ssout);

    while (fsync(fd) == -1) {
      strerr_warn4(WARNING,"unable to sync to ",fn,", pausing: ",&strerr_sys);
      sleep(60);
    }
    fchmod(fd,0444); /* if it fails, too bad */
    close(fd);
  }

  _exit(0);
}
