#include "auto_home.h"

void hier()
{
  h(auto_home,-1,-1,02755);

  d(auto_home,"bin",-1,-1,02755);

  c(auto_home,"bin","setuidgid",-1,-1,0700);
  c(auto_home,"bin","envuidgid",-1,-1,0755);
  c(auto_home,"bin","svscan",-1,-1,0755);
  c(auto_home,"bin","supervise",-1,-1,0755);
  c(auto_home,"bin","svok",-1,-1,0755);
  c(auto_home,"bin","svstat",-1,-1,0755);
  c(auto_home,"bin","svc",-1,-1,0755);
  c(auto_home,"bin","fghack",-1,-1,0755);
  c(auto_home,"bin","multilog",-1,-1,0755);
  c(auto_home,"bin","tai64n",-1,-1,0755);
  c(auto_home,"bin","tai64nlocal",-1,-1,0755);
  c(auto_home,"bin","softlimit",-1,-1,0755);
}
