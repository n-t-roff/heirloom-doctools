OBJ = ptx.o

FLAGS = -DLIBDIR='"$(LIBDIR)"' $(EUC) -I../include

.c.o:
	$(CC) $(_CFLAGS) $(FLAGS) -c $<

all: ptx ptx.1

ptx: $(OBJ)
	$(CC) $(_CFLAGS) $(_LDFLAGS) $(OBJ) $(LIBS) -o ptx

install:
	$(INSTALL) -c ptx $(ROOT)$(BINDIR)/ptx
	$(STRIP) $(ROOT)$(BINDIR)/ptx
	$(INSTALL) -c -m 644 ptx.1 $(ROOT)$(MANDIR)/man1/ptx.1
	test -d $(ROOT)$(LIBDIR) || mkdir -p $(ROOT)$(LIBDIR)
	$(INSTALL) -c -m 644 eign $(ROOT)$(LIBDIR)/eign

clean:
	rm -f $(OBJ) ptx core log *~ ptx.1

mrproper: clean

ptx.1: ptx.1.in
	sed -e "s'/usr/5bin/sort'`which sort`'" \
	    -e 's"/usr/ucblib/doctools/tmac/"$(ROOT)$(BINDIR)/"' \
	    -e 's"/usr/ucblib/"$(ROOT)$(LIBDIR)/"' \
	    ptx.1.in > $@
