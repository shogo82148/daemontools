# Don't edit Makefile! Use conf-* for configuration.

SHELL=/bin/sh

default: it

alloc.a: \
makelib alloc.o alloc_re.o
	./makelib alloc.a alloc.o alloc_re.o

alloc.o: \
compile alloc.c alloc.h error.h
	./compile alloc.c

alloc_re.o: \
compile alloc_re.c alloc.h byte.h
	./compile alloc_re.c

auto-str: \
load auto-str.o substdio.a error.a str.a
	./load auto-str substdio.a error.a str.a 

auto-str.o: \
compile auto-str.c substdio.h readwrite.h exit.h
	./compile auto-str.c

auto_home.c: \
auto-str conf-home
	./auto-str auto_home `head -1 conf-home` > auto_home.c

auto_home.o: \
compile auto_home.c
	./compile auto_home.c

byte_chr.o: \
compile byte_chr.c byte.h
	./compile byte_chr.c

byte_copy.o: \
compile byte_copy.c byte.h
	./compile byte_copy.c

byte_cr.o: \
compile byte_cr.c byte.h
	./compile byte_cr.c

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

env.a: \
makelib env.o envread.o
	./makelib env.a env.o envread.o

env.o: \
compile env.c str.h alloc.h env.h
	./compile env.c

envread.o: \
compile envread.c env.h str.h
	./compile envread.c

envuidgid: \
load envuidgid.o env.a alloc.a strerr.a substdio.a error.a str.a fs.a
	./load envuidgid env.a alloc.a strerr.a substdio.a error.a \
	str.a fs.a 

envuidgid.o: \
compile envuidgid.c strerr.h fmt.h env.h
	./compile envuidgid.c

error.a: \
makelib error.o error_str.o
	./makelib error.a error.o error_str.o

error.o: \
compile error.c error.h
	./compile error.c

error_str.o: \
compile error_str.c error.h
	./compile error_str.c

fd.a: \
makelib fd_copy.o fd_move.o
	./makelib fd.a fd_copy.o fd_move.o

fd_copy.o: \
compile fd_copy.c fd.h
	./compile fd_copy.c

fd_move.o: \
compile fd_move.c fd.h
	./compile fd_move.c

fghack: \
load fghack.o strerr.a substdio.a error.a wait.a str.a
	./load fghack strerr.a substdio.a error.a wait.a str.a 

fghack.o: \
compile fghack.c fork.h wait.h error.h strerr.h readwrite.h exit.h
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

fs.a: \
makelib fmt_uint.o fmt_uint0.o fmt_ulong.o scan_ulong.o
	./makelib fs.a fmt_uint.o fmt_uint0.o fmt_ulong.o \
	scan_ulong.o

getopt.a: \
makelib subgetopt.o sgetopt.o
	./makelib getopt.a subgetopt.o sgetopt.o

hasflock.h: \
choose compile load tryflock.c hasflock.h1 hasflock.h2
	./choose cl tryflock hasflock.h1 hasflock.h2 > hasflock.h

hasmkffo.h: \
choose compile load trymkffo.c hasmkffo.h1 hasmkffo.h2
	./choose cl trymkffo hasmkffo.h1 hasmkffo.h2 > hasmkffo.h

hassgact.h: \
choose compile load trysgact.c hassgact.h1 hassgact.h2
	./choose cl trysgact hassgact.h1 hassgact.h2 > hassgact.h

hassgprm.h: \
choose compile load trysgprm.c hassgprm.h1 hassgprm.h2
	./choose cl trysgprm hassgprm.h1 hassgprm.h2 > hassgprm.h

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
load install.o hier.o auto_home.o strerr.a substdio.a open.a error.a \
str.a
	./load install hier.o auto_home.o strerr.a substdio.a \
	open.a error.a str.a 

install.o: \
compile install.c substdio.h strerr.h error.h open.h readwrite.h \
exit.h
	./compile install.c

instcheck: \
load instcheck.o hier.o auto_home.o strerr.a substdio.a error.a str.a
	./load instcheck hier.o auto_home.o strerr.a substdio.a \
	error.a str.a 

instcheck.o: \
compile instcheck.c strerr.h error.h readwrite.h exit.h
	./compile instcheck.c

it: \
prog install instcheck

libtai.a: \
makelib tai_now.o tai_pack.o tai_unpack.o taia_now.o taia_pack.o \
taia_unpack.o
	./makelib libtai.a tai_now.o tai_pack.o tai_unpack.o \
	taia_now.o taia_pack.o taia_unpack.o

load: \
warn-auto.sh conf-ld
	( cat warn-auto.sh; \
	echo 'main="$$1"; shift'; \
	echo exec "`head -1 conf-ld`" \
	'-o "$$main" "$$main".o $${1+"$$@"}' \
	) > load
	chmod 755 load

lock.a: \
makelib lock_exnb.o
	./makelib lock.a lock_exnb.o

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
load matchtest.o match.o substdio.a error.a str.a
	./load matchtest match.o substdio.a error.a str.a 

matchtest.o: \
compile matchtest.c match.h substdio.h subfd.h substdio.h str.h
	./compile matchtest.c

multilog: \
load multilog.o match.o coe.o timestamp.o libtai.a strerr.a alloc.a \
substdio.a error.a sig.a open.a wait.a lock.a seek.a fd.a fs.a str.a
	./load multilog match.o coe.o timestamp.o libtai.a \
	strerr.a alloc.a substdio.a error.a sig.a open.a wait.a \
	lock.a seek.a fd.a fs.a str.a 

multilog.o: \
compile multilog.c direntry.h alloc.h exit.h substdio.h subfd.h \
substdio.h strerr.h error.h open.h lock.h scan.h byte.h seek.h \
timestamp.h fork.h wait.h coe.h sig.h match.h
	./compile multilog.c

ndelay.a: \
makelib ndelay.o ndelay_off.o
	./makelib ndelay.a ndelay.o ndelay_off.o

ndelay.o: \
compile ndelay.c ndelay.h
	./compile ndelay.c

ndelay_off.o: \
compile ndelay_off.c ndelay.h
	./compile ndelay_off.c

open.a: \
makelib open_append.o open_read.o open_trunc.o open_write.o
	./makelib open.a open_append.o open_read.o open_trunc.o \
	open_write.o

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

prog: \
svscan supervise svok svstat svc fghack multilog tai64n tai64nlocal \
softlimit setuidgid envuidgid rts matchtest

prot.o: \
compile prot.c hasshsgr.h prot.h
	./compile prot.c

rts: \
warn-auto.sh rts.sh conf-home
	cat warn-auto.sh rts.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> rts
	chmod 755 rts

scan_ulong.o: \
compile scan_ulong.c scan.h
	./compile scan_ulong.c

seek.a: \
makelib seek_set.o
	./makelib seek.a seek_set.o

seek_set.o: \
compile seek_set.c seek.h
	./compile seek_set.c

select.h: \
choose compile trysysel.c select.h1 select.h2
	./choose c trysysel select.h1 select.h2 > select.h

setuidgid: \
load setuidgid.o prot.o strerr.a substdio.a error.a str.a
	./load setuidgid prot.o strerr.a substdio.a error.a str.a 

setuidgid.o: \
compile setuidgid.c strerr.h prot.h
	./compile setuidgid.c

setup: \
it install
	./install

sgetopt.o: \
compile sgetopt.c substdio.h subfd.h substdio.h sgetopt.h subgetopt.h \
subgetopt.h
	./compile sgetopt.c

sig.a: \
makelib sig_block.o sig_catch.o sig_pause.o sig_pipe.o sig_child.o \
sig_term.o
	./makelib sig.a sig_block.o sig_catch.o sig_pause.o \
	sig_pipe.o sig_child.o sig_term.o

sig_block.o: \
compile sig_block.c sig.h hassgprm.h
	./compile sig_block.c

sig_catch.o: \
compile sig_catch.c sig.h hassgact.h
	./compile sig_catch.c

sig_child.o: \
compile sig_child.c sig.h
	./compile sig_child.c

sig_pause.o: \
compile sig_pause.c sig.h hassgprm.h
	./compile sig_pause.c

sig_pipe.o: \
compile sig_pipe.c sig.h
	./compile sig_pipe.c

sig_term.o: \
compile sig_term.c sig.h
	./compile sig_term.c

softlimit: \
load softlimit.o getopt.a strerr.a substdio.a error.a str.a fs.a
	./load softlimit getopt.a strerr.a substdio.a error.a \
	str.a fs.a 

softlimit.o: \
compile softlimit.c sgetopt.h subgetopt.h strerr.h scan.h str.h
	./compile softlimit.c

str.a: \
makelib str_len.o str_diff.o str_diffn.o str_cpy.o str_start.o \
byte_chr.o byte_rchr.o byte_copy.o byte_cr.o
	./makelib str.a str_len.o str_diff.o str_diffn.o str_cpy.o \
	str_start.o byte_chr.o byte_rchr.o byte_copy.o byte_cr.o

str_cpy.o: \
compile str_cpy.c str.h
	./compile str_cpy.c

str_diff.o: \
compile str_diff.c str.h
	./compile str_diff.c

str_diffn.o: \
compile str_diffn.c str.h
	./compile str_diffn.c

str_len.o: \
compile str_len.c str.h
	./compile str_len.c

str_start.o: \
compile str_start.c str.h
	./compile str_start.c

strerr.a: \
makelib strerr_sys.o strerr_die.o
	./makelib strerr.a strerr_sys.o strerr_die.o

strerr_die.o: \
compile strerr_die.c substdio.h subfd.h substdio.h exit.h strerr.h
	./compile strerr_die.c

strerr_sys.o: \
compile strerr_sys.c error.h strerr.h
	./compile strerr_sys.c

subfderr.o: \
compile subfderr.c readwrite.h substdio.h subfd.h substdio.h
	./compile subfderr.c

subfdin.o: \
compile subfdin.c readwrite.h substdio.h subfd.h substdio.h
	./compile subfdin.c

subfdins.o: \
compile subfdins.c readwrite.h substdio.h subfd.h substdio.h
	./compile subfdins.c

subfdout.o: \
compile subfdout.c readwrite.h substdio.h subfd.h substdio.h
	./compile subfdout.c

subfdouts.o: \
compile subfdouts.c readwrite.h substdio.h subfd.h substdio.h
	./compile subfdouts.c

subgetopt.o: \
compile subgetopt.c subgetopt.h
	./compile subgetopt.c

substdi.o: \
compile substdi.c substdio.h byte.h error.h
	./compile substdi.c

substdio.a: \
makelib substdio.o substdi.o substdo.o subfderr.o subfdout.o \
subfdouts.o subfdin.o subfdins.o substdio_copy.o
	./makelib substdio.a substdio.o substdi.o substdo.o \
	subfderr.o subfdout.o subfdouts.o subfdin.o subfdins.o \
	substdio_copy.o

substdio.o: \
compile substdio.c substdio.h
	./compile substdio.c

substdio_copy.o: \
compile substdio_copy.c substdio.h
	./compile substdio_copy.c

substdo.o: \
compile substdo.c substdio.h str.h byte.h error.h
	./compile substdo.c

supervise: \
load supervise.o coe.o fifo.o sig.a libtai.a strerr.a substdio.a \
error.a ndelay.a wait.a open.a lock.a str.a
	./load supervise coe.o fifo.o sig.a libtai.a strerr.a \
	substdio.a error.a ndelay.a wait.a open.a lock.a str.a 

supervise.o: \
compile supervise.c sig.h strerr.h error.h fifo.h open.h lock.h \
fork.h wait.h coe.h ndelay.h select.h taia.h tai.h uint64.h \
readwrite.h exit.h
	./compile supervise.c

svc: \
load svc.o sig.a getopt.a strerr.a substdio.a error.a open.a ndelay.a \
str.a
	./load svc sig.a getopt.a strerr.a substdio.a error.a \
	open.a ndelay.a str.a 

svc.o: \
compile svc.c ndelay.h strerr.h error.h open.h sgetopt.h subgetopt.h \
substdio.h readwrite.h exit.h byte.h sig.h
	./compile svc.c

svok: \
load svok.o strerr.a error.a substdio.a open.a str.a
	./load svok strerr.a error.a substdio.a open.a str.a 

svok.o: \
compile svok.c strerr.h error.h open.h exit.h
	./compile svok.c

svscan: \
load svscan.o coe.o strerr.a substdio.a error.a open.a wait.a fd.a \
str.a
	./load svscan coe.o strerr.a substdio.a error.a open.a \
	wait.a fd.a str.a 

svscan.o: \
compile svscan.c direntry.h strerr.h error.h open.h fork.h wait.h \
coe.h fd.h exit.h str.h
	./compile svscan.c

svstat: \
load svstat.o strerr.a substdio.a error.a str.a open.a fs.a
	./load svstat strerr.a substdio.a error.a str.a open.a \
	fs.a 

svstat.o: \
compile svstat.c strerr.h error.h open.h fmt.h taia.h tai.h uint64.h \
substdio.h readwrite.h exit.h
	./compile svstat.c

systype: \
find-systype.sh conf-cc conf-ld trycpp.c
	( cat warn-auto.sh; \
	echo CC=\'`head -1 conf-cc`\'; \
	echo LD=\'`head -1 conf-ld`\'; \
	cat find-systype.sh; \
	) | sh > systype

tai64n: \
load tai64n.o timestamp.o substdio.a error.a str.a libtai.a
	./load tai64n timestamp.o substdio.a error.a str.a \
	libtai.a 

tai64n.o: \
compile tai64n.c timestamp.h substdio.h readwrite.h exit.h
	./compile tai64n.c

tai64nlocal: \
load tai64nlocal.o substdio.a error.a str.a fs.a
	./load tai64nlocal substdio.a error.a str.a fs.a 

tai64nlocal.o: \
compile tai64nlocal.c substdio.h subfd.h substdio.h exit.h fmt.h
	./compile tai64nlocal.c

tai_now.o: \
compile tai_now.c tai.h uint64.h
	./compile tai_now.c

tai_pack.o: \
compile tai_pack.c tai.h uint64.h
	./compile tai_pack.c

tai_unpack.o: \
compile tai_unpack.c tai.h uint64.h
	./compile tai_unpack.c

taia_now.o: \
compile taia_now.c taia.h tai.h uint64.h
	./compile taia_now.c

taia_pack.o: \
compile taia_pack.c taia.h tai.h uint64.h
	./compile taia_pack.c

taia_unpack.o: \
compile taia_unpack.c taia.h tai.h uint64.h
	./compile taia_unpack.c

timestamp.o: \
compile timestamp.c timestamp.h taia.h tai.h uint64.h
	./compile timestamp.c

uint64.h: \
choose compile load tryulong64.c uint64.h1 uint64.h2
	./choose clr tryulong64 uint64.h1 uint64.h2 > uint64.h

wait.a: \
makelib wait_pid.o wait_nohang.o
	./makelib wait.a wait_pid.o wait_nohang.o

wait_nohang.o: \
compile wait_nohang.c haswaitp.h
	./compile wait_nohang.c

wait_pid.o: \
compile wait_pid.c error.h haswaitp.h
	./compile wait_pid.c
