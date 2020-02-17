#include <sys/types.h>
#include <pwd.h>
#include "strerr.h"
#include "fmt.h"
#include "env.h"

#define FATAL "envuidgid: fatal: "

void nomem()
{
  strerr_die2x(111,FATAL,"out of memory");
}

char strnum[FMT_ULONG];

main(argc,argv)
int argc;
char **argv;
{
  char *account;
  struct passwd *pw;

  account = *++argv;
  if (!account || !*++argv)
    strerr_die1x(100,"envuidgid: usage: envuidgid account child");

  pw = getpwnam(account);
  if (!pw)
    strerr_die3x(111,FATAL,"unknown account ",account);

  if (!env_init()) nomem();
  strnum[fmt_ulong(strnum,(unsigned long) pw->pw_gid)] = 0;
  if (!env_put2("GID",strnum)) nomem();
  strnum[fmt_ulong(strnum,(unsigned long) pw->pw_uid)] = 0;
  if (!env_put2("UID",strnum)) nomem();

  execvp(*argv,argv);
  strerr_die4sys(111,FATAL,"unable to run ",*argv,": ");
}
