# LEX and YACC
## lex notes
lex包含三部分，`Definition Section`, `Rules Section`, `User Subroutine Section`
*******
### Definition Section
```lex
%{
    /*
    any c code
    */
    #include <string.h>
    //include header files
%}
word [^ \t\n]+
eol \n
%%
```
`%%`意思是C代码包裹的结束符
******
### Rules Section
每一个`rule`包含一个`pattern`和一个`action`, 中间是空格  
#### Pattern
pattern其实就是unix-style正则表达式以及grep,sed,ed使用的正则扩展。  
```lex
%{
    /*
    */
%}
%%
[\t ]+ /*ignore whitespace*/ ;

foo|bar { printf("foo or bar will go here");}

do |
does |
did |
go { printf("%s: is a verb\n", yytext);}
[a-zA-Z]+ { printf("%s: is not verb\n", yytext);}
.|\n { ECHO;/*default way*/}
%%
```
`pattern`: `[\t ]+`  
`action`: `;`
#### Action 其实就是C代码
`|` 是一个特殊的`action`，表示具体action与下一个action相同。  
最后的`%%`是结束符。
******
### User Subroutine Section
C代码, lex会把这些内容拷贝到输出。
```lex
%%
main() {
    yylex();
}
```
其实我们可以不写main函数，因为lex输出会默认带一个main函数。  
除非某个`pattern`的`action`有`return`操作，否则`yylex`会在处理完所有输入之后才会返回.
### lex单词计数的例子
```lex
%{
    /*
    */
unsigned charCount = 0, wordCount = 0, lineCount = 0;
%}
%}
word [^ \t\n]+
eol \n
%%

{word} { wordCount++; charCount += yyleng; }
{eol} { charCount++; lineCount++; }
. charCount++;
%%

main(argc,argv)
int argc;
char **argv;
{
    if (argc > 1) {
        FILE *file;
        file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr,"could not open %s\n",argv[1]);
            exit(1);
        }
        yyin = file;
    }
    yylex();
    printf("%u %u %u\n",charCount, wordCount, lineCount);
    return 0;
}
```
`Rule Section`内的`{word}`会被替换成`Definition Section`部分word对应的正则。  
`yyleng`是当前匹配word的长度。  
首先匹配先出现的规则最后匹配`.`这个规则.  
`yyin`就是`yylex`的输入，默认是从`stdin`  
`yywrap`用来切换`yyin`的，当`yyin`读到文件结尾时, `yylex`会调用`yywrap`,如果`yywrap`返回1那么表示结束，0表示可以接着读，并且期望在`yywrap`里已经重新打开了新文件, `yyin`已经设置好了。  
要重写`yywrap`, `input`, `unput`等函数需要先undef  
```lex
%{
#undef yywrap
#undef input
#undef unput
int yywrap();
int input(void);
void unput(int ch);
%}
%%
```
### lex用来做命令行参数解析的列子
```lex
%{
#undef input
#undef unput
unsigned verbose
unsigned fname;
char *progName;
%}

%s FNAME
%%

[ ]+ /* ignore blanks */ ;
-h |
"-?" |
-help { printf("usage is: %s [-help | -h | -? ] [-verbose | -v]"
    " (-file | -f) filename\n", progName);
}
-v |
-verbose { printf("verbose mode is on\n"); verbose = 1; }
-f |
-file { BEGIN FNAME; fname = 1; }
<FNAME>[^ ]+ { printf("use file %s\n", yytext); BEGIN 0; fname = 2;}
[^ ] + ECHO;
%%

char **targv; /* remembers arguments */
char **arglim; /* end of arguments */
main(int argc, char **argv)
{
    progName = *argv;
    targv = argv+1;
    arglim = argv+argc;
    yylex();
    if(fname < 2)
    printf("No filename given\n");
}

static unsigned offset = 0;
int input(void)
{
    char c;
    if (targv >= arglim) return(0); /* EOF */
    /* end of argument, move to the next */
    if ((c = targv[0][offset++]) != '\0') return(c);
    targv++;
    offset = 0;
    return(' ');
}
/* simple unput only backs up, doesn't allow you to */
/* put back different text */
void
unput(int ch)
{
    /* AT&T lex sometimes puts back the EOF ! */
    if(ch == 0) return; /* ignore, can't put back EOF */
    if (offset) { /* back up in current arg */
        offset−−;
        return;
    }
    targv−−; /* back to previous arg */
    offset = strlen(*targv);
}
```
`input`, `unput`在`AT&T` lex支持重新define，貌似`flex`不支持  
这两个函数在`AT&T` lex中作为宏存在，我们可以将它重新定义为函数。  

`yylex`会反复调用`input`获取字符  
`input`返回0表示`EOF`, 返回空格表示下一个token, 否则返回一个字符.  
`unput`放回一个字符,其实就是把当前指针回退一格, 具体什么时候被调用没看懂...  

`%s FNAME`定义了一个`start state`  
用来标记一些具有上下文信息的规则, 如`<FNAME> [^ ]+ {printf("use file %s\n", yytext); BEGIN 0; fname = 2;}`  
上面匹配到`-file {BEGIN FNAME; fname = 1;}`时就会开启该`start state`  
标记之后lexer只会在当前状态处于`FNAME`时才会匹配到此规则.  
于是文件的名字就可以单独被解析出来, 此规则的`action`部分会`BEGIN 0`, `0`或者`INITIAL`, 重置状态.  



### 编译

```bash
lex 1.l
#生成lex.yy.c
yacc -d 1.y
#生成y.tab.c y.tab.h
#y.tab.h包含token定义使用-d选项
cc -c lex.yy.c y.tab.c
cc -o a.out lex.yy.o y.tab.o -ll
```
*******
## yacc notes
yacc 包括下面几部分
### Definition Section
与lex相同, 但是多了个`TOKEN`的定义部分
```yacc
%{
#include <stdio.h>
%}
%token NOUM PRONOUM VERB
%%
```
一般情况下`TOKEN`最好都是大写，其他用小写

### Rules Section
```yacc
sentence: subject VERB object {printf("Sentence is valid.\n");}
    ;
subject:    NOUN
    |       PRONOUN
    ;
object:     NOUM
    ;
%%
```

### User Subroutines Section
```yacc
extern FILE *yyin;
main()
{
    do {
        yyparse();
    } while (!feof(yyin));
}
yyerror(s)
char *s;
{
    fprintf(stderr, "%s\n", s);
}
```
lexer在返回0时表示当前语句解析结束。