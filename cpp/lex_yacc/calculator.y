%{
//this is calculator
double vbltable[26]; //26个小写字母的变量名字
%}

%union {
  double dval;
  int vblno;
}

%token <vblno> NAME
%token <dval> NUMBER
%left '-' '+'
%left '*' '/'
%nonassoc UMINUS

%type <dval> expression
%%
stmt_list: stmt '\n'
  | stmt_list stmt '\n'
  ;

stmt: NAME '=' expression { vbltable[$1] = $3; }
  | expression {printf("= %g\n", $1); }
  ;

expression: expression '+' expression {$$ = $1 + $3; }
  | expression '-' expression { $$ = $1 - $3; }
  | expression '*' expression { $$ = $1 * $3; }
  | expression '/' expression { $$ = $1 / $3; }
  | '-' expression %prec UMINUS { $$ = -$2; }
  | '(' expression ')' { $$ = $2; }
  | NUMBER
  | NAME {$$ = vbltable[$1];}
  ;


%%

int main() {
  yyparse();
  return 0;
}
