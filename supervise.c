#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include "sig.h"
#include "strerr.h"
#include "error.h"
#include "fifo.h"
#include "open.h"
#include "lock.h"
#include "fork.h"
#include "wait.h"
#include "coe.h"
#include "ndelay.h"
#include "select.h"
#include "taia.h"
#include "readwrite.h"
#include "exit.h"

#define FATAL "supervise: fatal: "
#define WARNING "supervise: warning: "

char *dir;
int selfpipe[2];
int fdlock;
int fdcontrolwrite;
int fdcontrol;
int fdok;

int flagexit = 0;
int flagwant = 1;
int flagwantup = 1;
int pid = 0; /* 0 means down */
int flagpaused; /* defined if(pid) */

char status[18];

void pidchange()
{
  struct taia now;
  unsigned long u;

  taia_now(&now);
  taia_pack(status,&now);

  u = (unsigned long) pid;
  status[12] = u; u >>= 8;
  status[13] = u; u >>= 8;
  status[14] = u; u >>= 8;
  status[15] = u;
}

void announce()
{
  int fd;
  int r;

  status[16] = (pid ? flagpaused : 0);
  status[17] = (flagwant ? (flagwantup ? 'u' : 'd') : 0);

  fd = open_trunc("supervise/status.new");
  if (fd == -1) {
    strerr_warn4(WARNING,"unable to open ",dir,"/supervise/status.new: ",&strerr_sys);
    return;
  }

  r = write(fd,status,sizeof status);
  if (r == -1) {
    strerr_warn4(WARNING,"unable to write ",dir,"/supervise/status.new: ",&strerr_sys);
    close(fd);
    return;
  }
  close(fd);
  if (r < sizeof status) {
    strerr_warn4(WARNING,"unable to write ",dir,"/supervise/status.new: partial write",0);
    return;
  }

  if (rename("supervise/status.new","supervise/status") == -1)
    strerr_warn4(WARNING,"unable to rename ",dir,"/supervise/status.new to status: ",&strerr_sys);
}

void trigger()
{
  write(selfpipe[1],"",1);
}

char *run[2] = { "./run", 0 };
char **runenv;

void trystart()
{
  int f;

  switch(f = fork()) {
    case -1:
      strerr_warn4(WARNING,"unable to fork for ",dir,", sleeping 60 seconds: ",&strerr_sys);
      sleep(60);
      trigger();
      return;
    case 0:
      sleep(1);
      execve(*run,run,runenv);
      strerr_die4sys(111,FATAL,"unable to start ",dir,"/run: ");
    default:
      flagpaused = 0;
      pid = f;
      pidchange();
  }
}

void doit()
{
  fd_set rfds;
  int nfds;
  int wstat;
  int r;
  char ch;

  for (;;) {
    announce();

    if (flagexit && !pid) return;

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
	if (r == pid) { pid = 0; pidchange(); }
      }
      if (flagexit && !pid) return;
      if (!pid && flagwant && flagwantup) trystart();
    }

    if (read(fdcontrol,&ch,1) == 1)
      switch(ch) {
	case 'd':
	  flagwant = 1;
	  flagwantup = 0;
	  if (pid) { kill(pid,SIGTERM); kill(pid,SIGCONT); flagpaused = 0; }
	  break;
	case 'u':
	  flagwant = 1;
	  flagwantup = 1;
	  if (!pid) trystart();
	  break;
	case 'o':
	  flagwant = 0;
	  if (!pid) trystart();
	  break;
	case 'a':
	  if (pid) kill(pid,SIGALRM);
	  break;
	case 'h':
	  if (pid) kill(pid,SIGHUP);
	  break;
	case 'k':
	  if (pid) kill(pid,SIGKILL);
	  break;
	case 't':
	  if (pid) kill(pid,SIGTERM);
	  break;
	case 'i':
	  if (pid) kill(pid,SIGINT);
	  break;
	case 'p':
	  if (pid) kill(pid,SIGSTOP);
	  flagpaused = 1;
	  break;
	case 'c':
	  if (pid) kill(pid,SIGCONT);
	  flagpaused = 0;
	  break;
	case 'x':
	  flagexit = 1;
	  break;
      }
  }
}

main(argc,argv,envp)
int argc;
char **argv;
char **envp;
{
  struct stat st;

  runenv = envp;

  dir = argv[1];
  if (!dir || argv[2])
    strerr_die1x(100,"supervise: usage: supervise dir");

  if (pipe(selfpipe) == -1)
    strerr_die4sys(111,FATAL,"unable to create pipe for ",dir,": ");
  coe(selfpipe[0]);
  coe(selfpipe[1]);
  ndelay_on(selfpipe[0]);
  ndelay_on(selfpipe[1]);

  sig_childcatch(trigger);

  if (chdir(dir) == -1)
    strerr_die4sys(111,FATAL,"unable to chdir to ",dir,": ");

  if (stat("down",&st) != -1)
    flagwantup = 0;
  else
    if (errno != error_noent)
      strerr_die4sys(111,FATAL,"unable to stat ",dir,"/down: ");

  mkdir("supervise",0700);
  fdlock = open_append("supervise/lock");
  if ((fdlock == -1) || (lock_exnb(fdlock) == -1))
    strerr_die4sys(111,FATAL,"unable to acquire ",dir,"/supervise/lock: ");
  coe(fdlock);

  fifo_make("supervise/control",0600);
  fdcontrol = open_read("supervise/control");
  if (fdcontrol == -1)
    strerr_die4sys(111,FATAL,"unable to read ",dir,"/supervise/control: ");
  coe(fdcontrol);
  ndelay_on(fdcontrol); /* shouldn't be necessary */
  fdcontrolwrite = open_write("supervise/control");
  if (fdcontrolwrite == -1)
    strerr_die4sys(111,FATAL,"unable to write ",dir,"/supervise/control: ");
  coe(fdcontrolwrite);

  pidchange();
  announce();

  fifo_make("supervise/ok",0600);
  fdok = open_read("supervise/ok");
  if (fdok == -1)
    strerr_die4sys(111,FATAL,"unable to read ",dir,"/supervise/ok: ");
  coe(fdok);

  if (!flagwant || flagwantup) trystart(argv);

  doit();
  announce();
  _exit(0);
}
