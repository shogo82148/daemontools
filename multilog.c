#include <sys/types.h>
#include <sys/stat.h>
#include "direntry.h"
#include "alloc.h"
#include "exit.h"
#include "substdio.h"
#include "subfd.h"
#include "strerr.h"
#include "error.h"
#include "open.h"
#include "lock.h"
#include "scan.h"
#include "byte.h"
#include "seek.h"
#include "timestamp.h"
#include "fork.h"
#include "wait.h"
#include "coe.h"
#include "sig.h"
#include "match.h"

#define FATAL "multilog: fatal: "
#define WARNING "multilog: warning: "

void pause3(s1,s2,s3)
char *s1; char *s2; char *s3;
{
  strerr_warn4(WARNING,s1,s2,s3,&strerr_sys);
  sleep(5);
}

void pause5(s1,s2,s3,s4,s5)
char *s1; char *s2; char *s3; char *s4; char *s5;
{
  strerr_warn6(WARNING,s1,s2,s3,s4,s5,&strerr_sys);
  sleep(5);
}

int fdstartdir;

int *f;

void f_init(script)
char **script;
{
  int i;
  int j;
  int fd;

  for (i = 0;script[i];++i)
    ;

  f = (int *) alloc(i * sizeof(*f));
  if (!f) strerr_die2x(111,FATAL,"out of memory");

  for (i = 0;script[i];++i) {
    fd = -1;
    if (script[i][0] == '=') {
      if (fchdir(fdstartdir) == -1)
        strerr_die2sys(111,FATAL,"unable to switch to starting directory: ");
      fd = open_append(script[i] + 1);
      if (fd == -1)
        strerr_die4sys(111,FATAL,"unable to create ",script[i] + 1,": ");
      close(fd);
      fd = open_write(script[i] + 1);
      if (fd == -1)
        strerr_die4sys(111,FATAL,"unable to write ",script[i] + 1,": ");
      coe(fd);
    }
    f[i] = fd;
  }
}

struct cyclog {
  char buf[512];
  substdio ss;
  int fdcurrent;
  unsigned long bytes;
  unsigned long num;
  unsigned long size;
  char *processor;
  char *dir;
  int fddir;
  int fdlock;
  int flagselected;
} *c;
int cnum;

char fn[40];

void finish(d,file,code)
struct cyclog *d;
char *file;
char *code;
{
  DIR *dir;
  direntry *x;
  int count;
  int i;
  struct stat st;

  for (;;) {
    if (stat(file,&st) == 0) break;
    if (errno == error_noent) return;
    pause5("unable to stat ",d->dir,"/",file,", pausing: ");
  }

  if (st.st_nlink == 1)
    for (;;) {
      timestamp(fn);
      fn[25] = '.';
      fn[26] = code[0];
      fn[27] = 0;

      if (link(file,fn) == 0) break;
      pause5("unable to link to ",d->dir,"/",fn,", pausing: ");
    }

  while (unlink(file) == -1)
    pause5("unable to remove ",d->dir,"/",file,", pausing: ");

  for (;;) {
    while (!(dir = opendir(".")))
      pause3("unable to read ",d->dir,", pausing: ");

    fn[0] = '@';
    fn[1] = 'z';
    fn[2] = 0;

    count = 0;
    while (x = readdir(dir))
      if (x->d_name[0] == '@')
        if (str_len(x->d_name) >= 25) {
          ++count;
          if (str_diff(x->d_name,fn) < 0) {
            for (i = 0;i < 25;++i)
              fn[i] = x->d_name[i];
            fn[25] = 0;
          }
        }
    closedir(dir);

    if (count < d->num) break;

    while (!(dir = opendir(".")))
      pause3("unable to read ",d->dir,", pausing: ");

    while (x = readdir(dir))
      if (x->d_name[0] == '@')
        if (str_len(x->d_name) >= 25)
          if (str_start(x->d_name,fn)) {
            while (unlink(x->d_name) == -1)
              pause5("unable to remove ",d->dir,"/",x->d_name,", pausing: ");
            break;
          }
    closedir(dir);
  }
}

void startprocessor(d)
struct cyclog *d;
{
  int fd;

  fd = open_read("previous");
  if (fd == -1) return;
  if (fd_move(0,fd) == -1) return;
  fd = open_trunc("processed");
  if (fd == -1) return;
  if (fd_move(1,fd) == -1) return;
  fd = open_read("state");
  if (fd == -1) return;
  if (fd_move(4,fd) == -1) return;
  fd = open_trunc("newstate");
  if (fd == -1) return;
  if (fd_move(5,fd) == -1) return;
  execlp("/bin/sh","sh","-c",d->processor,(char *) 0);
}

void fullcurrent(d)
struct cyclog *d;
{
  int fd;
  int pid;
  int wstat;

  while (fchdir(d->fddir) == -1)
    pause3("unable to switch to ",d->dir,", pausing: ");

  while (fsync(d->fdcurrent) == -1)
    pause3("unable to write ",d->dir,"/current to disk, pausing: ");
  close(d->fdcurrent);

  while (rename("current","previous") == -1)
    pause3("unable to rename current to previous in directory ",d->dir,", pausing: ");
  while ((d->fdcurrent = open_append("current")) == -1)
    pause3("unable to create ",d->dir,"/current, pausing: ");
  coe(d->fdcurrent);
  d->bytes = 0;
  while (fchmod(d->fdcurrent,0644) == -1)
    pause3("unable to set mode of ",d->dir,"/current, pausing: ");

  while (chmod("previous",0744) == -1)
    pause3("unable to set mode of ",d->dir,"/previous, pausing: ");

  if (!d->processor)
    finish(d,"previous","s");
  else {
    for (;;) {
      while ((pid = vfork()) == -1)
        pause3("unable to fork for processor in ",d->dir,", pausing: ");
      if (!pid) {
        startprocessor(d);
        strerr_die4sys(111,FATAL,"unable to run ",d->processor,": ");
      }
      if (wait_pid(&wstat,pid) == -1)
        pause3("wait failed for processor in ",d->dir,", pausing: ");
      else if (wait_crashed(wstat))
        pause3("processor crashed in ",d->dir,", pausing: ");
      else if (!wait_exitcode(wstat))
        break;
      strerr_warn4(WARNING,"processor failed in ",d->dir,", pausing",0);
      sleep(5);
    }

    while ((fd = open_append("processed")) == -1)
      pause3("unable to create ",d->dir,"/processed, pausing: ");
    while (fsync(fd) == -1)
      pause3("unable to write ",d->dir,"/processed to disk, pausing: ");
    while (fchmod(fd,0744) == -1)
      pause3("unable to set mode of ",d->dir,"/processed, pausing: ");
    close(fd);

    while ((fd = open_append("newstate")) == -1)
      pause3("unable to create ",d->dir,"/newstate, pausing: ");
    while (fsync(fd) == -1)
      pause3("unable to write ",d->dir,"/newstate to disk, pausing: ");
    close(fd);

    while (unlink("previous") == -1)
      pause3("unable to remove ",d->dir,"/previous, pausing: ");
    while (rename("newstate","state") == -1)
      pause3("unable to rename newstate to state in directory ",d->dir,", pausing: ");
    finish(d,"processed","s");
  }
}

int c_write(pos,buf,len)
int pos;
char *buf;
int len;
{
  struct cyclog *d;
  int w;

  d = c + pos;

  if (d->bytes >= d->size)
    fullcurrent(d);

  if (d->bytes + len >= d->size)
    len = d->size - d->bytes;

  if (d->bytes + len >= d->size - 2000) {
    w = byte_rchr(buf,len,'\n');
    if (w < len)
      len = w + 1;
  }

  for (;;) {
    w = write(d->fdcurrent,buf,len);
    if (w > 0) break;
    pause3("unable to write to ",d->dir,"/current, pausing: ");
  }

  d->bytes += w;
  if (d->bytes >= d->size - 2000)
    if (buf[w - 1] == '\n')
      fullcurrent(d);

  return w;
}

void restart(d)
struct cyclog *d;
{
  struct stat st;
  int fd;
  int flagprocessed;

  if (fchdir(fdstartdir) == -1)
    strerr_die2sys(111,FATAL,"unable to switch to starting directory: ");

  mkdir(d->dir,0700);
  d->fddir = open_read(d->dir);
  if ((d->fddir == -1) || (fchdir(d->fddir) == -1))
    strerr_die4sys(111,FATAL,"unable to open directory ",d->dir,": ");
  coe(d->fddir);

  d->fdlock = open_append("lock");
  if ((d->fdlock == -1) || (lock_exnb(d->fdlock) == -1))
    strerr_die4sys(111,FATAL,"unable to lock directory ",d->dir,": ");
  coe(d->fdlock);

  if (stat("current",&st) == -1) {
    if (errno != error_noent)
      strerr_die4sys(111,FATAL,"unable to stat ",d->dir,"/current: ");
  }
  else
    if (st.st_mode & 0100) {
      fd = open_append("current");
      if (fd == -1)
        strerr_die4sys(111,FATAL,"unable to append to ",d->dir,"/current: ");
      if (fchmod(fd,0644) == -1)
        strerr_die4sys(111,FATAL,"unable to set mode of ",d->dir,"/current: ");
      coe(fd);
      d->fdcurrent = fd;
      d->bytes = st.st_size;
      return;
    }

  unlink("state");
  unlink("newstate");

  flagprocessed = 0;
  if (stat("processed",&st) == -1) {
    if (errno != error_noent)
      strerr_die4sys(111,FATAL,"unable to stat ",d->dir,"/processed: ");
  }
  else if (st.st_mode & 0100)
    flagprocessed = 1;

  if (flagprocessed) {
    unlink("previous");
    finish(d,"processed","s");
  }
  else {
    unlink("processed");
    finish(d,"previous","u");
  }

  finish(d,"current","u");

  fd = open_trunc("state");
  if (fd == -1)
    strerr_die4sys(111,FATAL,"unable to write to ",d->dir,"/state: ");
  close(fd);
  fd = open_append("current");
  if (fd == -1)
    strerr_die4sys(111,FATAL,"unable to write to ",d->dir,"/current: ");
  coe(fd);
  d->fdcurrent = fd;
  d->bytes = 0;
}

void c_init(script)
char **script;
{
  int i;
  struct cyclog *d;
  char *processor;
  unsigned long num;
  unsigned long size;

  cnum = 0;
  for (i = 0;script[i];++i)
    if ((script[i][0] == '.') || (script[i][0] == '/'))
      ++cnum;

  c = (struct cyclog *) alloc(cnum * sizeof(*c));
  if (!c) strerr_die2x(111,FATAL,"out of memory");

  d = c;
  processor = 0;
  num = 10;
  size = 99999;

  for (i = 0;script[i];++i)
    if (script[i][0] == 's') {
      scan_ulong(script[i] + 1,&size);
      if (size < 4096) size = 4096;
      if (size > 16777215) size = 16777215;
    }
    else if (script[i][0] == 'n') {
      scan_ulong(script[i] + 1,&num);
      if (num < 2) num = 2;
    }
    else if (script[i][0] == '!') {
      processor = script[i] + 1;
    }
    else if ((script[i][0] == '.') || (script[i][0] == '/')) {
      d->num = num;
      d->size = size;
      d->processor = processor;
      d->dir = script[i];
      substdio_fdbuf(&d->ss,c_write,d - c,d->buf,sizeof d->buf);
      restart(d);
      ++d;
    }
}

void c_quit()
{
  int j;

  for (j = 0;j < cnum;++j) {
    substdio_flush(&c[j].ss);
    while (fsync(c[j].fdcurrent) == -1)
      pause3("unable to write ",c[j].dir,"/current to disk, pausing: ");
    while (fchmod(c[j].fdcurrent,0744) == -1)
      pause3("unable to set mode of ",c[j].dir,"/current, pausing: ");
  }
}

int flagexitasap = 0;
int flagnewline = 1;

int exitasap()
{
  flagexitasap = 1;
}

int flushread(fd,buf,len) int fd; char *buf; int len;
{
  int j;

  for (j = 0;j < cnum;++j)
    substdio_flush(&c[j].ss);

  if (!len) return 0;
  if (flagexitasap) {
    if (flagnewline) return 0;
    len = 1;
  }

  len = read(fd,buf,len);
  if (len <= 0) return len;
  flagnewline = (buf[len - 1] == '\n');
  return len;
}

char inbuf[1024];
substdio ssin = SUBSTDIO_FDBUF(flushread,0,inbuf,sizeof inbuf);

char line[1001];
int linelen;

void doit(script)
char **script;
{
  int flageof;
  char ch;
  int j;
  int i;
  char *action;
  int flagselected;
  int flagtimestamp;

  flagtimestamp = 0;
  if (script[0])
    if (script[0][0] == 't')
      flagtimestamp = 1;

  flageof = 0;
  for (;;) {
    linelen = 0;

    while (linelen < 1000) {
      if (substdio_get(&ssin,&ch,1) <= 0) {
        if (!linelen) return;
        flageof = 1;
        break;
      }
      if (!linelen)
        if (flagtimestamp) {
          timestamp(line);
          line[25] = ' ';
          linelen = 26;
        }
      if (ch == '\n')
        break;
      line[linelen++] = ch;
    }

    for (i = linelen;i <= 1000;++i)
      line[i] = '\n';

    flagselected = 1;
    j = 0;
    for (i = 0;action = script[i];++i)
      switch(*action) {
        case '+':
          if (!flagselected)
            if (match(action + 1,line,linelen))
              flagselected = 1;
          break;
        case '-':
          if (flagselected)
            if (match(action + 1,line,linelen))
              flagselected = 0;
          break;
        case 'e':
          if (flagselected) {
            if (linelen > 200) {
              substdio_put(subfderr,line,200);
              substdio_puts(subfderr,"...\n");
            }
            else {
              substdio_put(subfderr,line,linelen);
              substdio_puts(subfderr,"\n");
            }
            substdio_flush(subfderr);
          }
          break;
        case '=':
          if (flagselected)
            for (;;) {
              while (seek_begin(f[i]) == -1)
                pause3("unable to move to beginning of ",action + 1,", pausing: ");
              if (write(f[i],line,1001) == 1001)
                break;
              pause3("unable to write ",action + 1,", pausing: ");
            }
          break;
        case '.':
        case '/':
          c[j].flagselected = flagselected;
          ++j;
          break;
      }

    for (j = 0;j < cnum;++j)
      if (c[j].flagselected)
        substdio_put(&c[j].ss,line,linelen);
        
    if (linelen == 1000)
      for (;;) {
        if (substdio_get(&ssin,&ch,1) <= 0) {
          flageof = 1;
          break;
        }
        if (ch == '\n')
          break;
        for (j = 0;j < cnum;++j)
          if (c[j].flagselected)
            substdio_put(&c[j].ss,&ch,1);
      }

    for (j = 0;j < cnum;++j)
      if (c[j].flagselected)
        substdio_put(&c[j].ss,"\n",1);

    if (flageof) return;
  }
}

main(argc,argv)
int argc;
char **argv;
{
  umask(022);

  fdstartdir = open_read(".");
  if (fdstartdir == -1)
    strerr_die2sys(111,FATAL,"unable to switch to current directory: ");
  coe(fdstartdir);

  sig_termcatch(exitasap);

  ++argv;
  f_init(argv);
  c_init(argv);
  doit(argv);
  c_quit();
  _exit(0);
}
