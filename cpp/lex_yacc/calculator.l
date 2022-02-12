%{
#include "y.tab.h"
#include <math.h>
//不用再定义yylal了, 因为我们再yacc里定义了union
extern double vbltable[26];
%}
%%

([0-9]+|([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?) {
    yylval.dval = atof(yytext); return NUMBER;
  }

[ \t] ;
[a-z] {yylval.vblno = yytext[0] - 'a'; return NAME;}

"$" {return 0; /*end of input*/}
\n |
. return yytext[0];
%%
