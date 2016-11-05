OBJ = soelim.o

FLAGS =

.c.o:
	$(CC) $(_CFLAGS) $(FLAGS) -c $<

all: soelim soelim.1

soelim: $(OBJ)
	$(CC) $(_CFLAGS) $(_LDFLAGS) $(OBJ) $(LIBS) -o soelim

install:
	$(INSTALL) -c soelim $(ROOT)$(BINDIR)/soelim
	$(STRIP) $(ROOT)$(BINDIR)/soelim
	$(INSTALL) -c -m 644 soelim.1 $(ROOT)$(MANDIR)/man1/soelim.1

clean:
	rm -f $(OBJ) soelim core log *~ soelim.1

mrproper: clean

soelim.1: soelim.1.in
	sed 's"/usr/ucblib/doctools/tmac/"$(ROOT)$(MACDIR)/"' soelim.1.in > $@
