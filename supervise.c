#include <signal.h>
#include "now.h"
#include "fmt.h"
#include "sig.h"
#include "coe.h"
#include "open.h"
#include "wait.h"
#include "fork.h"
#include "lock.h"
#include "fifo.h"
#include "error.h"
#include "select.h"
#include "strerr.h"
#include "sgetopt.h"
#include "substdio.h"
#include "readwrite.h"

#define FATAL "supervise: fatal: "
#define WARNING "supervise: warning: "
void die_usage()
{
  strerr_die1x(100,"supervise: usage: supervise [ -rsudo ] dir program args ...");
}

int flagnormallyup = 1;
int flagwant = 1;
int flagwantup = 1;
int pid = 0; /* 0 means down */
int flagpaused; /* defined if(pid) */

int fdexecdir;
int fdlock;
int fdcontrolwrite;
int fdcontrol;
int selfpipe[2];

char mark[FMT_ULONG];
char pidstr[FMT_ULONG];

void trigger()
{
  write(selfpipe[1],"",1);
}

void statusmark()
{
  mark[fmt_ulong(mark,(unsigned long) now())] = 0;
  pidstr[fmt_ulong(pidstr,(unsigned long) pid)] = 0;
}

void announce()
{
  int fd;
  substdio ss;
  char ssbuf[64];

  fd = open_trunc("status.new");
  if (fd == -1) {
    strerr_warn2(WARNING,"unable to open status.new: ",&strerr_sys);
    return;
  }
  substdio_fdbuf(&ss,write,fd,ssbuf,sizeof ssbuf);

  if (substdio_puts(&ss,mark) == -1) goto writeerr;

  if (pid) {
    if (substdio_puts(&ss," up pid ") == -1) goto writeerr;
    if (substdio_puts(&ss,pidstr) == -1) goto writeerr;
    if (!flagnormallyup)
      if (substdio_puts(&ss,", normally down") == -1) goto writeerr;
  }
  else {
    if (substdio_puts(&ss," down") == -1) goto writeerr;
    if (flagnormallyup)
      if (substdio_puts(&ss,", normally up") == -1) goto writeerr;
  }

  if (pid && flagpaused)
    if (substdio_puts(&ss,", paused") == -1) goto writeerr;
  if (flagwant && flagwantup && !pid)
    if (substdio_puts(&ss,", want up") == -1) goto writeerr;
  if (flagwant && !flagwantup && pid)
    if (substdio_puts(&ss,", want down") == -1) goto writeerr;

  if (substdio_puts(&ss,"\n")) goto writeerr;
  if (substdio_flush(&ss) == -1) goto writeerr;
  close(fd);
  if (rename("status.new","status") == -1)
    strerr_warn2(WARNING,"unable to rename status.new to status: ",&strerr_sys);
  return;

  writeerr:
  strerr_warn2(WARNING,"unable to write status.new: ",&strerr_sys);
  close(fd);
  return;
}

void trystart(argv)
char **argv;
{
  int f;

  switch(f = fork()) {
    case -1:
      strerr_warn2(WARNING,"unable to fork, sleeping 60 seconds: ",&strerr_sys);
      sleep(60);
      trigger();
      return;
    case 0:
      sleep(1);
      if (fchdir(fdexecdir) == -1)
	strerr_die2sys(111,FATAL,"unable to set directory: ");
      execvp(*argv,argv);
      strerr_die4sys(111,FATAL,"unable to run ",*argv,": ");
    default:
      pid = f;
      flagpaused = 0;
      statusmark();
  }
}

void main(argc,argv)
int argc;
char **argv;
{
  int opt;
  char *dir;

  while ((opt = getopt(argc,argv,"rsudo")) != opteof)
    switch(opt) {
      case 'r': flagnormallyup = 1;
      case 'u': flagwant = 1; flagwantup = 1; break;
      case 's': flagnormallyup = 0;
      case 'd': flagwant = 1; flagwantup = 0; break;
      case 'o': flagwant = 0; break;
      default: die_usage();
    }
  argv += optind;

  if (!*argv) die_usage();
  dir = *argv++;
  if (!*argv) die_usage();

  fdexecdir = open_read(".");
  if (fdexecdir == -1)
    strerr_die2sys(111,FATAL,"unable to open current directory: ");
  coe(fdexecdir);

  if (chdir(dir) == -1)
    strerr_die4sys(111,FATAL,"unable to chdir to ",dir,": ");

  if (pipe(selfpipe) == -1)
    strerr_die2sys(111,FATAL,"unable to create pipe: ");
  coe(selfpipe[0]);
  coe(selfpipe[1]);
  ndelay_on(selfpipe[0]);
  ndelay_on(selfpipe[1]);

  sig_childcatch(trigger);

  fdlock = open_append("lock");
  if (fdlock == -1)
    strerr_die2sys(111,FATAL,"unable to open lock: ");
  coe(fdlock);
  if (lock_exnb(fdlock) == -1)
    strerr_die2sys(111,FATAL,"unable to acquire lock: ");

  fifo_make("svcontrol",0600);

  fdcontrol = open_read("svcontrol");
  if (fdcontrol == -1)
    strerr_die2sys(111,FATAL,"unable to open svcontrol: ");
  coe(fdcontrol);
  ndelay_on(fdcontrol); /* shouldn't be necessary */
  fdcontrolwrite = open_write("svcontrol");
  if (fdcontrolwrite == -1)
    strerr_die2sys(111,FATAL,"unable to open svcontrol for writing: ");
  coe(fdcontrolwrite);

  statusmark();
  if (!flagwant || flagwantup) trystart(argv);

  for (;;) {
    fd_set rfds;
    int nfds;
    int wstat;
    int r;
    char ch;

    announce();

    FD_ZERO(&rfds);
    FD_SET(selfpipe[0],&rfds);
    nfds = selfpipe[0] + 1;
    FD_SET(fdcontrol,&rfds);
    if (fdcontrol >= nfds) nfds = fdcontrol + 1;

    select(nfds,&rfds,(fd_set *) 0,(fd_set *) 0,(struct timeval *) 0);

    if (read(selfpipe[0],&ch,1) == 1) {
      for (;;) {
	r = wait_nohang(&wstat);
	if (!r || ((r == -1) && (errno != error_intr))) break;
	if (r == pid) { pid = 0; statusmark(); }
      }
      if (!pid) if (flagwant) if (flagwantup) trystart(argv);
    }

    if (read(fdcontrol,&ch,1) == 1)
      switch(ch) {
	case 'd':
	  flagwant = 1; flagwantup = 0;
	  if (pid) { kill(pid,SIGTERM); kill(pid,SIGCONT); flagpaused = 0; }
	  break;
	case 'u': flagwant = 1; flagwantup = 1; if (!pid) trystart(argv); break;
	case 'o': flagwant = 0; if (!pid) trystart(argv); break;
	case 'a': if (pid) kill(pid,SIGALRM); break;
	case 'h': if (pid) kill(pid,SIGHUP); break;
	case 'k': if (pid) kill(pid,SIGKILL); break;
	case 't': if (pid) kill(pid,SIGTERM); break;
	case 'i': if (pid) kill(pid,SIGINT); break;
	case 'p': if (pid) kill(pid,SIGSTOP); flagpaused = 1; break;
	case 'c': if (pid) kill(pid,SIGCONT); flagpaused = 0; break;
	case 'r': flagnormallyup = 1; break;
	case 's': flagnormallyup = 0; break;
      }
  }
}
