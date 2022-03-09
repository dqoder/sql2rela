# Makefile

FILES = Lexer.c Parser.c my_util.c main.c
CC = gcc
CFLACG = -g # -ansi

conv: Lexer.c Parser.c 
	$(CC) $(CFLAGS) $(FILES) -o conv

# gcc -g Lexer.c Parser.c -o conv

Lexer.c: Parser.c sql2ra.l
	flex sql2ra.l

Parser.c: sql2ra.y my_util.h 
	bison -d sql2ra.y

clean:
	rm -f Lexer.c Parser.c Parser.h Lexer.h conv