SHELL=/bin/sh

default: it

accustamp: \
load accustamp.o substdio.a error.a str.a fs.a
	./load accustamp substdio.a error.a str.a fs.a 

accustamp.0: \
accustamp.1
	nroff -man accustamp.1 > accustamp.0

accustamp.o: \
compile accustamp.c accustamp.c accustamp.c substdio.h accustamp.c \
fmt.h accustamp.c readwrite.h accustamp.c exit.h accustamp.c
	./compile accustamp.c

alloc.a: \
makelib alloc.o alloc_re.o
	./makelib alloc.a alloc.o alloc_re.o

alloc.o: \
compile alloc.c alloc.h alloc.c error.h alloc.c
	./compile alloc.c

alloc_re.o: \
compile alloc_re.c alloc.h alloc_re.c byte.h alloc_re.c
	./compile alloc_re.c

auto-ccld.sh: \
conf-cc conf-ld warn-auto.sh
	( cat warn-auto.sh; \
	echo CC=\'`head -1 conf-cc`\'; \
	echo LD=\'`head -1 conf-ld`\' \
	) > auto-ccld.sh

byte_chr.o: \
compile byte_chr.c byte.h byte_chr.c
	./compile byte_chr.c

byte_copy.o: \
compile byte_copy.c byte.h byte_copy.c
	./compile byte_copy.c

byte_cr.o: \
compile byte_cr.c byte.h byte_cr.c
	./compile byte_cr.c

check: \
it man instcheck conf-bin conf-man
	./instcheck "`head -1 conf-bin`" < BIN
	./instcheck "`head -1 conf-man`" < MAN

chkshsgr: \
load chkshsgr.o
	./load chkshsgr 

chkshsgr.o: \
compile chkshsgr.c exit.h chkshsgr.c
	./compile chkshsgr.c

coe.o: \
compile coe.c coe.c coe.h coe.c
	./compile coe.c

compile: \
make-compile warn-auto.sh systype
	( cat warn-auto.sh; ./make-compile "`cat systype`" ) > \
	compile
	chmod 755 compile

cyclog: \
load cyclog.o now.o open.a getopt.a strerr.a substdio.a error.a str.a \
fs.a
	./load cyclog now.o open.a getopt.a strerr.a substdio.a \
	error.a str.a fs.a 

cyclog.0: \
cyclog.1
	nroff -man cyclog.1 > cyclog.0

cyclog.o: \
compile cyclog.c cyclog.c cyclog.c direntry.h direntry.h direntry.h \
cyclog.c substdio.h cyclog.c sgetopt.h subgetopt.h sgetopt.h cyclog.c \
strerr.h cyclog.c scan.h cyclog.c fmt.h cyclog.c now.h datetime.h \
now.h cyclog.c
	./compile cyclog.c

direntry.h: \
compile trydrent.c direntry.h1 direntry.h2
	( ./compile trydrent.c >/dev/null 2>&1 \
	&& cat direntry.h2 || cat direntry.h1 ) > direntry.h
	rm -f trydrent.o

error.a: \
makelib error.o error_str.o
	./makelib error.a error.o error_str.o

error.o: \
compile error.c error.c error.h error.c
	./compile error.c

error_str.o: \
compile error_str.c error_str.c error.h error_str.c
	./compile error_str.c

errorsto: \
load errorsto.o ndelay.a open.a fd.a
	./load errorsto ndelay.a open.a fd.a 

errorsto.0: \
errorsto.1
	nroff -man errorsto.1 > errorsto.0

errorsto.o: \
compile errorsto.c open.h errorsto.c fd.h errorsto.c exit.h \
errorsto.c ndelay.h errorsto.c
	./compile errorsto.c

fd.a: \
makelib fd_copy.o fd_move.o
	./makelib fd.a fd_copy.o fd_move.o

fd_copy.o: \
compile fd_copy.c fd_copy.c fd.h fd_copy.c
	./compile fd_copy.c

fd_move.o: \
compile fd_move.c fd.h fd_move.c
	./compile fd_move.c

fifo: \
load fifo.o fifomain.o ndelay.a open.a strerr.a substdio.a error.a \
str.a
	./load fifo fifomain.o ndelay.a open.a strerr.a substdio.a \
	error.a str.a 

fifo.0: \
fifo.1
	nroff -man fifo.1 > fifo.0

fifo.o: \
compile fifo.c fifo.c fifo.c hasmkffo.h fifo.c fifo.h fifo.c
	./compile fifo.c

fifomain.o: \
compile fifomain.c fifomain.c fifomain.c fifo.h fifomain.c open.h \
fifomain.c strerr.h fifomain.c error.h fifomain.c substdio.h \
fifomain.c readwrite.h fifomain.c ndelay.h fifomain.c
	./compile fifomain.c

find-systype: \
find-systype.sh auto-ccld.sh
	cat auto-ccld.sh find-systype.sh > find-systype
	chmod 755 find-systype

fmt_str.o: \
compile fmt_str.c fmt.h fmt_str.c
	./compile fmt_str.c

fmt_uint.o: \
compile fmt_uint.c fmt.h fmt_uint.c
	./compile fmt_uint.c

fmt_uint0.o: \
compile fmt_uint0.c fmt.h fmt_uint0.c
	./compile fmt_uint0.c

fmt_ulong.o: \
compile fmt_ulong.c fmt.h fmt_ulong.c
	./compile fmt_ulong.c

fork.h: \
compile load tryvfork.c fork.h1 fork.h2
	( ( ./compile tryvfork.c && ./load tryvfork ) >/dev/null \
	2>&1 \
	&& cat fork.h2 || cat fork.h1 ) > fork.h
	rm -f tryvfork.o tryvfork

fs.a: \
makelib fmt_str.o fmt_uint.o fmt_uint0.o fmt_ulong.o scan_ulong.o \
scan_8long.o
	./makelib fs.a fmt_str.o fmt_uint.o fmt_uint0.o \
	fmt_ulong.o scan_ulong.o scan_8long.o

getln.a: \
makelib getln.o getln2.o
	./makelib getln.a getln.o getln2.o

getln.o: \
compile getln.c substdio.h getln.c byte.h getln.c stralloc.h \
gen_alloc.h stralloc.h getln.c getln.h getln.c
	./compile getln.c

getln2.o: \
compile getln2.c substdio.h getln2.c stralloc.h gen_alloc.h \
stralloc.h getln2.c byte.h getln2.c getln.h getln2.c
	./compile getln2.c

getopt.a: \
makelib subgetopt.o sgetopt.o
	./makelib getopt.a subgetopt.o sgetopt.o

hasflock.h: \
tryflock.c compile load
	( ( ./compile tryflock.c && ./load tryflock ) >/dev/null \
	2>&1 \
	&& echo \#define HASFLOCK 1 || exit 0 ) > hasflock.h
	rm -f tryflock.o tryflock

hasmkffo.h: \
trymkffo.c compile load
	( ( ./compile trymkffo.c && ./load trymkffo ) >/dev/null \
	2>&1 \
	&& echo \#define HASMKFIFO 1 || exit 0 ) > hasmkffo.h
	rm -f trymkffo.o trymkffo

hassgact.h: \
trysgact.c compile load
	( ( ./compile trysgact.c && ./load trysgact ) >/dev/null \
	2>&1 \
	&& echo \#define HASSIGACTION 1 || exit 0 ) > hassgact.h
	rm -f trysgact.o trysgact

hassgprm.h: \
trysgprm.c compile load
	( ( ./compile trysgprm.c && ./load trysgprm ) >/dev/null \
	2>&1 \
	&& echo \#define HASSIGPROCMASK 1 || exit 0 ) > hassgprm.h
	rm -f trysgprm.o trysgprm

hasshsgr.h: \
chkshsgr warn-shsgr tryshsgr.c compile load
	./chkshsgr || ( cat warn-shsgr; exit 1 )
	( ( ./compile tryshsgr.c \
	&& ./load tryshsgr && ./tryshsgr ) >/dev/null 2>&1 \
	&& echo \#define HASSHORTSETGROUPS 1 || exit 0 ) > \
	hasshsgr.h
	rm -f tryshsgr.o tryshsgr

haswaitp.h: \
trywaitp.c compile load
	( ( ./compile trywaitp.c && ./load trywaitp ) >/dev/null \
	2>&1 \
	&& echo \#define HASWAITPID 1 || exit 0 ) > haswaitp.h
	rm -f trywaitp.o trywaitp

install: \
load install.o getln.a strerr.a substdio.a stralloc.a alloc.a open.a \
error.a str.a fs.a
	./load install getln.a strerr.a substdio.a stralloc.a \
	alloc.a open.a error.a str.a fs.a 

install.o: \
compile install.c substdio.h install.c stralloc.h gen_alloc.h \
stralloc.h install.c getln.h install.c readwrite.h install.c exit.h \
install.c open.h install.c error.h install.c strerr.h install.c \
byte.h install.c
	./compile install.c

instcheck: \
load instcheck.o getln.a strerr.a substdio.a stralloc.a alloc.a \
error.a str.a fs.a
	./load instcheck getln.a strerr.a substdio.a stralloc.a \
	alloc.a error.a str.a fs.a 

instcheck.o: \
compile instcheck.c instcheck.c instcheck.c substdio.h instcheck.c \
stralloc.h gen_alloc.h stralloc.h instcheck.c getln.h instcheck.c \
readwrite.h instcheck.c exit.h instcheck.c error.h instcheck.c \
strerr.h instcheck.c byte.h instcheck.c
	./compile instcheck.c

it: \
supervise svc svstat testfilelock cyclog fifo errorsto usually \
accustamp tailocal setuser

load: \
make-load warn-auto.sh systype
	( cat warn-auto.sh; ./make-load "`cat systype`" ) > load
	chmod 755 load

lock.a: \
makelib lock_exnb.o
	./makelib lock.a lock_exnb.o

lock_exnb.o: \
compile lock_exnb.c lock_exnb.c lock_exnb.c lock_exnb.c hasflock.h \
lock_exnb.c lock.h lock_exnb.c
	./compile lock_exnb.c

make-compile: \
make-compile.sh auto-ccld.sh
	cat auto-ccld.sh make-compile.sh > make-compile
	chmod 755 make-compile

make-load: \
make-load.sh auto-ccld.sh
	cat auto-ccld.sh make-load.sh > make-load
	chmod 755 make-load

make-makelib: \
make-makelib.sh auto-ccld.sh
	cat auto-ccld.sh make-makelib.sh > make-makelib
	chmod 755 make-makelib

makelib: \
make-makelib warn-auto.sh systype
	( cat warn-auto.sh; ./make-makelib "`cat systype`" ) > \
	makelib
	chmod 755 makelib

man: \
supervise.0 svc.0 svstat.0 testfilelock.0 cyclog.0 fifo.0 errorsto.0 \
usually.0 accustamp.0 tailocal.0 setuser.0

ndelay.a: \
makelib ndelay.o ndelay_off.o
	./makelib ndelay.a ndelay.o ndelay_off.o

ndelay.o: \
compile ndelay.c ndelay.c ndelay.c ndelay.h ndelay.c
	./compile ndelay.c

ndelay_off.o: \
compile ndelay_off.c ndelay_off.c ndelay_off.c ndelay.h ndelay_off.c
	./compile ndelay_off.c

now.o: \
compile now.c now.c datetime.h now.c now.h datetime.h datetime.h \
now.h now.c
	./compile now.c

open.a: \
makelib open_append.o open_excl.o open_read.o open_trunc.o \
open_write.o
	./makelib open.a open_append.o open_excl.o open_read.o \
	open_trunc.o open_write.o

open_append.o: \
compile open_append.c open_append.c open_append.c open.h \
open_append.c
	./compile open_append.c

open_excl.o: \
compile open_excl.c open_excl.c open_excl.c open.h open_excl.c
	./compile open_excl.c

open_read.o: \
compile open_read.c open_read.c open_read.c open.h open_read.c
	./compile open_read.c

open_trunc.o: \
compile open_trunc.c open_trunc.c open_trunc.c open.h open_trunc.c
	./compile open_trunc.c

open_write.o: \
compile open_write.c open_write.c open_write.c open.h open_write.c
	./compile open_write.c

prot.o: \
compile prot.c hasshsgr.h prot.c prot.h prot.c
	./compile prot.c

scan_8long.o: \
compile scan_8long.c scan.h scan_8long.c
	./compile scan_8long.c

scan_ulong.o: \
compile scan_ulong.c scan.h scan_ulong.c
	./compile scan_ulong.c

select.h: \
compile trysysel.c select.h1 select.h2
	( ./compile trysysel.c >/dev/null 2>&1 \
	&& cat select.h2 || cat select.h1 ) > select.h
	rm -f trysysel.o trysysel

setup: \
it man install conf-bin conf-man
	./install "`head -1 conf-bin`" < BIN
	./install "`head -1 conf-man`" < MAN

setuser: \
load setuser.o prot.o strerr.a substdio.a error.a str.a fs.a
	./load setuser prot.o strerr.a substdio.a error.a str.a \
	fs.a 

setuser.0: \
setuser.1
	nroff -man setuser.1 > setuser.0

setuser.o: \
compile setuser.c setuser.c setuser.c strerr.h setuser.c scan.h \
setuser.c
	./compile setuser.c

sgetopt.o: \
compile sgetopt.c substdio.h sgetopt.c subfd.h substdio.h substdio.h \
subfd.h sgetopt.c sgetopt.h sgetopt.h subgetopt.h sgetopt.h sgetopt.c \
subgetopt.h subgetopt.h sgetopt.c
	./compile sgetopt.c

shar: \
FILES BLURB README TODO THANKS CHANGES FILES BIN MAN VERSION SYSDEPS \
Makefile supervise.c supervise.1 svc.c svc.1 accustamp.c accustamp.1 \
tailocal.c tailocal.1 cyclog.c cyclog.1 setuser.c setuser.1 conf-cc \
conf-ld find-systype.sh make-compile.sh make-load.sh make-makelib.sh \
trycpp.c warn-auto.sh INSTALL conf-bin conf-man auto-str.c auto-int.c \
auto-int8.c auto-uid.c auto-gid.c install.c instcheck.c substdio.h \
substdio.c substdi.c substdo.c substdio_copy.c subfd.h subfderr.c \
subfdouts.c subfdout.c subfdins.c subfdin.c readwrite.h exit.h \
gen_alloc.h gen_allocdefs.h stralloc.3 stralloc.h stralloc_eady.c \
stralloc_pend.c stralloc_copy.c stralloc_opyb.c stralloc_opys.c \
stralloc_cat.c stralloc_catb.c stralloc_cats.c stralloc_arts.c \
getln.3 getln.h getln.c getln2.3 getln2.c open.h open_append.c \
open_excl.c open_read.c open_trunc.c open_write.c strerr.h \
strerr_sys.c strerr_die.c byte.h byte_chr.c byte_copy.c byte_cr.c \
str.h str_len.c error.3 error_str.3 error.h error.c error_str.c \
alloc.3 alloc.h alloc.c alloc_re.c fmt.h fmt_str.c fmt_uint.c \
fmt_uint0.c fmt_ulong.c scan.h scan_ulong.c scan_8long.c fifo_make.3 \
fifo.h fifo.c trymkffo.c sgetopt.3 sgetopt.h sgetopt.c subgetopt.3 \
subgetopt.h subgetopt.c lock.h lock_exnb.c tryflock.c coe.3 coe.h \
coe.c fork.h1 fork.h2 tryvfork.c ndelay.h ndelay.c ndelay_off.c \
select.h1 select.h2 trysysel.c sig.h sig_block.c sig_catch.c \
sig_child.c sig_pipe.c trysgact.c trysgprm.c wait.3 wait.h \
wait_nohang.c trywaitp.c now.3 now.h now.c datetime.3 datetime.h \
direntry.3 direntry.h1 direntry.h2 trydrent.c prot.h prot.c \
chkshsgr.c warn-shsgr tryshsgr.c
	shar -m `cat FILES` > shar
	chmod 400 shar

sig.a: \
makelib sig_hup.o sig_block.o sig_catch.o sig_child.o sig_pipe.o
	./makelib sig.a sig_hup.o sig_block.o sig_catch.o \
	sig_child.o sig_pipe.o

sig_alarm.o: \
compile sig_alarm.c sig_alarm.c sig.h sig_alarm.c
	./compile sig_alarm.c

sig_block.o: \
compile sig_block.c sig_block.c sig.h sig_block.c hassgprm.h \
sig_block.c
	./compile sig_block.c

sig_catch.o: \
compile sig_catch.c sig_catch.c sig.h sig_catch.c hassgact.h \
sig_catch.c
	./compile sig_catch.c

sig_child.o: \
compile sig_child.c sig_child.c sig.h sig_child.c
	./compile sig_child.c

sig_hup.o: \
compile sig_hup.c sig_hup.c sig.h sig_hup.c
	./compile sig_hup.c

sig_pipe.o: \
compile sig_pipe.c sig_pipe.c sig.h sig_pipe.c
	./compile sig_pipe.c

slurp.o: \
compile slurp.c stralloc.h gen_alloc.h stralloc.h slurp.c slurp.h \
slurp.c error.h slurp.c open.h slurp.c
	./compile slurp.c

slurpclose.o: \
compile slurpclose.c stralloc.h gen_alloc.h stralloc.h slurpclose.c \
readwrite.h slurpclose.c slurpclose.h slurpclose.c error.h \
slurpclose.c
	./compile slurpclose.c

str.a: \
makelib str_len.o byte_chr.o byte_copy.o byte_cr.o
	./makelib str.a str_len.o byte_chr.o byte_copy.o byte_cr.o

str_len.o: \
compile str_len.c str.h str_len.c
	./compile str_len.c

stralloc.a: \
makelib stralloc_eady.o stralloc_pend.o stralloc_copy.o \
stralloc_opys.o stralloc_opyb.o stralloc_cat.o stralloc_cats.o \
stralloc_catb.o stralloc_arts.o
	./makelib stralloc.a stralloc_eady.o stralloc_pend.o \
	stralloc_copy.o stralloc_opys.o stralloc_opyb.o \
	stralloc_cat.o stralloc_cats.o stralloc_catb.o \
	stralloc_arts.o

stralloc_arts.o: \
compile stralloc_arts.c byte.h stralloc_arts.c str.h stralloc_arts.c \
stralloc.h gen_alloc.h stralloc.h stralloc_arts.c
	./compile stralloc_arts.c

stralloc_cat.o: \
compile stralloc_cat.c byte.h stralloc_cat.c stralloc.h gen_alloc.h \
stralloc.h stralloc_cat.c
	./compile stralloc_cat.c

stralloc_catb.o: \
compile stralloc_catb.c stralloc.h gen_alloc.h stralloc.h \
stralloc_catb.c byte.h stralloc_catb.c
	./compile stralloc_catb.c

stralloc_cats.o: \
compile stralloc_cats.c byte.h stralloc_cats.c str.h stralloc_cats.c \
stralloc.h gen_alloc.h stralloc.h stralloc_cats.c
	./compile stralloc_cats.c

stralloc_copy.o: \
compile stralloc_copy.c byte.h stralloc_copy.c stralloc.h gen_alloc.h \
stralloc.h stralloc_copy.c
	./compile stralloc_copy.c

stralloc_eady.o: \
compile stralloc_eady.c alloc.h stralloc_eady.c stralloc.h \
gen_alloc.h stralloc.h stralloc_eady.c gen_allocdefs.h \
gen_allocdefs.h gen_allocdefs.h stralloc_eady.c
	./compile stralloc_eady.c

stralloc_opyb.o: \
compile stralloc_opyb.c stralloc.h gen_alloc.h stralloc.h \
stralloc_opyb.c byte.h stralloc_opyb.c
	./compile stralloc_opyb.c

stralloc_opys.o: \
compile stralloc_opys.c byte.h stralloc_opys.c str.h stralloc_opys.c \
stralloc.h gen_alloc.h stralloc.h stralloc_opys.c
	./compile stralloc_opys.c

stralloc_pend.o: \
compile stralloc_pend.c alloc.h stralloc_pend.c stralloc.h \
gen_alloc.h stralloc.h stralloc_pend.c gen_allocdefs.h \
gen_allocdefs.h gen_allocdefs.h stralloc_pend.c
	./compile stralloc_pend.c

strerr.a: \
makelib strerr_sys.o strerr_die.o
	./makelib strerr.a strerr_sys.o strerr_die.o

strerr_die.o: \
compile strerr_die.c substdio.h strerr_die.c subfd.h substdio.h \
substdio.h subfd.h strerr_die.c exit.h strerr_die.c strerr.h \
strerr_die.c
	./compile strerr_die.c

strerr_sys.o: \
compile strerr_sys.c error.h strerr_sys.c strerr.h strerr_sys.c
	./compile strerr_sys.c

subfderr.o: \
compile subfderr.c readwrite.h subfderr.c substdio.h subfderr.c \
subfd.h substdio.h substdio.h subfd.h subfderr.c
	./compile subfderr.c

subfdin.o: \
compile subfdin.c readwrite.h subfdin.c substdio.h subfdin.c subfd.h \
substdio.h substdio.h subfd.h subfdin.c
	./compile subfdin.c

subfdins.o: \
compile subfdins.c readwrite.h subfdins.c substdio.h subfdins.c \
subfd.h substdio.h substdio.h subfd.h subfdins.c
	./compile subfdins.c

subfdout.o: \
compile subfdout.c readwrite.h subfdout.c substdio.h subfdout.c \
subfd.h substdio.h substdio.h subfd.h subfdout.c
	./compile subfdout.c

subfdouts.o: \
compile subfdouts.c readwrite.h subfdouts.c substdio.h subfdouts.c \
subfd.h substdio.h substdio.h subfd.h subfdouts.c
	./compile subfdouts.c

subgetopt.o: \
compile subgetopt.c subgetopt.h subgetopt.h subgetopt.c
	./compile subgetopt.c

substdi.o: \
compile substdi.c substdio.h substdi.c byte.h substdi.c error.h \
substdi.c
	./compile substdi.c

substdio.a: \
makelib substdio.o substdi.o substdo.o subfderr.o subfdout.o \
subfdouts.o subfdin.o subfdins.o substdio_copy.o
	./makelib substdio.a substdio.o substdi.o substdo.o \
	subfderr.o subfdout.o subfdouts.o subfdin.o subfdins.o \
	substdio_copy.o

substdio.o: \
compile substdio.c substdio.h substdio.c
	./compile substdio.c

substdio_copy.o: \
compile substdio_copy.c substdio.h substdio_copy.c
	./compile substdio_copy.c

substdo.o: \
compile substdo.c substdio.h substdo.c str.h substdo.c byte.h \
substdo.c error.h substdo.c
	./compile substdo.c

supervise: \
load supervise.o coe.o fifo.o ndelay.a wait.a sig.a open.a lock.a \
strerr.a getopt.a substdio.a error.a str.a
	./load supervise coe.o fifo.o ndelay.a wait.a sig.a open.a \
	lock.a strerr.a getopt.a substdio.a error.a str.a 

supervise.0: \
supervise.1
	nroff -man supervise.1 > supervise.0

supervise.o: \
compile supervise.c supervise.c supervise.c supervise.c now.h \
datetime.h now.h supervise.c sig.h supervise.c coe.h supervise.c \
open.h supervise.c wait.h supervise.c fork.h supervise.c lock.h \
supervise.c fifo.h supervise.c error.h supervise.c select.h select.h \
select.h select.h supervise.c strerr.h supervise.c sgetopt.h \
subgetopt.h sgetopt.h supervise.c substdio.h supervise.c readwrite.h \
supervise.c
	./compile supervise.c

svc: \
load svc.o open.a sig.a ndelay.a getopt.a strerr.a substdio.a error.a \
str.a
	./load svc open.a sig.a ndelay.a getopt.a strerr.a \
	substdio.a error.a str.a 

svc.0: \
svc.1
	nroff -man svc.1 > svc.0

svc.o: \
compile svc.c ndelay.h svc.c strerr.h svc.c open.h svc.c sgetopt.h \
subgetopt.h sgetopt.h svc.c substdio.h svc.c readwrite.h svc.c exit.h \
svc.c byte.h svc.c sig.h svc.c
	./compile svc.c

svstat: \
load svstat.o strerr.a open.a substdio.a error.a str.a fs.a
	./load svstat strerr.a open.a substdio.a error.a str.a \
	fs.a 

svstat.0: \
svstat.1
	nroff -man svstat.1 > svstat.0

svstat.o: \
compile svstat.c strerr.h svstat.c open.h svstat.c substdio.h \
svstat.c readwrite.h svstat.c exit.h svstat.c fmt.h svstat.c
	./compile svstat.c

systype: \
find-systype trycpp.c
	./find-systype > systype

tailocal: \
load tailocal.o substdio.a error.a str.a fs.a
	./load tailocal substdio.a error.a str.a fs.a 

tailocal.0: \
tailocal.1
	nroff -man tailocal.1 > tailocal.0

tailocal.o: \
compile tailocal.c tailocal.c tailocal.c substdio.h tailocal.c \
subfd.h substdio.h substdio.h subfd.h tailocal.c exit.h tailocal.c \
fmt.h tailocal.c
	./compile tailocal.c

testfilelock: \
load testfilelock.o open.a lock.a strerr.a substdio.a error.a str.a
	./load testfilelock open.a lock.a strerr.a substdio.a \
	error.a str.a 

testfilelock.0: \
testfilelock.1
	nroff -man testfilelock.1 > testfilelock.0

testfilelock.o: \
compile testfilelock.c lock.h testfilelock.c strerr.h testfilelock.c \
exit.h testfilelock.c error.h testfilelock.c
	./compile testfilelock.c

usually: \
load usually.o slurp.o slurpclose.o open.a sig.a strerr.a substdio.a \
stralloc.a alloc.a error.a str.a
	./load usually slurp.o slurpclose.o open.a sig.a strerr.a \
	substdio.a stralloc.a alloc.a error.a str.a 

usually.0: \
usually.1
	nroff -man usually.1 > usually.0

usually.o: \
compile usually.c substdio.h usually.c readwrite.h usually.c strerr.h \
usually.c sig.h usually.c stralloc.h gen_alloc.h stralloc.h usually.c \
alloc.h usually.c gen_allocdefs.h gen_allocdefs.h gen_allocdefs.h \
usually.c slurp.h usually.c
	./compile usually.c

wait.a: \
makelib wait_nohang.o
	./makelib wait.a wait_nohang.o

wait_nohang.o: \
compile wait_nohang.c wait_nohang.c wait_nohang.c haswaitp.h \
wait_nohang.c
	./compile wait_nohang.c
