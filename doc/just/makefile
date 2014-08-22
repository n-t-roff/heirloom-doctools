DISTFLAGS = -q -dBATCH -dNOPAUSE -sDEVICE=pdfwrite \
	-sProcessColorModel=DeviceGray -dPDFSETTINGS=/prepress
CRYPT = -sOwnerPassword="`dd if=/dev/random bs=12 count=1 2>/dev/null | \
	md5sum | awk '{ print $$1 }'` " -dEncryptionR=3 \
	-dKeyLength=128 -dPermissions=-300

.SUFFIXES: .tr .ps .pdf

.tr.ps:
	rm -f $@; trap "rm -f $$$$.ps" 0 2 3; \
		tbl $< | eqn | troff | dpost >$$$$.ps && mv $$$$.ps $@

.ps.pdf:
	gs -sOutputFile=$@ $(DISTFLAGS) -f $<

all: doc.ps

print.ps: doc.pdf
	pdftops -paper match doc.pdf $@

book.ps: print.ps
	psbook print.ps | psnup -2 -pa4 -Pa5 | \
	sed 's/^\(%%DocumentMedia:\).*/\1 x595y842 595 842 0 () ()/' >$@

odd.ps: book.ps
	psselect -o book.ps $@

even.ps: book.ps
	psselect -e book.ps $@

clean:
	rm -f book.ps book.pdf doc.ps doc.pdf print.ps \
		odd.ps odd.pdf even.ps even.pdf core log *~

mrproper: clean
