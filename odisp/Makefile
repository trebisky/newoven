# build a version of odisp independent of IRAF
# Tom Trebisky  3-19-2021

# HELPER_OBJS = odisp_helper.o common.o init.o shmalloc.o
HELPER_OBJS = odisp_helper.o

FOBJS = smooth.o iqhscv.o iqhsd.o iqhse.o iqhsf.o iqhsg.o iqhsh.o uertst.o ugetio.o

#CC = cc -g
#CC = cc -pedantic -Wstrict-prototypes -Wmissing-prototypes
#CC = cc -Wold-style-definition -Wmissing-prototypes
#CC = cc -Wold-style-definition -pedantic
CC = cc -Wold-style-definition -pedantic -I..
FC = g77

# ----

TARGETS = odisp_helper
PUBLISH = odisp_helper odisp

all: $(TARGETS)

install:	all
	cp $(PUBLISH) /usr/local/bin

.c.o:
	$(CC) -o $@ -c $<

%.o: ../%.c
	$(CC) -c $^  -o $@

.f.o:
	$(FC) -o $@ -c $<

odisp_helper: $(HELPER_OBJS) $(FOBJS)
	cc -o odisp_helper $(HELPER_OBJS) $(FOBJS) -lm -lcfitsio

odisp_helper.o:	odisp_helper.c
common.o:	../common.c
init.o:		../init.c
shmalloc.o:	../shmalloc.c

clean:
	rm -f $(TARGETS)
	rm -f *.o

# THE END
