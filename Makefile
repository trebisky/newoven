# Build the oven menu system outside of IRAF

OBJS = oven.o menus.o domenus.o prints.o scans.o init.o \
	errorreport.o stale.o automkey.o \
	context.o cache.o menusc.o \
	shmalloc.o database.o databasc.o \
	ipportrw.o util.o console.o

all: oven

.c.o:
	cc -g -o $@ -c $<

#oven: oven.c
#	cc -o oven oven.c

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
ipportrw.o:	ipportrw.c
shmalloc.o:	shmalloc.c
util.o:		util.c
console.o:	console.c

menus.o:	menusm/menus.o
	cp menusm/menus.o menus.o

#menusc.o:	menusc/menusc.o
#	cp menusc/menusc.o menusc.o

menusm/menus.o:
	make -C menusc
	make -C menusm

pieces:
	make -C menusc
	make -C menusm

makemenus: makemenus.c
	cc -o makemenus makemenus.c


clean:
	rm -f $(OBJS)
	make -C menusm clean
	make -C menusc clean

# THE END
