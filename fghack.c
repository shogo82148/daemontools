#include "fork.h"
#include "wait.h"
#include "error.h"
#include "strerr.h"
#include "readwrite.h"
#include "exit.h"

#define FATAL "fghack: fatal: "

main(argc,argv)
int argc;
char **argv;
{
  char ch;
  int wstat;
  int pid;
  int pi[2];
  int i;

  if (!argv[1])
    strerr_die1x(100,"fghack: usage: fghack child");

  if (pipe(pi) == -1)
    strerr_die2sys(111,FATAL,"unable to create pipe: ");

  switch(pid = fork()) {
    case -1:
      strerr_die2sys(111,FATAL,"unable to fork: ");
    case 0:
      close(pi[0]);
      for (i = 0;i < 30;++i)
        dup(pi[1]);
      execvp(argv[1],argv + 1);
      strerr_die4sys(111,FATAL,"unable to run ",argv[1],": ");
  }

  close(pi[1]);

  for (;;) {
    i = read(pi[0],&ch,1);
    if ((i == -1) && (errno == error_intr)) continue;
    if (i == 1) continue;
    break;
  }

  if (wait_pid(&wstat,pid) == -1)
    strerr_die2sys(111,FATAL,"wait failed: ");
  if (wait_crashed(wstat))
    strerr_die2x(111,FATAL,"child crashed");
  _exit(wait_exitcode(wstat));
}
