%{
#include <stdio.h>
#include <stdlib.h>
#include "include/Token.hpp"

int curr_line = 1;
int curr_col = 1;

Token current_token;

void set_token(TokenType type, const char* text);

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

<INITIAL>\n         { curr_line++; curr_col = 1; }
<INITIAL>[ \t]+     { curr_col += yyleng; }
<INITIAL>{number}    {
    set_token(NUMBER, yytext);
    //printf("NUMBER(%s)\n", yytext);
    current_token.value = atoi(yytext);
    return NUMBER;
    }
<INITIAL>{comment}   { }

<INITIAL>"begin"     {
    //printf("BEGIN\n");
    set_token(BEGIN_TOKEN, yytext);
    return BEGIN_TOKEN;
    }
<INITIAL>"end"       {
    //printf("END\n");
    set_token(END_TOKEN, yytext);
    return END_TOKEN; }

<INITIAL>"for"       {
    //printf("FOR\n");
    set_token(FOR, yytext);
    return FOR;
    }

<INITIAL>"do"        {
    //printf("DO\n");
    set_token(DO, yytext);
    return DO;
    }
<INITIAL>"while"     {
    //printf("WHILE\n");
    set_token(WHILE, yytext);
    return WHILE;
    }

<INITIAL>"if"        {
    //printf("IF\n");
    set_token(IF, yytext);
    return IF;
    }
<INITIAL>"then"      {
    //printf("THEN\n");
    set_token(THEN, yytext);
    return THEN;
    }
<INITIAL>"else"      {
    //printf("ELSE\n");
    set_token(ELSE, yytext);
    return ELSE;
    }

<INITIAL>"print"     {
    //printf("PRINT\n");
    set_token(PRINT, yytext);
    return PRINT;
    }

<INITIAL>"int"       {
    //printf("INT\n");
    set_token(INT, yytext);
    return INT;
    }
<INITIAL>"bool"      {
    //printf("BOOL\n");
    set_token(BOOL, yytext);
    return BOOL;
    }

<INITIAL>"true"      {
    //printf("TRUE\n");
    set_token(TRUE, yytext);
    return TRUE;
    }
<INITIAL>"false"     {
    //printf("FALSE\n");
    set_token(FALSE, yytext);
    return FALSE;
    }

<INITIAL>"*"         {
    //printf("MULTIPLY\n");
    set_token(MULTIPLY, yytext);
    return MULTIPLY;
    }
<INITIAL>"+"         {
    //printf("PLUS\n");
    set_token(PLUS, yytext);
    return PLUS;
    }

<INITIAL>">"         {
    //printf("GREATER_THAN\n");
    set_token(GREATER_THAN, yytext);
    return GREATER_THAN;
    }

<INITIAL>">="        {
    //printf("GREATER_EQUAL\n");
    set_token(GREATER_EQUAL, yytext);
    return GREATER_EQUAL;
    }

<INITIAL>"=="        {
    //printf("EQUAL\n");
    set_token(EQUAL, yytext);
    return EQUAL;
    }

<INITIAL>"="         {
    //printf("ASSIGN\n");
    set_token(ASSIGN, yytext);
    return ASSIGN;
    }

<INITIAL>";"         {
    //printf("SEMICOLON\n");
    set_token(SEMICOLON, yytext);
    return SEMICOLON;
    }

<INITIAL>"("         {
    //printf("LEFT_PAREN\n");
    set_token(LEFT_PAREN, yytext);
    return LEFT_PAREN;
    }
<INITIAL>")"         {
    //printf("RIGHT_PAREN\n");
    set_token(RIGHT_PAREN, yytext);
    return RIGHT_PAREN;
    }

<INITIAL>"{"         {
    //printf("LEFT_CURLY\n");
    set_token(LEFT_CURLY, yytext);
    return LEFT_CURLY;
    }

<INITIAL>"}"         {
    //printf("RIGHT_CURLY\n");
    set_token(RIGHT_CURLY, yytext);
    return RIGHT_CURLY;
    }

<INITIAL>{true_id}           { 
    //printf("IDENTIFIER(%s)\n", yytext);
    set_token(IDENTIFIER, yytext);
    return IDENTIFIER;
    }
<INITIAL>{candidate_id}      { 
    //printf("WRONG IDENTIFIER(%s)\n", yytext); 
    set_token(UNKNOWN, yytext);
    return UNKNOWN;
    }

"/*"                { BEGIN(COMMENT); }
<COMMENT><<EOF>>    { //printf("UNTERMINATED_COMMENT!\n");
                         exit(1);}
<COMMENT>"*/"       { BEGIN(INITIAL); }
<COMMENT>\n         { ++curr_line; }
<COMMENT>.|\n       { /* Bỏ qua nội dung comment */ }

.           { 
    //printf("UNKNOWN(%s)\n", yytext);
    set_token(UNKNOWN, yytext);
    return UNKNOWN;
    }

%%

void set_token(TokenType type, const char* text) {
    current_token.type = type;
    current_token.lexeme = text;  // cần free sau này nếu muốn
    current_token.line = curr_line;
    current_token.col = curr_col;
    curr_col += yyleng;
}


