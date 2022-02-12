CC = gcc
all:
	lex 1.l
	${CC} lex.yy.c -ggdb -O0 -ll -o 1.out
	lex 2.l
	${CC} lex.yy.c -ggdb -O0 -ll -o 2.out
	lex 3.l
	yacc -d 1.y
	${CC} lex.yy.c y.tab.c -ggdb -O0 -ll -ly -o 3.out

clean:
	rm -rf ./*.o
	rm -rf ./*.out
