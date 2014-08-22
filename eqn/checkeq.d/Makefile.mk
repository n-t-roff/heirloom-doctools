VPATH=..
OBJ = checkeq.o

FLAGS =

.c.o:
	$(CC) $(CFLAGS) $(WARN) $(CPPFLAGS) $(FLAGS) -c $<

all: checkeq

checkeq: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LIBS) -o checkeq

install:
	$(INSTALL) -c checkeq $(ROOT)$(BINDIR)/checkeq
	$(STRIP) $(ROOT)$(BINDIR)/checkeq
	rm -f $(ROOT)$(MANDIR)/man1b/checkeq.1b
	ln -s eqn.1b $(ROOT)$(MANDIR)/man1b/checkeq.1b

clean:
	rm -f $(OBJ) checkeq core log *~

mrproper: clean
