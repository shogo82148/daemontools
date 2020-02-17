#include <sys/types.h>
#include <sys/stat.h>
#include "direntry.h"
#include "strerr.h"
#include "error.h"
#include "open.h"
#include "fork.h"
#include "wait.h"
#include "coe.h"
#include "fd.h"
#include "exit.h"
#include "str.h"

#define WARNING "svscan: warning: "
#define FATAL "svscan: fatal: "

struct {
  unsigned long dev;
  unsigned long ino;
  int flaglog;
  int pid; /* 0 if not running */
  int pidlog; /* 0 if not running */
  int pi[2]; /* defined if flaglog */
} x[1000];
int numx = 0;

void reap()
{
  int r;
  int wstat;
  int i;

  for (;;) {
    r = wait_nohang(&wstat);
    if (!r) break;
    if (r == -1) {
      if (errno == error_intr) continue;
      break;
    }

    for (i = 0;i < numx;++i) {
      if (x[i].pid == r) { x[i].pid = 0; break; }
      if (x[i].pidlog == r) { x[i].pidlog = 0; break; }
    }
  }
}

void start(fn)
char *fn;
{
  struct stat st;
  int child;
  int i;
  char *args[3];

  if (str_equal(fn,".")) return;
  if (str_equal(fn,"..")) return;

  if (stat(fn,&st) == -1) {
    strerr_warn4(WARNING,"unable to stat ",fn,": ",&strerr_sys);
    return;
  }

  if ((st.st_mode & S_IFMT) != S_IFDIR) return;

  for (i = 0;i < numx;++i)
    if (x[i].ino == st.st_ino)
      if (x[i].dev == st.st_dev)
	break;

  if (i == numx) {
    if (numx >= 1000) {
      strerr_warn4(WARNING,"unable to start ",fn,": already running 1000 services",0);
      return;
    }
    if (st.st_mode & 01000) {
      if (pipe(x[i].pi) == -1) {
        strerr_warn4(WARNING,"unable to create pipe for ",fn,": ",&strerr_sys);
        return;
      }
      coe(x[i].pi[0]);
      coe(x[i].pi[1]);
      x[i].flaglog = 1;
    }
    else
      x[i].flaglog = 0;
    x[i].ino = st.st_ino;
    x[i].dev = st.st_dev;
    x[i].pid = 0;
    x[i].pidlog = 0;
    ++numx;
  }

  if (!x[i].pid)
    switch(child = fork()) {
      case -1:
        strerr_warn4(WARNING,"unable to fork for ",fn,": ",&strerr_sys);
        return;
      case 0:
        if (x[i].flaglog)
	  if (fd_move(1,x[i].pi[1]) == -1)
            strerr_die4sys(111,WARNING,"unable to set up descriptors for ",fn,": ");
        args[0] = "supervise";
        args[1] = fn;
        args[2] = 0;
        execvp(args[0],args);
        strerr_die4sys(111,WARNING,"unable to start supervise ",fn,": ");
      default:
	x[i].pid = child;
    }

  if (x[i].flaglog && !x[i].pidlog)
    switch(child = fork()) {
      case -1:
        strerr_warn4(WARNING,"unable to fork for ",fn,"/log: ",&strerr_sys);
        return;
      case 0:
        if (fd_move(0,x[i].pi[0]) == -1)
          strerr_die4sys(111,WARNING,"unable to set up descriptors for ",fn,"/log: ");
	if (chdir(fn) == -1)
          strerr_die4sys(111,WARNING,"unable to switch to ",fn,": ");
        args[0] = "supervise";
        args[1] = "log";
        args[2] = 0;
        execvp(args[0],args);
        strerr_die4sys(111,WARNING,"unable to start supervise ",fn,"/log: ");
      default:
	x[i].pidlog = child;
    }
}

main()
{
  DIR *dir;
  direntry *d;

  for (;;) {
    reap();
    dir = opendir(".");
    if (!dir)
      strerr_warn2(WARNING,"unable to read directory: ",&strerr_sys);
    else {
      while (d = readdir(dir))
	start(d->d_name);
      closedir(dir);
    }
    sleep(60);
  }
}
