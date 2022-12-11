</$objtype/mkfile

TARG=lua9
BIN=/$objtype/bin
LIB=lua/liblua.a.$O
OFILES=\
	display.$O \
	font.$O \
	image.$O \
	geometry.$O \
	draw.$O \
	event.$O \
	key.$O \
	color.$O \
	plumb.$O \
	plumbmsg.$O \
	plumbattr.$O \
	lib9.$O \
	utils.$O \
	lua9.$O
HFILES=a.h


</sys/src/cmd/mkone

CFLAGS=-FTVw -p -Ilua/shim -Ilua -DLUA_USE_PLAN9

$LIB:V:
	@{cd lua; mk}

pull:V:
	@{if(test -d lua){cd lua; git/pull}
	  if not git/clone https://git.sr.ht/~kvik/lu9-lua lua}
	
clean nuke:V:
	@{ cd lua; mk $target }
	rm -f *.[$OS] [$OS].out $TARG
