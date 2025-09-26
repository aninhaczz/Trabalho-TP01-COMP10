
CC=gcc
CFLAGS=-Wall -Wextra -g
OBJ=trabalho.o symtable.o
all: lexer
lexer: $(OBJ)
	$(CC) $(CFLAGS) -o lexer $(OBJ)
trabalho.o: trabalho.c token.h symtable.h
	$(CC) $(CFLAGS) -c trabalho.c
symtable.o: symtable.c token.h symtable.h
	$(CC) $(CFLAGS) -c symtable.c
clean:
	rm -f *.o lexer *.lex
