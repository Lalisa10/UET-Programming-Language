#include <bits/stdc++.h>
using namespace std;

enum TokenType {
    TOKEN_BEGIN, TOKEN_END, TOKEN_INT, TOKEN_BOOL, 
    TOKEN_IF, TOKEN_THEN, TOKEN_ELSE, TOKEN_DO, TOKEN_WHILE, TOKEN_FOR, TOKEN_PRINT,
    TOKEN_PLUS, TOKEN_MULTIPLY, TOKEN_GREATER, TOKEN_GREATER_EQUAL, TOKEN_EQUAL, TOKEN_ASSIGN,
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN, TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE, 
    TOKEN_SEMICOLON, TOKEN_COMMA,
    TOKEN_IDENTIFIER, TOKEN_NUMBER,
    TOKEN_EOF, TOKEN_ERROR
};
string tokenTypeName(TokenType type);

struct Token {
    TokenType type;
    string message, lexeme;
    int line;
    
    Token(TokenType type, const string& lexeme, int line, const string& message = "")
        : type(type), line(line), message(message), lexeme(lexeme) {}

    friend ostream& operator<<(ostream& os, const Token& token) {
        if (token.message.empty()) {
            os << tokenTypeName(token.type);
        } else {
            os << tokenTypeName(token.type) << ' ' << token.message;
        }
        return os;
    }
};

class FileReader {
private:
    ifstream file;
    static const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE * 2 + 2]; // 2 buffers + 2 EOF markers
    char* lexemeBegin;               // Con trỏ đến điểm bắt đầu token
    char* forward;                   // Con trỏ đến vị trí đang quét
    int line;
    int column;
    bool reachedEOF;
    string lexemeBuffer;

public:
    FileReader(const string& filename) : line(1), column(0), reachedEOF(false) {
        file.open(filename);
        if (!file.is_open()) {
            throw runtime_error("Không thể mở file: " + filename);
        }
        
        // Đánh dấu cuối của mỗi buffer bằng ký tự EOF (để xác định khi nào cần đọc buffer tiếp theo)
        buffer[BUFFER_SIZE] = EOF;
        buffer[BUFFER_SIZE * 2 + 1] = EOF;
        
        // Đọc dữ liệu vào buffer đầu tiên
        file.read(buffer, BUFFER_SIZE);
        if (file.gcount() < BUFFER_SIZE) {
            buffer[file.gcount()] = EOF; // Đánh dấu EOF thực sự
            reachedEOF = true;
        }
        
        // Khởi tạo con trỏ
        lexemeBegin = buffer;
        forward = buffer;

        lexemeBuffer.reserve(1024);
    }

    ~FileReader() {
        if (file.is_open()) {
            file.close();
        }
    }

    // Trả về ký tự hiện tại mà không di chuyển con trỏ
    char peek() {
        return *forward;
    }

    // Trả về ký tự kế tiếp mà không di chuyển con trỏ
    char peekNext() {
        if (*(forward + 1) == EOF) {
            loadNextBuffer();
        }
        return *(forward + 1);
    }

    // Di chuyển con trỏ và trả về ký tự mới
    char advance() {
        if (*forward == EOF) {
            loadNextBuffer();
        }
        
        char c = *forward++;
        column++;
        
        if (c == '\n') {
            line++;
            column = 0;
        }
        
        return c;
    }

    // Lấy lexeme hiện tại (từ lexemeBegin đến forward)
    const string& getLexeme() {
        if (lexemeBegin <= forward) {
            lexemeBuffer.assign(lexemeBegin, forward);
        } else {
            lexemeBuffer.clear();
            lexemeBuffer.append(lexemeBegin, buffer + BUFFER_SIZE);
            lexemeBuffer.append(buffer + BUFFER_SIZE + 1, forward);
        }
        return lexemeBuffer;
    }

    // Đặt lại điểm bắt đầu token
    void setLexemeBegin() {
        lexemeBegin = forward;
    }

    // Kiểm tra xem đã đọc hết file chưa
    bool isAtEnd() {
        return reachedEOF && *forward == EOF;
    }

    int getLine() const {
        return line;
    }

    int getColumn() const {
        return column;
    }

private:
    void loadNextBuffer() {
        // Xác định buffer nào đang được sử dụng
        bool usingFirstBuffer = (forward < buffer + BUFFER_SIZE);
        
        // Xác định vị trí để đọc vào
        char* loadPosition = usingFirstBuffer ? buffer + BUFFER_SIZE + 1 : buffer;
        
        // Nếu đã đọc hết file, không làm gì
        if (reachedEOF) return;
        
        // Đọc dữ liệu mới
        file.read(loadPosition, BUFFER_SIZE);
        int bytesRead = file.gcount();
        
        // Đánh dấu EOF nếu cần
        if (bytesRead < BUFFER_SIZE) {
            loadPosition[bytesRead] = EOF;
            reachedEOF = true;
        }
        
        // Cập nhật con trỏ forward
        forward = usingFirstBuffer ? buffer + BUFFER_SIZE + 1 : buffer;
    }
};

class Lexer {
private:
    unique_ptr<FileReader> reader;
    unordered_map<string, TokenType> keywords;

public:
    Lexer(const string& filename) {
        reader = make_unique<FileReader>(filename);
        keywords["begin"] = TOKEN_BEGIN;
        keywords["end"] = TOKEN_END;
        keywords["int"] = TOKEN_INT;
        keywords["bool"] = TOKEN_BOOL;
        keywords["if"] = TOKEN_IF;
        keywords["then"] = TOKEN_THEN;
        keywords["else"] = TOKEN_ELSE;
        keywords["do"] = TOKEN_DO;
        keywords["while"] = TOKEN_WHILE;
        keywords["for"] = TOKEN_FOR;
        keywords["print"] = TOKEN_PRINT;
    }

    vector<Token> scanTokens();
    Token nextToken();

private:
    void skipWhitespace();
    bool match(char expected);
    Token scanIdentifier();
    Token scanNumber();
};