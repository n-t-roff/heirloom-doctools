DISTFLAGS = -q -dBATCH -dNOPAUSE -sDEVICE=pdfwrite \
	-sProcessColorModel=DeviceGray -dPDFSETTINGS=/prepress
CRYPT = -sOwnerPassword="`dd if=/dev/random bs=12 count=1 2>/dev/null | \
	md5sum | awk '{ print $$1 }'` " -dEncryptionR=3 \
	-dKeyLength=128 -dPermissions=-300
TROFFFLAGS = -mgchar

.SUFFIXES: .tr .ps .pdf .html

.tr.ps:
	rm -f $@; trap "rm -f $$$$.ps" 0 2 3; \
		tbl -g $< | troff $(TROFFFLAGS) | \
		    dpost >$$$$.ps && mv $$$$.ps $@

.ps.pdf:
	gs -sOutputFile=$@ $(DISTFLAGS) -f $<

.tr.html:
	tbl $< | troff -Thtml $(TROFFFLAGS) | dhtml -t \
	    "Nroff/Troff User's Manual" > $@

all: doc.ps

clean:
	rm -f doc.ps doc.pdf doc.html core log *~

mrproper: clean
