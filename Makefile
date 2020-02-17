# Don't edit Makefile! Use conf-* for configuration.

SHELL=/bin/sh

default: it

alloc.o: \
compile alloc.c alloc.h error.h
	./compile alloc.c

alloc_re.o: \
compile alloc_re.c alloc.h byte.h
	./compile alloc_re.c

auto-str: \
load auto-str.o unix.a byte.a
	./load auto-str unix.a byte.a 

auto-str.o: \
compile auto-str.c buffer.h readwrite.h exit.h
	./compile auto-str.c

auto_home.c: \
auto-str conf-home
	./auto-str auto_home `head -1 conf-home` > auto_home.c

auto_home.o: \
compile auto_home.c
	./compile auto_home.c

buffer.o: \
compile buffer.c buffer.h
	./compile buffer.c

buffer_0.o: \
compile buffer_0.c readwrite.h buffer.h
	./compile buffer_0.c

buffer_1.o: \
compile buffer_1.c readwrite.h buffer.h
	./compile buffer_1.c

buffer_2.o: \
compile buffer_2.c readwrite.h buffer.h
	./compile buffer_2.c

buffer_copy.o: \
compile buffer_copy.c buffer.h
	./compile buffer_copy.c

buffer_get.o: \
compile buffer_get.c buffer.h byte.h error.h
	./compile buffer_get.c

buffer_put.o: \
compile buffer_put.c buffer.h str.h byte.h error.h
	./compile buffer_put.c

byte.a: \
makelib byte_chr.o byte_copy.o byte_cr.o byte_diff.o byte_rchr.o \
fmt_uint.o fmt_uint0.o fmt_ulong.o scan_ulong.o str_chr.o str_diff.o \
str_len.o str_start.o
	./makelib byte.a byte_chr.o byte_copy.o byte_cr.o \
	byte_diff.o byte_rchr.o fmt_uint.o fmt_uint0.o fmt_ulong.o \
	scan_ulong.o str_chr.o str_diff.o str_len.o str_start.o

byte_chr.o: \
compile byte_chr.c byte.h
	./compile byte_chr.c

byte_copy.o: \
compile byte_copy.c byte.h
	./compile byte_copy.c

byte_cr.o: \
compile byte_cr.c byte.h
	./compile byte_cr.c

byte_diff.o: \
compile byte_diff.c byte.h
	./compile byte_diff.c

byte_rchr.o: \
compile byte_rchr.c byte.h
	./compile byte_rchr.c

check: \
it instcheck
	./instcheck

chkshsgr: \
load chkshsgr.o
	./load chkshsgr 

chkshsgr.o: \
compile chkshsgr.c exit.h
	./compile chkshsgr.c

choose: \
warn-auto.sh choose.sh conf-home
	cat warn-auto.sh choose.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> choose
	chmod 755 choose

coe.o: \
compile coe.c coe.h
	./compile coe.c

compile: \
warn-auto.sh conf-cc
	( cat warn-auto.sh; \
	echo exec "`head -1 conf-cc`" '-c $${1+"$$@"}' \
	) > compile
	chmod 755 compile

direntry.h: \
choose compile trydrent.c direntry.h1 direntry.h2
	./choose c trydrent direntry.h1 direntry.h2 > direntry.h

env.o: \
compile env.c str.h env.h
	./compile env.c

envdir: \
load envdir.o unix.a byte.a
	./load envdir unix.a byte.a 

envdir.o: \
compile envdir.c byte.h open.h error.h direntry.h stralloc.h \
gen_alloc.h openreadclose.h stralloc.h strerr.h pathexec.h
	./compile envdir.c

envuidgid: \
load envuidgid.o unix.a byte.a
	./load envuidgid unix.a byte.a 

envuidgid.o: \
compile envuidgid.c fmt.h strerr.h pathexec.h
	./compile envuidgid.c

error.o: \
compile error.c error.h
	./compile error.c

error_str.o: \
compile error_str.c error.h
	./compile error_str.c

fd_copy.o: \
compile fd_copy.c fd.h
	./compile fd_copy.c

fd_move.o: \
compile fd_move.c fd.h
	./compile fd_move.c

fghack: \
load fghack.o unix.a byte.a
	./load fghack unix.a byte.a 

fghack.o: \
compile fghack.c fork.h wait.h error.h strerr.h readwrite.h \
pathexec.h exit.h
	./compile fghack.c

fifo.o: \
compile fifo.c hasmkffo.h fifo.h
	./compile fifo.c

fmt_uint.o: \
compile fmt_uint.c fmt.h
	./compile fmt_uint.c

fmt_uint0.o: \
compile fmt_uint0.c fmt.h
	./compile fmt_uint0.c

fmt_ulong.o: \
compile fmt_ulong.c fmt.h
	./compile fmt_ulong.c

fork.h: \
choose compile load tryvfork.c fork.h1 fork.h2
	./choose cl tryvfork fork.h1 fork.h2 > fork.h

hasflock.h: \
choose compile load tryflock.c hasflock.h1 hasflock.h2
	./choose cl tryflock hasflock.h1 hasflock.h2 > hasflock.h

hasmkffo.h: \
choose compile load trymkffo.c hasmkffo.h1 hasmkffo.h2
	./choose cl trymkffo hasmkffo.h1 hasmkffo.h2 > hasmkffo.h

hassgact.h: \
choose compile load trysgact.c hassgact.h1 hassgact.h2
	./choose cl trysgact hassgact.h1 hassgact.h2 > hassgact.h

hasshsgr.h: \
choose compile load tryshsgr.c hasshsgr.h1 hasshsgr.h2 chkshsgr \
warn-shsgr
	./chkshsgr || ( cat warn-shsgr; exit 1 )
	./choose clr tryshsgr hasshsgr.h1 hasshsgr.h2 > hasshsgr.h

haswaitp.h: \
choose compile load trywaitp.c haswaitp.h1 haswaitp.h2
	./choose cl trywaitp haswaitp.h1 haswaitp.h2 > haswaitp.h

hier.o: \
compile hier.c auto_home.h
	./compile hier.c

install: \
load install.o hier.o auto_home.o unix.a byte.a
	./load install hier.o auto_home.o unix.a byte.a 

install.o: \
compile install.c buffer.h strerr.h error.h open.h readwrite.h exit.h
	./compile install.c

instcheck: \
load instcheck.o hier.o auto_home.o unix.a byte.a
	./load instcheck hier.o auto_home.o unix.a byte.a 

instcheck.o: \
compile instcheck.c strerr.h error.h readwrite.h exit.h
	./compile instcheck.c

iopause.h: \
choose compile trypoll.c iopause.h1 iopause.h2
	./choose clr trypoll iopause.h1 iopause.h2 > iopause.h

iopause.o: \
compile iopause.c taia.h tai.h uint64.h select.h iopause.h taia.h
	./compile iopause.c

it: \
prog install instcheck

load: \
warn-auto.sh conf-ld
	( cat warn-auto.sh; \
	echo 'main="$$1"; shift'; \
	echo exec "`head -1 conf-ld`" \
	'-o "$$main" "$$main".o $${1+"$$@"}' \
	) > load
	chmod 755 load

lock_ex.o: \
compile lock_ex.c hasflock.h lock.h
	./compile lock_ex.c

lock_exnb.o: \
compile lock_exnb.c hasflock.h lock.h
	./compile lock_exnb.c

makelib: \
warn-auto.sh systype
	( cat warn-auto.sh; \
	echo 'main="$$1"; shift'; \
	echo 'rm -f "$$main"'; \
	echo 'ar cr "$$main" $${1+"$$@"}'; \
	case "`cat systype`" in \
	sunos-5.*) ;; \
	unix_sv*) ;; \
	irix64-*) ;; \
	irix-*) ;; \
	dgux-*) ;; \
	hp-ux-*) ;; \
	sco*) ;; \
	*) echo 'ranlib "$$main"' ;; \
	esac \
	) > makelib
	chmod 755 makelib

match.o: \
compile match.c match.h
	./compile match.c

matchtest: \
load matchtest.o match.o unix.a byte.a
	./load matchtest match.o unix.a byte.a 

matchtest.o: \
compile matchtest.c match.h buffer.h str.h
	./compile matchtest.c

multilog: \
load multilog.o match.o time.a unix.a byte.a
	./load multilog match.o time.a unix.a byte.a 

multilog.o: \
compile multilog.c direntry.h alloc.h exit.h buffer.h strerr.h \
error.h open.h lock.h scan.h byte.h seek.h timestamp.h fork.h wait.h \
coe.h env.h sig.h match.h
	./compile multilog.c

ndelay_off.o: \
compile ndelay_off.c ndelay.h
	./compile ndelay_off.c

ndelay_on.o: \
compile ndelay_on.c ndelay.h
	./compile ndelay_on.c

open_append.o: \
compile open_append.c open.h
	./compile open_append.c

open_read.o: \
compile open_read.c open.h
	./compile open_read.c

open_trunc.o: \
compile open_trunc.c open.h
	./compile open_trunc.c

open_write.o: \
compile open_write.c open.h
	./compile open_write.c

openreadclose.o: \
compile openreadclose.c error.h open.h readclose.h stralloc.h \
gen_alloc.h openreadclose.h stralloc.h
	./compile openreadclose.c

pathexec_env.o: \
compile pathexec_env.c stralloc.h gen_alloc.h alloc.h str.h byte.h \
env.h pathexec.h
	./compile pathexec_env.c

pathexec_run.o: \
compile pathexec_run.c error.h stralloc.h gen_alloc.h str.h env.h \
pathexec.h
	./compile pathexec_run.c

prog: \
svscan supervise svc svok svstat fghack multilog tai64n tai64nlocal \
softlimit setuidgid envuidgid envdir setlock rts matchtest

prot.o: \
compile prot.c hasshsgr.h prot.h
	./compile prot.c

readclose.o: \
compile readclose.c readwrite.h error.h readclose.h stralloc.h \
gen_alloc.h
	./compile readclose.c

rts: \
warn-auto.sh rts.sh conf-home
	cat warn-auto.sh rts.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> rts
	chmod 755 rts

scan_ulong.o: \
compile scan_ulong.c scan.h
	./compile scan_ulong.c

seek_set.o: \
compile seek_set.c seek.h
	./compile seek_set.c

select.h: \
choose compile trysysel.c select.h1 select.h2
	./choose c trysysel select.h1 select.h2 > select.h

setlock: \
load setlock.o unix.a byte.a
	./load setlock unix.a byte.a 

setlock.o: \
compile setlock.c lock.h open.h strerr.h exit.h pathexec.h sgetopt.h \
subgetopt.h
	./compile setlock.c

setuidgid: \
load setuidgid.o unix.a byte.a
	./load setuidgid unix.a byte.a 

setuidgid.o: \
compile setuidgid.c prot.h strerr.h pathexec.h
	./compile setuidgid.c

setup: \
it install
	./install

sgetopt.o: \
compile sgetopt.c buffer.h sgetopt.h subgetopt.h subgetopt.h
	./compile sgetopt.c

sig.o: \
compile sig.c sig.h
	./compile sig.c

sig_catch.o: \
compile sig_catch.c sig.h hassgact.h
	./compile sig_catch.c

softlimit: \
load softlimit.o unix.a byte.a
	./load softlimit unix.a byte.a 

softlimit.o: \
compile softlimit.c pathexec.h sgetopt.h subgetopt.h strerr.h scan.h \
str.h
	./compile softlimit.c

str_chr.o: \
compile str_chr.c str.h
	./compile str_chr.c

str_diff.o: \
compile str_diff.c str.h
	./compile str_diff.c

str_len.o: \
compile str_len.c str.h
	./compile str_len.c

str_start.o: \
compile str_start.c str.h
	./compile str_start.c

stralloc_cat.o: \
compile stralloc_cat.c byte.h stralloc.h gen_alloc.h
	./compile stralloc_cat.c

stralloc_catb.o: \
compile stralloc_catb.c stralloc.h gen_alloc.h byte.h
	./compile stralloc_catb.c

stralloc_cats.o: \
compile stralloc_cats.c byte.h str.h stralloc.h gen_alloc.h
	./compile stralloc_cats.c

stralloc_eady.o: \
compile stralloc_eady.c alloc.h stralloc.h gen_alloc.h \
gen_allocdefs.h
	./compile stralloc_eady.c

stralloc_opyb.o: \
compile stralloc_opyb.c stralloc.h gen_alloc.h byte.h
	./compile stralloc_opyb.c

stralloc_opys.o: \
compile stralloc_opys.c byte.h str.h stralloc.h gen_alloc.h
	./compile stralloc_opys.c

stralloc_pend.o: \
compile stralloc_pend.c alloc.h stralloc.h gen_alloc.h \
gen_allocdefs.h
	./compile stralloc_pend.c

strerr_die.o: \
compile strerr_die.c buffer.h exit.h strerr.h
	./compile strerr_die.c

strerr_sys.o: \
compile strerr_sys.c error.h strerr.h
	./compile strerr_sys.c

subgetopt.o: \
compile subgetopt.c subgetopt.h
	./compile subgetopt.c

supervise: \
load supervise.o time.a unix.a byte.a
	./load supervise time.a unix.a byte.a 

supervise.o: \
compile supervise.c sig.h strerr.h error.h fifo.h open.h lock.h \
fork.h wait.h coe.h ndelay.h env.h iopause.h taia.h tai.h uint64.h \
taia.h readwrite.h exit.h
	./compile supervise.c

svc: \
load svc.o unix.a byte.a
	./load svc unix.a byte.a 

svc.o: \
compile svc.c ndelay.h strerr.h error.h open.h sgetopt.h subgetopt.h \
buffer.h readwrite.h exit.h byte.h sig.h
	./compile svc.c

svok: \
load svok.o unix.a byte.a
	./load svok unix.a byte.a 

svok.o: \
compile svok.c strerr.h error.h open.h exit.h
	./compile svok.c

svscan: \
load svscan.o unix.a byte.a
	./load svscan unix.a byte.a 

svscan.o: \
compile svscan.c direntry.h strerr.h error.h fork.h wait.h coe.h fd.h \
env.h pathexec.h
	./compile svscan.c

svstat: \
load svstat.o time.a unix.a byte.a
	./load svstat time.a unix.a byte.a 

svstat.o: \
compile svstat.c strerr.h error.h open.h fmt.h tai.h uint64.h \
buffer.h readwrite.h exit.h
	./compile svstat.c

systype: \
find-systype.sh conf-cc conf-ld trycpp.c x86cpuid.c
	( cat warn-auto.sh; \
	echo CC=\'`head -1 conf-cc`\'; \
	echo LD=\'`head -1 conf-ld`\'; \
	cat find-systype.sh; \
	) | sh > systype

tai64n: \
load tai64n.o time.a unix.a byte.a
	./load tai64n time.a unix.a byte.a 

tai64n.o: \
compile tai64n.c timestamp.h buffer.h readwrite.h exit.h
	./compile tai64n.c

tai64nlocal: \
load tai64nlocal.o unix.a byte.a
	./load tai64nlocal unix.a byte.a 

tai64nlocal.o: \
compile tai64nlocal.c buffer.h exit.h fmt.h
	./compile tai64nlocal.c

tai_now.o: \
compile tai_now.c tai.h uint64.h
	./compile tai_now.c

tai_pack.o: \
compile tai_pack.c tai.h uint64.h
	./compile tai_pack.c

tai_sub.o: \
compile tai_sub.c tai.h uint64.h
	./compile tai_sub.c

tai_unpack.o: \
compile tai_unpack.c tai.h uint64.h
	./compile tai_unpack.c

taia_add.o: \
compile taia_add.c taia.h tai.h uint64.h
	./compile taia_add.c

taia_approx.o: \
compile taia_approx.c taia.h tai.h uint64.h
	./compile taia_approx.c

taia_frac.o: \
compile taia_frac.c taia.h tai.h uint64.h
	./compile taia_frac.c

taia_less.o: \
compile taia_less.c taia.h tai.h uint64.h
	./compile taia_less.c

taia_now.o: \
compile taia_now.c taia.h tai.h uint64.h
	./compile taia_now.c

taia_pack.o: \
compile taia_pack.c taia.h tai.h uint64.h
	./compile taia_pack.c

taia_sub.o: \
compile taia_sub.c taia.h tai.h uint64.h
	./compile taia_sub.c

taia_uint.o: \
compile taia_uint.c taia.h tai.h uint64.h
	./compile taia_uint.c

time.a: \
makelib iopause.o tai_now.o tai_pack.o tai_sub.o tai_unpack.o \
taia_add.o taia_approx.o taia_frac.o taia_less.o taia_now.o \
taia_pack.o taia_sub.o taia_uint.o timestamp.o
	./makelib time.a iopause.o tai_now.o tai_pack.o tai_sub.o \
	tai_unpack.o taia_add.o taia_approx.o taia_frac.o \
	taia_less.o taia_now.o taia_pack.o taia_sub.o taia_uint.o \
	timestamp.o

timestamp.o: \
compile timestamp.c taia.h tai.h uint64.h timestamp.h
	./compile timestamp.c

uint64.h: \
choose compile load tryulong64.c uint64.h1 uint64.h2
	./choose clr tryulong64 uint64.h1 uint64.h2 > uint64.h

unix.a: \
makelib alloc.o alloc_re.o buffer.o buffer_0.o buffer_1.o buffer_2.o \
buffer_copy.o buffer_get.o buffer_put.o coe.o env.o error.o \
error_str.o fd_copy.o fd_move.o fifo.o lock_ex.o lock_exnb.o \
ndelay_off.o ndelay_on.o open_append.o open_read.o open_trunc.o \
open_write.o openreadclose.o pathexec_env.o pathexec_run.o prot.o \
readclose.o seek_set.o sgetopt.o sig.o sig_catch.o stralloc_cat.o \
stralloc_catb.o stralloc_cats.o stralloc_eady.o stralloc_opyb.o \
stralloc_opys.o stralloc_pend.o strerr_die.o strerr_sys.o subgetopt.o \
wait_nohang.o wait_pid.o
	./makelib unix.a alloc.o alloc_re.o buffer.o buffer_0.o \
	buffer_1.o buffer_2.o buffer_copy.o buffer_get.o \
	buffer_put.o coe.o env.o error.o error_str.o fd_copy.o \
	fd_move.o fifo.o lock_ex.o lock_exnb.o ndelay_off.o \
	ndelay_on.o open_append.o open_read.o open_trunc.o \
	open_write.o openreadclose.o pathexec_env.o pathexec_run.o \
	prot.o readclose.o seek_set.o sgetopt.o sig.o sig_catch.o \
	stralloc_cat.o stralloc_catb.o stralloc_cats.o \
	stralloc_eady.o stralloc_opyb.o stralloc_opys.o \
	stralloc_pend.o strerr_die.o strerr_sys.o subgetopt.o \
	wait_nohang.o wait_pid.o

wait_nohang.o: \
compile wait_nohang.c haswaitp.h
	./compile wait_nohang.c

wait_pid.o: \
compile wait_pid.c error.h haswaitp.h
	./compile wait_pid.c
