%{
#include "calculator.h"
#include <stdio.h>
#include "y.tab.h"
#include <stdlib.h>
#include <string.h>
struct Symbol symbol_table[MAX_SYMBOL_NUM];
extern int yylex();
extern int yyerror();

%}

%union {
  double dval;
  struct Symbol *symbol;
}

%token <symbol> NAME
%token <dval> NUMBER
%right '='
%left '-' '+'
%left '*' '/'
%nonassoc UMINUS

%type <dval> expression
%type <dval> assign_expression
%%
stmt_list: stmt '\n'
  | stmt_list stmt '\n'
  ;

stmt: expression {printf("= %g\n", $1); }
  | assign_expression {printf("= %g\n", $1);}
  ;

assign_expression: NAME '=' expression {$$ = $1->value = $3;}
  | NAME '=' assign_expression {$$ = $1->value = $3; }
  ;

expression: 
  NAME {$$ = $1->value;}
  | expression '+' expression {$$ = $1 + $3; }
  | expression '-' expression { $$ = $1 - $3; }
  | expression '*' expression { $$ = $1 * $3; }
  | expression '/' expression { $$ = $1 / $3; }
  | '-' expression %prec UMINUS { $$ = -$2; }
  | '(' expression ')' { $$ = $2; }
  | NUMBER
  ;


%%

int main() {
  yyparse();
  return 0;
}

struct Symbol *symbol_look(const char* s)
{
  char *p;
  struct Symbol *sp;
  for(sp = symbol_table; sp < &symbol_table[MAX_SYMBOL_NUM]; sp++) {
    /* is it already here? */
    if(sp->name && !strcmp(sp->name, s))
      return sp;
    /* is it free */
    if(!sp->name) {
      sp->name = strdup(s);
      return sp;
    }
  /* otherwise continue to next */
  }
  yyerror("Too many symbols");
  exit(1); /* cannot continue */
}
