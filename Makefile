CC=gcc
CFLAGS=-I. -ggdb
OBJ = src/xhell.o
LIBS=-lncurses
EXE=xhell.elf #Specifying .elf so .gitignore can pick it up


%.o: %.c $(DEPS) 
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


clean: 
	rm -f *.o && rm -f $(EXE)
