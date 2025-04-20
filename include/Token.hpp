#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

typedef enum {
    NUMBER=1,
    BEGIN_TOKEN, END_TOKEN,
    FOR, DO, WHILE,
    IF, THEN, ELSE,
    PRINT,
    INT, BOOL,
    TRUE, FALSE,
    MULTIPLY, PLUS,
    GREATER_THAN, GREATER_EQUAL, EQUAL,
    ASSIGN,
    SEMICOLON,
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_CURLY, RIGHT_CURLY,
    IDENTIFIER,
    UNKNOWN,
    DOLLAR,
} TokenType;

struct Token {
    TokenType type;
    std::string lexeme;
    int value;
    int line;
    int col;

    Token();
    ~Token();

};

std::ostream& operator<<(std::ostream& os, const Token& token);

extern Token current_token;

#endif