IDIR =include
SDIR =src
TDIR = tools
CC=g++
CFLAGS=-I$(IDIR)

ODIR=.
LDIR =

LIBS=-lm

_DEPS = freakDatum.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ_T = testDatum.o
_OBJ_S = freakDatum.o

OBJ_T = $(patsubst %,$(TDIR)/$(ODIR)/%,$(_OBJ_T))
OBJ_S = $(patsubst %,$(SDIR)/$(ODIR)/%,$(_OBJ_S))

OBJ = $(OBJ_T) $(OBJ_S)

$(info $(OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

out: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
