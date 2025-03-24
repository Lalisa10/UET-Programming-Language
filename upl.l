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

"*"         {printf("MULTIPLY\n");}
"+"         {printf("PLUS\n");}

">"         {printf("GREATER_THAN\n");}
">="        {printf("GREATER_EQUAL\n");}
"=="        {printf("EQUAL\n");}

"="         {printf("ASSIGN\n");}

";"         {printf("SEMICOLON\n");}

"("         {printf("LEFT_PAREN\n");}
")"         {printf("RIGHT_PAREN\n");}

"{"         {printf("LEFT_CURLY\n");}
"}"         {printf("RIGHT_CURLY\n");}

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
