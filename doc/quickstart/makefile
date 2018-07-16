DISTFLAGS = -q -dBATCH -dNOPAUSE -sDEVICE=pdfwrite \
	-sProcessColorModel=DeviceGray -dPDFSETTINGS=/prepress
CRYPT = -sOwnerPassword="`dd if=/dev/random bs=12 count=1 2>/dev/null | \
	md5sum | awk '{ print $$1 }'` " -dEncryptionR=3 \
	-dKeyLength=128 -dPermissions=-300

.SUFFIXES: .tr .ps .pdf .more .ditroff

.tr.ps:
	rm -f $@; trap "rm -f $$$$.ps" 0 2 3; \
		tbl $< | troff | dpost >$$$$.ps && mv $$$$.ps $@

.tr.ditroff:
	rm -f $@; trap "rm -f $$$$.ps" 0 2 3; \
		tbl $< | troff > $@

.ps.pdf:
	gs -sOutputFile=$@ $(DISTFLAGS) -f $<

.tr.more:
	tbl -Tlocale $< | nroff -Tlocale | col -x > $@

all: doc.ps

test:
	[ -e doc.more ] && mv doc.more doc.more.orig
	make doc.more
	diff -d doc.more.orig doc.more
	mv doc.more.orig doc.more
	[ -e doc.ditroff ] && mv doc.ditroff doc.ditroff.orig
	make doc.ditroff
	rm fonts.tmac
	diff -d doc.ditroff.orig doc.ditroff
	mv doc.ditroff.orig doc.ditroff
	[ -e doc.ps ] && mv doc.ps doc.ps.orig
	make doc.ps
	rm fonts.tmac
	sed 's/.CreationDate.*/%/' doc.ps >doc.ps.sed
	diff -d doc.ps.orig doc.ps.sed
	rm doc.ps.sed
	mv doc.ps.orig doc.ps

clean:
	rm -f doc.ps doc.pdf doc.more core log *~ fonts.tmac

fonts.tmac:
	@[ -n "$$TROFFONTS" ] || \
	    (echo "Error: Environment variable TROFFONTS not set" && false)
	@for F in LinLibertine AGaramondPro ; do \
		find $$(echo $$TROFFONTS | sed -e 's/:/ /g') -name "$$F*" | \
		    grep $$F > /dev/null && echo ".nr $$F 1" >> $@ || true; \
	done
	@grep nr $@ > /dev/null || \
	    (echo 'Error: None of the fonts "LinLibertine" and ' \
	     '"AGaramondPro" found' && false)

mrproper: clean

doc.ps: fonts.tmac
doc.ditroff: fonts.tmac
