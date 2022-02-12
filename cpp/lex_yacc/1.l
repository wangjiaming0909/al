%{
%}
%s STATE1
%x STATE2
%%


state1 { BEGIN STATE1; printf("enterd state1\n");}
state2 { BEGIN STATE2; printf("enterd state2\n");}
OK { printf("UNKNOW state OK\n");}
<STATE1>OK { BEGIN INITIAL; printf("exit state1: %s\n", yytext);}
<STATE2>OK { BEGIN INITIAL; printf("exit state2: %s\n", yytext);}


\"[^"]*\"  {
              if (yytext[yyleng - 2] == '\\') {
                yyless(yyleng - 1);
                yymore();
              } else {
                  printf("got a string: %s\n", yytext);
              }
            }


^[ \t]*\n { printf("empty line found\n"); }
\n { printf("end of none-empty line found\n"); }
. { printf("char: %s\n", yytext); }
%%
int main()
{
  yylex();
  return 0;
}



