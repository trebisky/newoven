# Build all the little pieces.
# The files in ../menust are automatically generated "templates"
# that were convered by hand into these files.

INCLIST = ../oven.h ../menus.h ../context.h ../global.h ../protos.h

OBJS = aa.o ad.o al.o cs.o db.o dc.o er.o fs.o gl.o he.o ht.o hp.o \
	jt.o ms.o no.o oo.o pp.o rn.o rt.o rz.o sb.o tc.o ti.o \
	ts.o tt.o ty.o zo.o zp.o zt.o zz.o

.c.o:
	cc -o $@ -c $<


all:	menusc.o

menusc.o:	$(OBJS)
	cc -o menusc.o -r $(OBJS)

clean:
	rm -f $(OBJS)

aa.o:	aa.c $(INCLIST)
ad.o:	ad.c $(INCLIST)
al.o:	al.c $(INCLIST)
cs.o:	cs.c $(INCLIST)
db.o:	db.c $(INCLIST)
dc.o:	dc.c $(INCLIST)
er.o:	er.c $(INCLIST)
fs.o:	fs.c $(INCLIST)
gl.o:	gl.c $(INCLIST)
he.o:	he.c $(INCLIST)
ht.o:	ht.c $(INCLIST)
hp.o:	hp.c $(INCLIST)
jt.o:	jt.c $(INCLIST)
ms.o:	ms.c $(INCLIST)
no.o:	no.c $(INCLIST)
oo.o:	oo.c $(INCLIST)
pp.o:	pp.c $(INCLIST)
rn.o:	rn.c $(INCLIST)
rt.o:	rt.c $(INCLIST)
rz.o:	rz.c $(INCLIST)
sb.o:	sb.c $(INCLIST)
tc.o:	tc.c $(INCLIST)
ti.o:	ti.c $(INCLIST)
ts.o:	ts.c $(INCLIST)
tt.o:	tt.c $(INCLIST)
ty.o:	ty.c $(INCLIST)
zo.o:	zo.c $(INCLIST)
zp.o:	zp.c $(INCLIST)
zt.o:	zt.c $(INCLIST)
zz.o:	zz.c $(INCLIST)

