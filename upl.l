%{
#include <stdio.h>
#include <stdlib.h>
%}

/* Định nghĩa Flex */
%option noyywrap
%start COMMENT

/* 🔹 Regular Definitions */
delim                   [ \t\n]
digit                   [0-9]
letter                  [A-Za-z]
candidate_id            [A-Za-z0-9]+
true_id                 {letter}+{digit}*
number                  {digit}+
comment                 "//".*


%%

<INITIAL>{number}    { printf("NUMBER(%s)\n", yytext); }
<INITIAL>{delim}     { /* Bỏ qua khoảng trắng */ }
<INITIAL>{comment}   { }

<INITIAL>"begin"     { printf("BEGIN\n"); }
<INITIAL>"end"       { printf("END\n"); }

<INITIAL>"for"       {printf("FOR\n");}

<INITIAL>"do"        {printf("DO\n");}
<INITIAL>"while"     {printf("WHILE\n");}

<INITIAL>"if"        {printf("IF\n");}
<INITIAL>"then"      {printf("THEN\n");}
<INITIAL>"else"      {printf("ELSE\n");}

<INITIAL>"print"     {printf("PRINT\n");}

<INITIAL>"int"       {printf("INT\n");}
<INITIAL>"bool"      {printf("BOOL\n");}

<INITIAL>"true"      {printf("TRUE\n");}
<INITIAL>"false"     {printf("FALSE\n");}

<INITIAL>"*"         {printf("MULTIPLY\n");}
<INITIAL>"+"         {printf("PLUS\n");}

<INITIAL>">"         {printf("GREATER_THAN\n");}
<INITIAL>">="        {printf("GREATER_EQUAL\n");}
<INITIAL>"=="        {printf("EQUAL\n");}

<INITIAL>"="         {printf("ASSIGN\n");}

<INITIAL>";"         {printf("SEMICOLON\n");}

<INITIAL>"("         {printf("LEFT_PAREN\n");}
<INITIAL>")"         {printf("RIGHT_PAREN\n");}

<INITIAL>"{"         {printf("LEFT_CURLY\n");}
<INITIAL>"}"         {printf("RIGHT_CURLY\n");}

<INITIAL>{true_id}           { printf("IDENTIFIER(%s)\n", yytext); }
<INITIAL>{candidate_id}      { printf("WRONG IDENTIFIER(%s)\n", yytext); }

"/*"                { BEGIN(COMMENT); }
<COMMENT><<EOF>>    { printf("UNTERMINATED_COMMENT!\n"); exit(1);}
<COMMENT>"*/"       { BEGIN(INITIAL); }
<COMMENT>.|\n       { /* Bỏ qua nội dung comment */ }

.           { printf("UNKNOWN(%s)\n", yytext); }

%%

int main() {
    printf("Lexical Analysis Started...\n");
    yylex();
    printf("Lexical Analysis Finished.\n");
    return 0;
}
