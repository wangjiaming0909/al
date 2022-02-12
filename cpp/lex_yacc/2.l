%{
%}

%s FNAME
%%

[ \t]+ {printf("ignore blanks\n");}

-f    |
--file {BEGIN FNAME; printf("BEGIN FNAME\n");}

<FNAME>[^ \t\n]+ {printf("use file %s\n", yytext); BEGIN INITIAL;}

\n { printf("new line\n"); }


. {printf("any [%s]\n", yytext);}

%%
int main()
{
  yylex();
  return 0;
}



