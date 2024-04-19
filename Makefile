CC=gcc
CFLAGS=-I.
OBJ = xhell.o
LIBS=-lncurses
EXE=xhell


%.o: %.c $(DEPS) 
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


clean: 
	rm -f *.o && rm -f $(EXE)
