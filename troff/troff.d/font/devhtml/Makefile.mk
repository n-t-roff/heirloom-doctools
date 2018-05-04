BIN=		makefont
OBJS=		$(BIN).o
FONTS=		R I B BI C CW CR CI CB H HI HB S

.SUFFIXES:	.in
.in: ${BIN}
	cat $< > $@
	./${BIN} $@ >> $@

all: ${FONTS}

install:
	d=$(ROOT)$(FNTDIR)/devhtml; test -d $$d || mkdir $$d; \
	install -m 644 CHAR DESC $$d/; \
	echo charset >> $$d/DESC; \
	sed '1,2d;s/[[:space:]].*//' charset >> $$d/DESC; \
	for i in $(FONTS); do \
		install -m 644 $$i $$d/; \
	done

clean:
		rm -rf $(BIN) $(OBJS) $(FONTS)

mrproper:	clean

$(BIN):		$(OBJS)
		$(CC) $(_CFLAGS) $(_LDFLAGS) $(OBJS) -o $@

$(FONTS): $(BIN)
