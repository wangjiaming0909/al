%{
%}

%start stmt
%token NAME NUMBER
%left '-' '+'
%left '*' '/'
%right UMINUS

%%

stmt: NAME '=' expression
    | expression { printf("= %d\n", $1); }
    ;

expression: expression '+' expression {$$ = $1 + $3; }
    | expression '-' expression {$$ = $1 - $3; }
    | expression '*' expression { $$ = $1 * $3; }
    | expression '/' expression
      {
        if ($3 == 0) yyerror("divide by zero");
        else $$ = $1 / $3;
      }
    | '-' expression { $$ = -$2; }
    | '(' expression ')' { $$ = $2; }
    | NUMBER { $$ = $1; }
    ;
%%


int main()
{
  yyparse();
  return 0;
}
