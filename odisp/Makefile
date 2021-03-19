# build a version of odisp independent of IRAF
# Tom Trebisky  3-19-2021

HELPER_OBJS = odisp_helper.o common.o init.o shmalloc.o database.o ipportrw.o

#CC = cc -g
#CC = cc -pedantic -Wstrict-prototypes -Wmissing-prototypes
#CC = cc -Wold-style-definition -Wmissing-prototypes
#CC = cc -Wold-style-definition -pedantic
CC = cc -Wold-style-definition -pedantic -I..

# ----

TARGETS = odisp_helper
PUBLISH = odisp_helper

all: $(TARGETS)

install:	all
	cp $(PUBLISH) /usr/local/bin

.c.o:
	$(CC) -o $@ -c $<

%.o: ../%.c
	$(CC) -c $^  -o $@

odisp_helper: $(HELPER_OBJS)
	cc -o odisp_helper $(HELPER_OBJS) -lm

odisp_helper.o:	odisp_helper.c
common.o:	../common.c
init.o:		../init.c
shmalloc.o:	../shmalloc.c
database.o:	../database.c
ipportrw.o:	../ipportrw.c

clean:
	rm -f $(TARGETS)
	rm -f *.o

# THE END
