#include "open.h"
#include "fd.h"
#include "exit.h"
#include "ndelay.h"

void main(argc,argv)
int argc;
char **argv;
{
  char *fn;
  int fd;

  fn = argv[1];
  if (!fn) _exit(100);

  argv += 2;
  if (!*argv) _exit(100);

  fd = open_append(fn);
  if (fd == -1) _exit(111);

  if (fd_move(2,fd) == -1) _exit(111);

  ndelay_off(2);

  execvp(*argv,argv);
  _exit(111);
}
