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
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

struct Token {
    TokenType type;
    string lexeme;
    int line;
    
    Token(TokenType type, const string& lexeme, int line)
        : type(type), lexeme(lexeme), line(line) {}

    friend ostream& operator<<(ostream& os, const Token& token) {
        os << tokenTypeName(token.type) << " " << token.lexeme << " " << token.line;
        return os;
    }
};

class FileReader {
private:
    std::ifstream file;
    static const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE * 2 + 2]; // 2 buffers + 2 EOF markers
    char* lexemeBegin;               // Con trỏ đến điểm bắt đầu token
    char* forward;                   // Con trỏ đến vị trí đang quét
    int line;
    int column;
    bool reachedEOF;

public:
    FileReader(const std::string& filename) : line(1), column(0), reachedEOF(false) {
        file.open(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Không thể mở file: " + filename);
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
    string getLexeme() {
        // Xử lý trường hợp lexeme nằm giữa hai buffer
        if (lexemeBegin <= forward) {
            return string(lexemeBegin, forward);
        } else {
            // Lexeme nằm ở cuối buffer 1 và đầu buffer 2
            string part1(lexemeBegin, buffer + BUFFER_SIZE);
            string part2(buffer + BUFFER_SIZE + 1, forward);
            return part1 + part2;
        }
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
    // Đọc buffer tiếp theo khi cần
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
        
        // Nếu lexemeBegin nằm ở buffer cũ, chúng ta phải xử lý việc sao chép
        // để đảm bảo lexeme còn nguyên vẹn (việc này thường được làm khi bắt đầu token mới)
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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }
    try {
        freopen("lexer_output.txt", "w", stdout);

        Lexer lexer(argv[1]);
        vector<Token> tokens = lexer.scanTokens();
        for (const Token& token : tokens) {
            cout << token << endl;
        }
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
    return 0;
}