%{
#include <stdio.h>
#include <stdlib.h>
%}

/* Định nghĩa Flex */
%option noyywrap

/* 🔹 Regular Definitions */
delim           [ \t\n]
digit           [0-9]
letter          [A-Za-z]
candidate_id    [A-Za-z0-9]+
true_id         {letter}+{digit}*
number          {digit}+
comment         "/*"([^*]|\*+[^*/])*\*+"/"|"//".*


%%

{number}    { printf("NUMBER(%s)\n", yytext); }
{delim}     { /* Bỏ qua khoảng trắng */ }
{comment}   { }

"begin"     { printf("BEGIN\n"); }
"end"       { printf("END\n"); }

"for"       {printf("FOR\n");}

"do"        {printf("DO\n");}
"while"     {printf("WHILE\n");}

"if"        {printf("IF\n");}
"then"      {printf("THEN\n");}
"else"      {printf("ELSE\n");}

"print"     {printf("PRINT\n");}

"int"       {printf("INT\n");}
"bool"      {printf("BOOL\n");}

"true"      {printf("TRUE\n");}
"false"     {printf("FALSE\n");}

"*"         {printf("MUL\n");}
"+"         {printf("ADD\n");}

">"         {printf("GT\n");}
">="        {printf("GE\n");}
"=="        {printf("EQ\n");}

"="         {printf("ASSIGN\n");}

";"         {printf("SEMICOLON\n");}

"("         {printf("OPEN_PARENTHESE\n");}
")"         {printf("CLOSE_PARENTHESE\n");}

"{"         {printf("OPEN_CURLY\n");}
"}"         {printf("CLOSE_CURLY\n");}

{true_id}        { printf("IDENTIFIER(%s)\n", yytext); }
{candidate_id}   { printf("WRONG IDENTIFIER(%s)\n", yytext); }
.           { printf("UNKNOWN(%s)\n", yytext); }

%%

int main() {
    printf("Lexical Analysis Started...\n");
    yylex();
    printf("Lexical Analysis Finished.\n");
    return 0;
}
