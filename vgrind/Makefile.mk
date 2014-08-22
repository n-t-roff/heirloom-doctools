OBJ = regexp.o vfontedpr.o vgrindefs.o version.o

FLAGS = $(EUC) -DLIBDIR='"$(LIBDIR)"'

.c.o:
	$(CC) $(CFLAGS) $(WARN) $(FLAGS) $(CPPFLAGS) -c $<

all: vgrind vfontedpr

vfontedpr: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LIBS) -o vfontedpr

vgrind: vgrind.sh
	rm -f $@
	echo "#!$(SHELL)" >>$@
	echo "_TROFF=$(BINDIR)/troff" >>$@
	echo "_VFONTEDPR=$(LIBDIR)/vfontedpr" >>$@
	echo "_TMAC_VGRIND=$(MACDIR)/vgrind" >>$@
	echo "_DPOST=$(BINDIR)/dpost" >>$@
	cat vgrind.sh >>$@
	chmod 755 $@

install:
	$(INSTALL) -c vgrind $(ROOT)$(BINDIR)/vgrind
	test -d $(ROOT)$(LIBDIR) || mkdir -p $(ROOT)$(LIBDIR)
	$(INSTALL) -c vfontedpr $(ROOT)$(LIBDIR)/vfontedpr
	$(STRIP) $(ROOT)$(LIBDIR)/vfontedpr
	$(INSTALL) -c -m 644 vgrindefs.src $(ROOT)$(LIBDIR)/vgrindefs
	$(INSTALL) -c -m 644 vgrind.1b $(ROOT)$(MANDIR)/man1b/vgrind.1b

clean:
	rm -f $(OBJ) vfontedpr vgrind retest retest.o core log *~

mrproper: clean
