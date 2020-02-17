#include <sys/types.h>
#include <sys/stat.h>
#include "strerr.h"
#include "error.h"
#include "open.h"
#include "fmt.h"
#include "taia.h"
#include "substdio.h"
#include "readwrite.h"
#include "exit.h"

#define FATAL "svstat: fatal: "
#define WARNING "svstat: warning: "

char outbuf[1024];
substdio ssout = SUBSTDIO_FDBUF(write,1,outbuf,sizeof outbuf);

char status[18];
char strnum[FMT_ULONG];

unsigned long pid;
unsigned char normallyup;
unsigned char want;
unsigned char paused;

void doit(dir)
char *dir;
{
  struct stat st;
  int r;
  int i;
  int fd;
  char *x;

  substdio_puts(&ssout,dir);
  substdio_puts(&ssout,": ");

  if (chdir(dir) == -1) {
    x = error_str(errno);
    substdio_puts(&ssout,"unable to chdir: ");
    substdio_puts(&ssout,x);
    return;
  }

  normallyup = 0;
  if (stat("down",&st) == -1) {
    if (errno != error_noent) {
      x = error_str(errno);
      substdio_puts(&ssout,"unable to stat down: ");
      substdio_puts(&ssout,x);
      return;
    }
    normallyup = 1;
  }

  fd = open_write("supervise/ok");
  if (fd == -1) {
    if (errno == error_nodevice) {
      substdio_puts(&ssout,"supervise not running");
      return;
    }
    x = error_str(errno);
    substdio_puts(&ssout,"unable to open supervise/ok: ");
    substdio_puts(&ssout,x);
    return;
  }
  close(fd);

  fd = open_read("supervise/status");
  if (fd == -1) {
    x = error_str(errno);
    substdio_puts(&ssout,"unable to open supervise/status: ");
    substdio_puts(&ssout,x);
    return;
  }
  r = read(fd,status,sizeof status);
  close(fd);
  if (r == -1) {
    substdio_puts(&ssout,"unable to read supervise/status: ");
    substdio_puts(&ssout,x);
    return;
  }
  if (r < sizeof status) {
    substdio_puts(&ssout,"unable to read supervise/status: bad format");
    return;
  }

  pid = (unsigned char) status[15];
  pid <<= 8; pid += (unsigned char) status[14];
  pid <<= 8; pid += (unsigned char) status[13];
  pid <<= 8; pid += (unsigned char) status[12];

  paused = status[16];
  want = status[17];

  if (pid) {
    substdio_puts(&ssout,"up (pid ");
    substdio_put(&ssout,strnum,fmt_ulong(strnum,pid));
    substdio_puts(&ssout,")");
    if (!normallyup)
      substdio_puts(&ssout,", normally down");
  }
  else {
    substdio_puts(&ssout,"down");
    if (normallyup)
      substdio_puts(&ssout,", normally up");
  }

  if (pid && paused)
    substdio_puts(&ssout,", paused");
  if (!pid && (want == 'u'))
    substdio_puts(&ssout,", want up");
  if (pid && (want == 'd'))
    substdio_puts(&ssout,", want down");
}

main(argc,argv)
int argc;
char **argv;
{
  int fdorigdir;
  char *dir;

  ++argv;

  fdorigdir = open_read(".");
  if (fdorigdir == -1)
    strerr_die2sys(111,FATAL,"unable to open current directory: ");

  while (dir = *argv++) {
    doit(dir);
    substdio_puts(&ssout,"\n");
    if (fchdir(fdorigdir) == -1)
      strerr_die2sys(111,FATAL,"unable to set directory: ");
  }

  substdio_flush(&ssout);

  _exit(0);
}
