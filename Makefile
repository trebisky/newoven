# Build the oven menu system outside of IRAF

OBJS = oven.o menus.o domenus.o prints.o scans.o init.o \
	errorreport.o stale.o automkey.o \
	context.o cache.o menusc.o \
	shmalloc.o database.o ipportrw.o \
	util.o glue.o

all: oven $(OBJS)

#oven: oven.c
#	cc -o oven oven.c

oven: $(OBJS)
	cc -o oven $(OBJS) -lm

init.o:	init.c
domenus.o:	domenus.c
scans.o:	scans.c
automkey.o:	automkey.c
context.o:	context.c
cache.o:	cache.c
errorreport.o:	errorreport.c
stale.o:	stale.c
database.o:	database.c
ipportrw.o:	ipportrw.c
shmalloc.o:	shmalloc.c
util.o:		util.c
glue.o:		glue.c

menus.o:	menusm/menus.o
	cp menusm/menus.o menus.o

menusc.o:	menusc/menusc.o
	cp menusc/menusc.o menusc.o

menusm/menus.o:
	make -C menust
	make -C menusm

pieces:
	make -C menust

makemenus: makemenus.c
	cc -o makemenus makemenus.c


clean:
	rm -f $(OBJS)
	make -C menusm clean
	make -C menust clean

# THE END
