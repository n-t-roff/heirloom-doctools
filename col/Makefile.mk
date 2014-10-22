BIN = col

OBJ = col.o

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LIBS) -o $(BIN)

install:
	$(INSTALL) -c $(BIN) $(ROOT)$(BINDIR)/$(BIN)
	$(STRIP) $(ROOT)$(BINDIR)/$(BIN)

clean:
	rm -f $(OBJ) $(BIN) core log *~

mrproper: clean
