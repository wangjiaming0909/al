%{
#include "y.tab.h"
#include <math.h>
//不用再定义yylal了, 因为我们在yacc里定义了union
extern double vbltable[26];
extern struct Symbol *symbol_look(const char*);
%}
%%

([0-9]+|([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?) {
    yylval.dval = atof(yytext); return NUMBER;
  }

[ \t] ;
[A-Za-z_][A-Za-z0-9_]* { yylval.symbol = symbol_look(yytext); return NAME;}

"$" { return 0; /*end of input*/}
\n |
. return yytext[0];
%%
