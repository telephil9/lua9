APE=/sys/src/ape
<$APE/config

TARG=lua9

BIN=/$objtype/bin/ape

OFILES=\
	display.$O \
	font.$O \
	image.$O \
	geometry.$O \
	key.$O \
	color.$O \
	utils.$O \
	lua9.$O

HFILES=\
	ldraw.h \
	utils.h

</sys/src/cmd/mkone

CC=pcc
LD=pcc
CFLAGS= -c -I. -D_C99_SNPRINTF_EXTENSION -D_PLAN9_SOURCE -D_POSIX_SOURCE \
        -D_SUSV2_SOURCE -DLUA_POSIX -DENABLE_CJSON_GLOBAL \
        -DPlan9 -DMAKE_LUA

install:V:
        cp $O.out /$objtype/bin/ape/$TARG
        
nuke:V:
        rm -f /$objtype/bin/ape/$TARG
        

