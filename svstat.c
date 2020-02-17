#include "strerr.h"
#include "open.h"
#include "substdio.h"
#include "readwrite.h"
#include "exit.h"
#include "fmt.h"

char outbuf[1024];
substdio ssout = SUBSTDIO_FDBUF(write,1,outbuf,sizeof outbuf);

#define FATAL "svstat: fatal: "
#define WARNING "svstat: warning: "

int fdorigdir;

char *dir;
int fd;
unsigned char status[23];

unsigned long pid;
unsigned long when;
unsigned char normallyup;
unsigned char want;
unsigned char paused;

char strnum[FMT_ULONG];

void main(argc,argv)
int argc;
char **argv;
{
  int r;

  ++argv;

  fdorigdir = open_read(".");
  if (fdorigdir == -1)
    strerr_die2sys(111,FATAL,"unable to open current directory: ");

  while (dir = *argv++) {
    if (fchdir(fdorigdir) == -1)
      strerr_die2sys(111,FATAL,"unable to set directory: ");
    if (chdir(dir) == -1)
      strerr_warn4(WARNING,"unable to chdir to ",dir,": ",&strerr_sys);
    else {
      fd = open_read("status");
      if (fd == -1)
        strerr_warn4(WARNING,"unable to read status in ",dir,": ",&strerr_sys);
      else {
	r = read(fd,status,sizeof status);
	if (r == -1)
          strerr_warn4(WARNING,"unable to read status in ",dir,": ",&strerr_sys);
	else if (r < sizeof status)
          strerr_warn4(WARNING,"unable to read status in ",dir,": bad format",0);
	else {
	  pid = status[19];
	  pid <<= 8; pid += status[18];
	  pid <<= 8; pid += status[17];
	  pid <<= 8; pid += status[16];

	  when = status[0] - 64;
	  when <<= 8; when += status[1];
	  when <<= 8; when += status[2];
	  when <<= 8; when += status[3];
	  when <<= 8; when += status[4];
	  when <<= 8; when += status[5];
	  when <<= 8; when += status[6];
	  when <<= 8; when += status[7];

	  normallyup = status[20];
	  paused = status[21];
	  want = status[22];

	  substdio_put(&ssout,strnum,fmt_ulong(strnum,when));

	  if (pid) {
	    substdio_puts(&ssout," up pid ");
	    substdio_put(&ssout,strnum,fmt_ulong(strnum,pid));
	    if (!normallyup)
	      substdio_puts(&ssout,", normally down");
	  }
	  else {
	    substdio_puts(&ssout," down");
	    if (normallyup)
	      substdio_puts(&ssout,", normally up");
	  }

	  if (pid && paused)
	    substdio_puts(&ssout,", paused");
	  if (!pid && (want == 'u'))
	    substdio_puts(&ssout,", want up");
	  if (pid && (want == 'd'))
	    substdio_puts(&ssout,", want down");

	  substdio_puts(&ssout,"\n");
	}
        close(fd);
      }
    }
  }

  substdio_flush(&ssout);

  _exit(0);
}
