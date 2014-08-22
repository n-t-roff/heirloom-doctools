FLAGS =

.c.o:
	$(CC) $(CFLAGS) $(WARN) $(CPPFLAGS) $(FLAGS) -c $<

OBJ = hnjalloc.o hyphen.o

all: libhnj.a test

libhnj.a: $(OBJ)
	$(AR) -rv libhnj.a $(OBJ)
	$(RANLIB) $@

test: test.o libhnj.a
	$(CC) $(LDFLAGS) test.o -L. -lhnj -o test

install:

clean:
	rm -f $(OBJ) test test.o core log *~

mrproper: clean
	rm -f libhnj.a
