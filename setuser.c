#include <sys/types.h>
#include <pwd.h>
#include "strerr.h"
#include "scan.h"

#define FATAL "setuser: fatal: "

void main(argc,argv)
int argc;
char **argv;
{
  char *username;
  struct passwd *pw;

  username = *++argv;
  if (!username || !*++argv)
    strerr_die1x(100,"setuser: usage: setuser username program [ arg ... ]");

  pw = getpwnam(username);
  if (!pw)
    strerr_die3x(111,FATAL,"unknown username ",username);

  if (prot_gid(pw->pw_gid) == -1)
    strerr_die2sys(111,FATAL,"unable to setgid: ");
  if (prot_uid(pw->pw_uid) == -1)
    strerr_die2sys(111,FATAL,"unable to setuid: ");

  execvp(*argv,argv);
  strerr_die4sys(111,FATAL,"unable to run ",*argv,": ");
}
