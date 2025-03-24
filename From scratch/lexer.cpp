#include "lexer.hpp"

string tokenTypeName(TokenType type) {
    switch (type) {
        case TOKEN_BEGIN: return "BEGIN";
        case TOKEN_END: return "END";
        case TOKEN_INT: return "INT";
        case TOKEN_BOOL: return "BOOL";
        case TOKEN_IF: return "IF";
        case TOKEN_THEN: return "THEN";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_DO: return "DO";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_FOR: return "FOR";
        case TOKEN_PRINT: return "PRINT";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_GREATER: return "GREATER";
        case TOKEN_GREATER_EQUAL: return "GREATER_EQUAL";
        case TOKEN_EQUAL: return "EQUAL";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_LEFT_PAREN: return "LEFT_PAREN";
        case TOKEN_RIGHT_PAREN: return "RIGHT_PAREN";
        case TOKEN_LEFT_BRACE: return "LEFT_BRACE";
        case TOKEN_RIGHT_BRACE: return "RIGHT_BRACE";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

vector<Token> Lexer::scanTokens() {
    vector<Token> tokens;
    
    while (!reader->isAtEnd()) {
        // Đặt điểm bắt đầu token mới
        reader->setLexemeBegin();
        
        Token token = nextToken();
        if (token.type != TOKEN_EOF) {
            tokens.push_back(token);
        }
        
        if (token.type == TOKEN_EOF) {
            break;
        }
    }
    
    return tokens;
}

Token Lexer::nextToken() {
    skipWhitespace();
    
    // Đặt điểm bắt đầu token mới sau khi bỏ qua whitespace
    reader->setLexemeBegin();

    if (reader->isAtEnd()) {
        return Token(TOKEN_EOF, "", reader->getLine());
    }

    char c = reader->advance();

    switch (c) {
        case '(': return Token(TOKEN_LEFT_PAREN, "(", reader->getLine());
        case ')': return Token(TOKEN_RIGHT_PAREN, ")", reader->getLine());
        case '{': return Token(TOKEN_LEFT_BRACE, "{", reader->getLine());
        case '}': return Token(TOKEN_RIGHT_BRACE, "}", reader->getLine());
        case ';': return Token(TOKEN_SEMICOLON, ";", reader->getLine());
        case '+': return Token(TOKEN_PLUS, "+", reader->getLine());
        case '*': return Token(TOKEN_MULTIPLY, "*", reader->getLine());

        case '>': {
            if (match('=')) {
                return Token(TOKEN_GREATER_EQUAL, ">=", reader->getLine());
            }
            return Token(TOKEN_GREATER, ">", reader->getLine());
        }

        case '=': {
            if (match('=')) {
                return Token(TOKEN_EQUAL, "==", reader->getLine());
            }
            return Token(TOKEN_ASSIGN, "=", reader->getLine());
        }

        case '/': {
            if (match('/')) {
                // Comment một dòng
                while (reader->peek() != '\n' && !reader->isAtEnd()) {
                    reader->advance();
                }
                return nextToken();
            } else if (match('*')) {
                // Comment nhiều dòng
                while (!(reader->peek() == '*' && reader->peekNext() == '/') && !reader->isAtEnd()) {
                    reader->advance();
                }
                
                if (reader->isAtEnd()) {
                    return Token(TOKEN_ERROR, "", reader->getLine(), "Unterminated comment");
                }
                
                // Bỏ qua */ kết thúc
                reader->advance(); // *
                reader->advance(); // /
                
                return nextToken();
            }
            return Token(TOKEN_ERROR, "", reader->getLine(), "Unexpected character");
        }
        
        default: {
            if (isdigit(c)) {
                return scanNumber();
            } else if (isalpha(c)) {
                return scanIdentifier();
            } else {
                return Token(TOKEN_ERROR, string(1, c), reader->getLine(), "Unexpected character \'" + string(1, c) + "\'");
            }
        }
    }
}

void Lexer::skipWhitespace() {
    while (true) {
        char c = reader->peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
            case '\n':
                reader->advance();
                break;
            default:
                return;
        }
    }
}

bool Lexer::match(char expected) {
    if (reader->isAtEnd() || reader->peek() != expected) {
        return false;
    }
    
    reader->advance();
    return true;
}

Token Lexer::scanIdentifier() {
    // Đọc toàn bộ identifier
    while (isalnum(reader->peek())) {
        reader->advance();
    }
    
    // Lấy lexeme từ lexemeBegin đến forward
    const string &lexeme = reader->getLexeme();
    
    // Kiểm tra từ khóa
    TokenType type = TOKEN_IDENTIFIER;
    if (keywords.find(lexeme) != keywords.end()) {
        type = keywords[lexeme];
        return Token(type, lexeme, reader->getLine());
    } else {
        // Kiểm tra tính hợp lệ của identifier
        bool valid = true;
        if (!isalpha(lexeme[0])) {
            valid = false;
        } else {
            // Tìm vị trí đầu tiên của số
            size_t i = 1;
            while (i < lexeme.size() && isalpha(lexeme[i])) {
                i++;
            }
            
            // Từ vị trí i đến cuối chuỗi phải toàn là số
            while (i < lexeme.size()) {
                if (!isdigit(lexeme[i])) {
                    valid = false;
                    break;
                }
                i++;
            }
        }
        
        if (!valid) {
            return Token(TOKEN_ERROR, lexeme, reader->getLine(), "Invalid identifier \'" + lexeme + "\'");
        }
        
        return Token(type, lexeme, reader->getLine(), lexeme);
    }
}

Token Lexer::scanNumber() {
    // Đọc toàn bộ số
    while (isdigit(reader->peek())) {
        reader->advance();
    }

    if (isalpha(reader->peek())) {
        // Đọc nốt phần còn lại để báo lỗi
        while (isalnum(reader->peek())) {
            reader->advance();
        }
        
        return Token(TOKEN_ERROR, reader->getLexeme(), reader->getLine(), "Invalid number \'" + reader->getLexeme() + "\'");
    }
    
    // Lấy lexeme từ lexemeBegin đến forward
    string lexeme = reader->getLexeme();
    
    return Token(TOKEN_NUMBER, lexeme, reader->getLine(), lexeme);
}