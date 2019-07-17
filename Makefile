# Build the oven menu system outside of IRAF

OBJS = oven.o menus.o domenus.o prints.o scans.o init.o \
	stale.o automkey.o \
	context.o cache.o menusc.o \
	shmalloc.o database.o databasc.o \
	ipportrw.o util.o console.o

DOBJS = ovend.o init.o shmalloc.o stale.o datalogger.o logdata.o database.o ipportrw.o

GOBJS = oveng.o init.o shmalloc.o database.o ipportrw.o

BOBJS = ovenb.o init.o shmalloc.o ipportrw.o
POBJS = ovenp.o init.o shmalloc.o ipportrw.o

#ROBJS = ovenr.o ipportrw.o
ROBJS = ovenr.o

EOBJS = ovene.o errorreport.o prints.o scans.o util.o init.o shmalloc.o database.o ipportrw.o

#CC = cc -g
#CC = cc -pedantic -Wstrict-prototypes -Wmissing-prototypes
CC = cc -Wold-style-definition

# ----

TARGETS = oven ovend oveng ovenb ovenp ovenr ovene

all: $(TARGETS)

install:	all
	cp $(TARGETS) /usr/local/bin
	cp train.au /usr/local/bin
	cp watch.pl /usr/local/bin/watch

.c.o:
	$(CC) -o $@ -c $<

ovene: $(EOBJS)
	cc -o ovene $(EOBJS) -lm

ovenr: $(ROBJS)
	cc -o ovenr $(ROBJS) -lm

ovenp: $(POBJS)
	cc -o ovenp $(POBJS) -lm

ovenb: $(BOBJS)
	cc -o ovenb $(BOBJS) -lm

oveng: $(GOBJS)
	cc -o oveng $(GOBJS) -lm

ovend: $(DOBJS)
	cc -o ovend $(DOBJS) -lm -lcfitsio

oven: $(OBJS)
	cc -o oven $(OBJS) -lm -lncurses

init.o:	init.c
domenus.o:	domenus.c
scans.o:	scans.c
automkey.o:	automkey.c
context.o:	context.c
cache.o:	cache.c
errorreport.o:	errorreport.c
stale.o:	stale.c
database.o:	database.c
databasc.o:	databasc.c
datalogger.o:	datalogger.c
logdata.o:	logdata.c
ipportrw.o:	ipportrw.c
shmalloc.o:	shmalloc.c
util.o:		util.c
console.o:	console.c

menus.o:	menusm/menus.o
	cp menusm/menus.o menus.o

menusc.o:	menusc/menusc.o
	cp menusc/menusc.o menusc.o

menusm/menus.o:
	make -C menusc
	make -C menusm

pieces:
	make -C menusc
	make -C menusm

makemenus: makemenus.c
	cc -o makemenus makemenus.c


clean:
	rm -f $(TARGETS)
	rm -f $(OBJS)
	make -C menusm clean
	make -C menusc clean

# THE END
