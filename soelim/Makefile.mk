OBJ = soelim.o

FLAGS =

.c.o:
	$(CC) $(CFLAGS) $(WARN) $(CPPFLAGS) $(FLAGS) -c $<

all: soelim

soelim: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LIBS) -o soelim

install:
	$(INSTALL) -c soelim $(ROOT)$(BINDIR)/soelim
	$(STRIP) $(ROOT)$(BINDIR)/soelim
	$(INSTALL) -c -m 644 soelim.1b $(ROOT)$(MANDIR)/man1b/soelim.1b

clean:
	rm -f $(OBJ) soelim core log *~

mrproper: clean
