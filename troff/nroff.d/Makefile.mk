VPATH=..

LIBHNJ = ../libhnj

OBJ = n10.o n6.o hytab.o n1.o n2.o n3.o n4.o n5.o \
	n7.o n8.o n9.o ni.o nii.o suftab.o \
	version.o

FLAGS = -DNROFF -DUSG $(EUC) -I. -I.. -DMACDIR='"$(MACDIR)"' \
	-DFNTDIR='"$(FNTDIR)"' -DTABDIR='"$(TABDIR)"' -DHYPDIR='"$(HYPDIR)"' \
	-DSHELL='"$(SHELL)"'

.c.o:
	$(CC) $(CFLAGS) $(WARN) $(CPPFLAGS) $(FLAGS) -c $<

all: nroff

nroff: $(OBJ) $(LIBHNJ)/libhnj.a
	$(CC) $(LDFLAGS) $(OBJ) -L$(LIBHNJ) -lhnj $(LIBS) -o nroff

install:
	$(INSTALL) -c nroff $(ROOT)$(BINDIR)/nroff
	$(STRIP) $(ROOT)$(BINDIR)/nroff
	$(INSTALL) -c -m 644 nroff.1b $(ROOT)$(MANDIR)/man1b/nroff.1b

clean:
	rm -f $(OBJ) nroff core log *~

mrproper: clean

n10.o: n10.c ../tdef.h ../ext.h tw.h pt.h
n6.o: n6.c ../tdef.h tw.h pt.h ../ext.h
hytab.o: ../hytab.c
malloc.o: ../malloc.c ../mallint.h
n1.o: ../n1.c ../tdef.h ../ext.h ./pt.h tw.h
n2.o: ../n2.c ../tdef.h ./pt.h ../ext.h tw.h
n3.o: ../n3.c ../tdef.h ./pt.h ../ext.h tw.h
n4.o: ../n4.c ../tdef.h ./pt.h ../ext.h tw.h
n5.o: ../n5.c ../tdef.h ./pt.h ../ext.h tw.h
n7.o: ../n7.c ../tdef.h ./pt.h ../ext.h tw.h
n8.o: ../n8.c ../tdef.h ../ext.h ./pt.h
n9.o: ../n9.c ../tdef.h ./pt.h ../ext.h tw.h
ni.o: ../ni.c ../tdef.h ./pt.h ./ext.h
nii.o: ../nii.c ../tdef.h ./pt.h ../ext.h tw.h
suftab.o: ../suftab.c
version.o: ../version.c
