MACS = acm.me bib chars.me deltext.me e eqn.me \
	float.me footnote.me index.me local.me m mmn mmt ms.acc \
	ms.cov ms.eqn ms.ref ms.tbl ms.ths ms.toc null.me refer.me \
	s sh.me tbl.me thesis.me v vgrind \
	an andoc doc doc-common doc-ditroff doc-nroff doc-syms \
	pictures color pm srefs ptx safe g padj taa naa

.SUFFIXES: .in
.in:
	sed 's:@MACDIR@:$(MACDIR):; s:@LIBDIR@:$(LIBDIR):' $< >$@

all: $(MACS)

install: all
	test -d $(ROOT)$(MACDIR) || mkdir -p $(ROOT)$(MACDIR)
	for i in $(MACS); \
	do \
		$(INSTALL) -c -m 644 $$i $(ROOT)$(MACDIR)/$$i || exit; \
	done
	test -d $(ROOT)$(MANDIR)/man7 || mkdir -p $(ROOT)$(MANDIR)/man7
	$(INSTALL) -c -m 644 mcolor.7 $(ROOT)$(MANDIR)/man7/mcolor.7
	$(INSTALL) -c -m 644 mpictures.7 $(ROOT)$(MANDIR)/man7/mpictures.7

clean:
	rm -f andoc bib doc e g m s pm

mrproper: clean
