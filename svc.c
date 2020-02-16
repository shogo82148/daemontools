#include "ndelay.h"
#include "strerr.h"
#include "open.h"
#include "sgetopt.h"
#include "substdio.h"
#include "readwrite.h"
#include "exit.h"
#include "byte.h"

#define FATAL "svc: fatal: "
#define WARNING "svc: warning: "
void die_usage()
{
  strerr_die1x(100,"svc: usage: svc [ -udorspchitk ] dir");
}

int datalen = 0;
char data[20];

substdio ss;
char ssbuf[1];

void main(argc,argv)
int argc;
char **argv;
{
  int opt;
  int fd;
  char *dir;

  while ((opt = getopt(argc,argv,"udorspchitk")) != opteof)
    switch(opt) {
      case 'u':
      case 'd':
      case 'o':
      case 'r':
      case 's':
      case 'p':
      case 'c':
      case 'h':
      case 'i':
      case 't':
      case 'k':
	if (datalen < sizeof data)
	  if (byte_chr(data,datalen,opt) == datalen)
	    data[datalen++] = opt;
	break;
      default:
	die_usage();
    }
  argv += optind;

  dir = *argv;
  if (!dir)
    die_usage();
  if (chdir(dir) == -1)
    strerr_die4sys(111,FATAL,"unable to chdir to ",dir,": ");

  fd = open_write("svcontrol");
  if (fd == -1)
    strerr_die2sys(111,FATAL,"unable to open control pipe: ");
  ndelay_off(fd);

  substdio_fdbuf(&ss,write,fd,ssbuf,sizeof ssbuf);
  if (substdio_putflush(&ss,data,datalen) == -1)
    strerr_die2sys(111,FATAL,"error writing commands: ");

  _exit(0);
}
