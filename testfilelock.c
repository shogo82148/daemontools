#include "lock.h"
#include "strerr.h"
#include "exit.h"
#include "error.h"

#define FATAL "testfilelock: fatal: "

void main(argc,argv)
int argc;
char **argv;
{
  char *fn;
  int fd;

  fn = argv[1];
  if (!fn)
    strerr_die1x(100,"testfilelock: usage: testfilelock filename");

  fd = open_write(fn);
  if (fd == -1)
    strerr_die4sys(111,FATAL,"unable to open ",fn,": ");

  if (lock_exnb(fd) == -1)
    if ((errno == error_again) || (errno == error_wouldblock))
      _exit(0);
    else
      strerr_die4sys(111,FATAL,"trouble locking ",fn,": ");

  _exit(100);
}
