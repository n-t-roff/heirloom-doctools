SUBDIRS = eqn/eqn.d \
	eqn/neqn.d \
	eqn/checkeq.d \
	eqn/eqnchar.d \
	tbl \
	pic \
	grap \
	vgrind \
	refer \
	mpm \
	troff/libhnj \
	troff/libhnj/hyphen.d \
	troff/troff.d/font \
	troff/troff.d/dpost.d \
	troff/troff.d/devaps \
	troff/troff.d/tmac.d \
	troff/troff.d/postscript \
	troff/troff.d \
	troff/nroff.d \
	troff/nroff.d/terms.d \
	picpack \
	checknr \
	ptx \
	soelim

MAKEFILES = $(SUBDIRS:=/Makefile)

.SUFFIXES: .mk
.mk:
	cat mk.config $< >$@

dummy: $(MAKEFILES) all

makefiles: $(MAKEFILES)

.DEFAULT:
	+ for i in $(SUBDIRS); \
	do \
		(cd "$$i" && $(MAKE) $@) || exit; \
	done

mrproper: clean
	+ for i in $(SUBDIRS); \
	do \
		(cd "$$i" && $(MAKE) $@) || exit; \
	done
	rm -f $(MAKEFILES)

PKGROOT = /var/tmp/heirloom-devtools
PKGTEMP = /var/tmp
PKGPROTO = pkgproto

heirloom-doctools.pkg: all
	rm -rf $(PKGROOT)
	mkdir -p $(PKGROOT)
	$(MAKE) ROOT=$(PKGROOT) install
	rm -f $(PKGPROTO)
	echo 'i pkginfo' >$(PKGPROTO)
	(cd $(PKGROOT) && find . -print | pkgproto) | >>$(PKGPROTO) sed 's:^\([df] [^ ]* [^ ]* [^ ]*\) .*:\1 root root:; s:^f\( [^ ]* etc/\):v \1:; s:^f\( [^ ]* var/\):v \1:; s:^\(s [^ ]* [^ ]*=\)\([^/]\):\1./\2:'
	rm -rf $(PKGTEMP)/$@
	pkgmk -a `uname -m` -d $(PKGTEMP) -r $(PKGROOT) -f $(PKGPROTO) $@
	pkgtrans -o -s $(PKGTEMP) `pwd`/$@ $@
	rm -rf $(PKGROOT) $(PKGPROTO) $(PKGTEMP)/$@
