#include "../include/Token.hpp"

Token::Token() {

}

Token::~Token() {

}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "Token(type=" << token.type
       << ", lexeme=\"" << token.lexeme
       << "\", line=" << token.line
       << ", col=" << token.col << ")";
    return os;
}
