IDIR = ./include
CC = gcc
CFLAGS = -g -I$(IDIR)
ODIR = ./obj
LDIR = ./lib

LIBS = -lm #add libs here


_DEPS = pineDucky.h parser.h
DEPS = $(pathsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = pineDucky.o parser.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

pineDucky: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -rf $(ODIR)/*.o
