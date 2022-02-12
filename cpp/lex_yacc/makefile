CC = gcc -DYYDEBUG=1
LEX = lex -I
YACC = yacc -d
all: lex1 lex2 yacc1 yacc2

clean:
	rm -rf ./*.o
	rm -rf ./*.out

lex1: 1.l
	$(LEX) 1.l
	${CC} lex.yy.c -ggdb -O0 -ll -o 1.out

lex2: 2.l
	$(LEX) 2.l
	${CC} lex.yy.c -ggdb -O0 -ll -o 2.out

yacc1: 3.l 1.y
	$(LEX) 3.l
	$(YACC) -d 1.y
	${CC} lex.yy.c y.tab.c -ggdb -O0 -ll -ly -o 3.out

yacc2: calculator.y calculator.l
	$(YACC) -d calculator.y
	$(LEX) calculator.l
	${CC} lex.yy.c y.tab.c -ggdb -O0 -ll -ly -o calculator.out

calculator: calculator1.y calculator1.l
	$(YACC) -d calculator1.y
	$(LEX) calculator1.l
	${CC} lex.yy.c y.tab.c -ggdb -O0 -ll -ly -o calculator.out


