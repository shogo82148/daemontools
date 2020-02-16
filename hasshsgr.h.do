dependon chkshsgr warn-shsgr tryshsgr.c compile load
formake './chkshsgr || ( cat warn-shsgr; exit 1 )'
formake '( ( ./compile tryshsgr.c \'
formake '&& ./load tryshsgr && ./tryshsgr ) >/dev/null 2>&1 \'
formake '&& echo \#define HASSHORTSETGROUPS 1 || exit 0 ) > hasshsgr.h'
formake 'rm -f tryshsgr.o tryshsgr'
./chkshsgr || ( cat warn-shsgr; exit 1 )
( ./compile tryshsgr.c \
&& ./load tryshsgr && ./tryshsgr ) >/dev/null 2>&1 \
&& echo \#define HASSHORTSETGROUPS 1
rm -f tryshsgr.o tryshsgr
