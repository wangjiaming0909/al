%{
%}

%%

^[ \t]*\n { printf("empty line found\n"); }
\n { printf("end of none-empty line found\n"); }
. { printf("char: %s\n", yytext); }

%%
int main()
{
  yylex();
  return 0;
}



