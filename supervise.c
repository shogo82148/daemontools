#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include "now.h"
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
  strerr_die1x(100,"supervise: usage: supervise [ -rsudox ] dir program args ...");
}

int flagexit = 0;
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

unsigned char status[23];

void trigger()
{
  write(selfpipe[1],"",1);
}

void statusmark()
{
  struct timeval tv;
  unsigned long u;

  gettimeofday(&tv,(struct timezone *) 0);
  u = tv.tv_sec;
  status[7] = u; u >>= 8;
  status[6] = u; u >>= 8;
  status[5] = u; u >>= 8;
  status[4] = u; u >>= 8;
  status[3] = u; u >>= 8;
  status[2] = u; u >>= 8;
  status[1] = u; u >>= 8;
  status[0] = u + 64; /* TAI64 */
  u = tv.tv_usec * 1000;
  status[11] = u; u >>= 8;
  status[10] = u; u >>= 8;
  status[9] = u; u >>= 8;
  status[8] = u;
  status[15] = status[14] = status[13] = status[12] = 0;
  u = (unsigned long) pid;
  status[16] = u; u >>= 8;
  status[17] = u; u >>= 8;
  status[18] = u; u >>= 8;
  status[19] = u;
}

void announce()
{
  int fd;
  int r;

  status[20] = flagnormallyup;
  status[21] = (pid ? flagpaused : 0);
  status[22] = (flagwant ? (flagwantup ? 'u' : 'd') : 0);

  fd = open_trunc("status.new");
  if (fd == -1) {
    strerr_warn2(WARNING,"unable to open status.new: ",&strerr_sys);
    return;
  }

  r = write(fd,status,sizeof status);
  if (r == -1)
    strerr_warn2(WARNING,"unable to write status.new: ",&strerr_sys);
  else if (r < sizeof status)
    strerr_warn2(WARNING,"unable to write status.new: partial write",0);
  close(fd);

  if (r == sizeof status)
    if (rename("status.new","status") == -1)
      strerr_warn2(WARNING,"unable to rename status.new to status: ",&strerr_sys);
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

  while ((opt = getopt(argc,argv,"rsudox")) != opteof)
    switch(opt) {
      case 'r': flagnormallyup = 1;
      case 'u': flagwant = 1; flagwantup = 1; break;
      case 's': flagnormallyup = 0;
      case 'd': flagwant = 1; flagwantup = 0; break;
      case 'o': flagwant = 0; break;
      case 'x': flagexit = 1; break;
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

    if (flagexit && !pid) _exit(0);

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
      if (flagexit && !pid) { announce(); _exit(0); }
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
	case 'x': flagexit = 1; break;
      }
  }
}
