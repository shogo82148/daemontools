#include "match.h"
#include "substdio.h"
#include "subfd.h"
#include "str.h"

main(argc,argv)
int argc;
char **argv;
{
  char *pattern = "";
  char *buf = "";

  if (argv[1]) {
    pattern = argv[1];
    if (argv[2])
      buf = argv[2];
  }

  substdio_puts(subfdout,match(pattern,buf,str_len(buf)) ? "+" : "-");
  substdio_puts(subfdout,pattern);
  substdio_puts(subfdout," (");
  substdio_puts(subfdout,buf);
  substdio_puts(subfdout,")\n");
  substdio_flush(subfdout);
  _exit(0);
}
