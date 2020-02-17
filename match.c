#include "match.h"

int match(pattern,buf,len)
char *pattern;
char *buf;
int len;
{
  char ch;

  for (;;) {
    ch = *pattern++;
    if (!ch) return !len;
    if (ch == '*') {
      ch = *pattern;
      if (!ch) return 1;
      for (;;) {
        if (!len) return 0;
        if (*buf == ch) break;
        ++buf; --len;
      }
      continue;
    }
    if (!len) return 0;
    if (*buf != ch) return 0;
    ++buf; --len;
  }
}
