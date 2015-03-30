OBJ = checknr.o

FLAGS = -D$(SYSTEM) -I../include

.c.o:
	$(CC) $(CFLAGS) $(WARN) $(CPPFLAGS) $(FLAGS) -c $<

all: checknr

checknr: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LIBS) -o checknr

install:
	$(INSTALL) -c checknr $(ROOT)$(BINDIR)/checknr
	$(STRIP) $(ROOT)$(BINDIR)/checknr
	$(INSTALL) -c -m 644 checknr.1 $(ROOT)$(MANDIR)/man1/checknr.1

clean:
	rm -f $(OBJ) checknr core log *~

mrproper: clean
