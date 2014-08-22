OBJ = checknr.o

FLAGS =

.c.o:
	$(CC) $(CFLAGS) $(WARN) $(CPPFLAGS) $(FLAGS) -c $<

all: checknr

checknr: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LIBS) -o checknr

install:
	$(INSTALL) -c checknr $(ROOT)$(BINDIR)/checknr
	$(STRIP) $(ROOT)$(BINDIR)/checknr
	$(INSTALL) -c -m 644 checknr.1b $(ROOT)$(MANDIR)/man1b/checknr.1b

clean:
	rm -f $(OBJ) checknr core log *~

mrproper: clean
