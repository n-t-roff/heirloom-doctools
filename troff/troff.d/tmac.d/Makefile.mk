MACS = acm.me bib chars.me deltext.me e eqn.me \
	float.me footnote.me index.me local.me m mmn mmt ms.acc \
	ms.cov ms.eqn ms.ref ms.tbl ms.ths ms.toc null.me refer.me \
	s sh.me tbl.me thesis.me v vgrind \
	tmac.gchar andoc doc doc-common doc-nroff doc-syms \
	pictures color pm srefs ptx safe g padj taa naa
IMACS =	an doc-ditroff
OMACS = $(IMACS:%=%.out)

.SUFFIXES: .in
.in:
	sed 's:@MACDIR@:$(MACDIR):; s:@LIBDIR@:$(LIBDIR):' $< >$@

all: $(MACS) $(OMACS)

install: all
	test -d $(ROOT)$(MACDIR) || mkdir -p $(ROOT)$(MACDIR)
	for i in $(MACS); \
	do \
		$(INSTALL) -c -m 644 $$i $(ROOT)$(MACDIR)/$$i || exit; \
	done
	for i in $(IMACS); do \
		$(INSTALL) -c -m 644 $${i}.out $(ROOT)$(MACDIR)/$$i || exit; \
	done
	test -d $(ROOT)$(MANDIR)/man7 || mkdir -p $(ROOT)$(MANDIR)/man7
	$(INSTALL) -c -m 644 mcolor.7 $(ROOT)$(MANDIR)/man7/mcolor.7
	$(INSTALL) -c -m 644 mpictures.7 $(ROOT)$(MANDIR)/man7/mpictures.7

clean:
	rm -f andoc bib doc e g m s pm $(OMACS)

mrproper: clean

$(OMACS): $(IMACS) tmac.gchar
	sed 's:@MACDIR@:$(MACDIR):; s:@LIBDIR@:$(LIBDIR):' $(@:%.out=%) >$@
