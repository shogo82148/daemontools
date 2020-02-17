#include <sys/types.h>
#include <pwd.h>
#include "strerr.h"
#include "prot.h"

#define FATAL "setuidgid: fatal: "

main(argc,argv)
int argc;
char **argv;
{
  char *account;
  struct passwd *pw;

  account = *++argv;
  if (!account || !*++argv)
    strerr_die1x(100,"setuidgid: usage: setuidgid account child");

  pw = getpwnam(account);
  if (!pw)
    strerr_die3x(111,FATAL,"unknown account ",account);

  if (prot_gid(pw->pw_gid) == -1)
    strerr_die2sys(111,FATAL,"unable to setgid: ");
  if (prot_uid(pw->pw_uid) == -1)
    strerr_die2sys(111,FATAL,"unable to setuid: ");

  execvp(*argv,argv);
  strerr_die4sys(111,FATAL,"unable to run ",*argv,": ");
}
