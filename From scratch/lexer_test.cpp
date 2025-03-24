#include <gtest/gtest.h>
#include "lexer.hpp"
#include <fstream>
#include <sstream>

// Hàm hỗ trợ tạo file tạm cho test
std::string createTempFile(const std::string& content) {
    std::string tempFilename = "temp_test_file.txt";
    std::ofstream tempFile(tempFilename);
    tempFile << content;
    tempFile.close();
    return tempFilename;
}

// Hàm lọc bỏ token EOF từ danh sách token
std::vector<Token> filterEOFTokens(const std::vector<Token>& tokens) {
    std::vector<Token> result;
    for (const auto& token : tokens) {
        if (token.type != TOKEN_EOF) {
            result.push_back(token);
        }
    }
    return result;
}

// Test case cho từ khóa
TEST(LexerTest, KeywordTokens) {
    std::string input = "begin end int bool if then else do while for print";
    std::string tempFilename = createTempFile(input);
    
    Lexer lexer(tempFilename);
    std::vector<Token> tokens = filterEOFTokens(lexer.scanTokens());
    
    ASSERT_EQ(tokens.size(), 11); // 11 keywords, không có EOF
    EXPECT_EQ(tokens[0].type, TOKEN_BEGIN);
    EXPECT_EQ(tokens[1].type, TOKEN_END);
    EXPECT_EQ(tokens[2].type, TOKEN_INT);
    EXPECT_EQ(tokens[3].type, TOKEN_BOOL);
    EXPECT_EQ(tokens[4].type, TOKEN_IF);
    EXPECT_EQ(tokens[5].type, TOKEN_THEN);
    EXPECT_EQ(tokens[6].type, TOKEN_ELSE);
    EXPECT_EQ(tokens[7].type, TOKEN_DO);
    EXPECT_EQ(tokens[8].type, TOKEN_WHILE);
    EXPECT_EQ(tokens[9].type, TOKEN_FOR);
    EXPECT_EQ(tokens[10].type, TOKEN_PRINT);
    
    // Xóa file tạm
    std::remove(tempFilename.c_str());
}

// Test case cho toán tử
TEST(LexerTest, OperatorTokens) {
    std::string input = "+ * > >= == = ";
    std::string tempFilename = createTempFile(input);
    
    Lexer lexer(tempFilename);
    std::vector<Token> tokens = filterEOFTokens(lexer.scanTokens());
    
    ASSERT_EQ(tokens.size(), 6); // 6 operators, không có EOF
    EXPECT_EQ(tokens[0].type, TOKEN_PLUS);
    EXPECT_EQ(tokens[1].type, TOKEN_MULTIPLY);
    EXPECT_EQ(tokens[2].type, TOKEN_GREATER);
    EXPECT_EQ(tokens[3].type, TOKEN_GREATER_EQUAL);
    EXPECT_EQ(tokens[4].type, TOKEN_EQUAL);
    EXPECT_EQ(tokens[5].type, TOKEN_ASSIGN);
    
    std::remove(tempFilename.c_str());
}

// Test case cho các token đặc biệt
TEST(LexerTest, SpecialTokens) {
    std::string input = "( ) { } ; , ";
    std::string tempFilename = createTempFile(input);
    
    Lexer lexer(tempFilename);
    std::vector<Token> tokens = filterEOFTokens(lexer.scanTokens());
    
    ASSERT_EQ(tokens.size(), 6); // 6 special tokens, không có EOF
    EXPECT_EQ(tokens[0].type, TOKEN_LEFT_PAREN);
    EXPECT_EQ(tokens[1].type, TOKEN_RIGHT_PAREN);
    EXPECT_EQ(tokens[2].type, TOKEN_LEFT_BRACE);
    EXPECT_EQ(tokens[3].type, TOKEN_RIGHT_BRACE);
    EXPECT_EQ(tokens[4].type, TOKEN_SEMICOLON);
    EXPECT_EQ(tokens[5].type, TOKEN_COMMA);
    
    std::remove(tempFilename.c_str());
}

// Test case cho identifier hợp lệ
TEST(LexerTest, ValidIdentifiers) {
    std::string input = "x y z abc abc123 variable ";
    std::string tempFilename = createTempFile(input);
    
    Lexer lexer(tempFilename);
    std::vector<Token> tokens = filterEOFTokens(lexer.scanTokens());
    
    ASSERT_EQ(tokens.size(), 6); // 6 identifiers, không có EOF
    EXPECT_EQ(tokens[0].type, TOKEN_IDENTIFIER);
    EXPECT_EQ(tokens[0].lexeme, "x");
    EXPECT_EQ(tokens[1].type, TOKEN_IDENTIFIER);
    EXPECT_EQ(tokens[1].lexeme, "y");
    EXPECT_EQ(tokens[2].type, TOKEN_IDENTIFIER);
    EXPECT_EQ(tokens[2].lexeme, "z");
    EXPECT_EQ(tokens[3].type, TOKEN_IDENTIFIER);
    EXPECT_EQ(tokens[3].lexeme, "abc");
    EXPECT_EQ(tokens[4].type, TOKEN_IDENTIFIER);
    EXPECT_EQ(tokens[4].lexeme, "abc123");
    EXPECT_EQ(tokens[5].type, TOKEN_IDENTIFIER);
    EXPECT_EQ(tokens[5].lexeme, "variable");
    
    std::remove(tempFilename.c_str());
}

// Test case cho số
TEST(LexerTest, NumberTokens) {
    std::string input = "0 123 9999 ";
    std::string tempFilename = createTempFile(input);
    
    Lexer lexer(tempFilename);
    std::vector<Token> tokens = filterEOFTokens(lexer.scanTokens());
    
    ASSERT_EQ(tokens.size(), 3); // 3 numbers, không có EOF
    EXPECT_EQ(tokens[0].type, TOKEN_NUMBER);
    EXPECT_EQ(tokens[0].lexeme, "0");
    EXPECT_EQ(tokens[1].type, TOKEN_NUMBER);
    EXPECT_EQ(tokens[1].lexeme, "123");
    EXPECT_EQ(tokens[2].type, TOKEN_NUMBER);
    EXPECT_EQ(tokens[2].lexeme, "9999");
    
    std::remove(tempFilename.c_str());
}

// Test case cho comment
TEST(LexerTest, Comments) {
    std::string input = "int x; // This is a comment\nint y; /* This is a\nmulti-line comment */\nint z; ";
    std::string tempFilename = createTempFile(input);
    
    Lexer lexer(tempFilename);
    std::vector<Token> tokens = filterEOFTokens(lexer.scanTokens());
    
    // Comments bị bỏ qua, nên chỉ còn: int, x, ;, int, y, ;, int, z, ;
    ASSERT_EQ(tokens.size(), 9);
    EXPECT_EQ(tokens[0].type, TOKEN_INT);
    EXPECT_EQ(tokens[1].type, TOKEN_IDENTIFIER);
    EXPECT_EQ(tokens[1].lexeme, "x");
    EXPECT_EQ(tokens[2].type, TOKEN_SEMICOLON);
    EXPECT_EQ(tokens[3].type, TOKEN_INT);
    EXPECT_EQ(tokens[4].type, TOKEN_IDENTIFIER);
    EXPECT_EQ(tokens[4].lexeme, "y");
    EXPECT_EQ(tokens[5].type, TOKEN_SEMICOLON);
    EXPECT_EQ(tokens[6].type, TOKEN_INT);
    EXPECT_EQ(tokens[7].type, TOKEN_IDENTIFIER);
    EXPECT_EQ(tokens[7].lexeme, "z");
    EXPECT_EQ(tokens[8].type, TOKEN_SEMICOLON);
    
    std::remove(tempFilename.c_str());
}

// Test case cho các lỗi
TEST(LexerTest, Errors) {
    // Test identifier không hợp lệ bắt đầu bằng số
    std::string input = "int 1abc; ";
    std::string tempFilename = createTempFile(input);
    
    Lexer lexer(tempFilename);
    std::vector<Token> tokens = filterEOFTokens(lexer.scanTokens());
    
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, TOKEN_INT);
    EXPECT_EQ(tokens[1].type, TOKEN_ERROR); // 1abc là lỗi
    EXPECT_EQ(tokens[2].type, TOKEN_SEMICOLON);
    
    std::remove(tempFilename.c_str());
    
    // Test comment không kết thúc
    input = "int x; /* This comment never ends ";
    tempFilename = createTempFile(input);
    
    Lexer lexer2(tempFilename);
    tokens = filterEOFTokens(lexer2.scanTokens());
    
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type, TOKEN_INT);
    EXPECT_EQ(tokens[1].type, TOKEN_IDENTIFIER);
    EXPECT_EQ(tokens[1].lexeme, "x");
    EXPECT_EQ(tokens[2].type, TOKEN_SEMICOLON);
    EXPECT_EQ(tokens[3].type, TOKEN_ERROR); // Comment không kết thúc
    
    std::remove(tempFilename.c_str());
    
    // Test ký tự không hợp lệ
    input = "int x = @; ";
    tempFilename = createTempFile(input);
    
    Lexer lexer3(tempFilename);
    tokens = filterEOFTokens(lexer3.scanTokens());
    
    ASSERT_EQ(tokens.size(), 5);
    EXPECT_EQ(tokens[0].type, TOKEN_INT);
    EXPECT_EQ(tokens[1].type, TOKEN_IDENTIFIER);
    EXPECT_EQ(tokens[1].lexeme, "x");
    EXPECT_EQ(tokens[2].type, TOKEN_ASSIGN);
    EXPECT_EQ(tokens[3].type, TOKEN_ERROR); // @ là ký tự không hợp lệ
    EXPECT_EQ(tokens[4].type, TOKEN_SEMICOLON);
    
    std::remove(tempFilename.c_str());
}

// Test chương trình UPL hoàn chỉnh
TEST(LexerTest, CompleteProgram) {
    std::string input = R"(
begin
    int x;
    int y = x + 1;
    /* comments 
    cho nhiều dòng 
    */
    bool a;//comment cho một dòng
    if (x > a) then {
        int c = 1;
    } else {
        y = x;
        x = x + 1;
    }
    print(a);
end 
)";
    std::string tempFilename = createTempFile(input);
    
    Lexer lexer(tempFilename);
    std::vector<Token> tokens = filterEOFTokens(lexer.scanTokens());
    
    // Kiểm tra số lượng token
    ASSERT_GT(tokens.size(), 10);
    
    // Kiểm tra một số token đặc trưng
    EXPECT_EQ(tokens[0].type, TOKEN_BEGIN);
    
    // Tìm token "print"
    bool foundPrint = false;
    for (const Token& token : tokens) {
        if (token.type == TOKEN_PRINT) {
            foundPrint = true;
            break;
        }
    }
    EXPECT_TRUE(foundPrint);
    
    // Token cuối cùng phải là END
    EXPECT_EQ(tokens.back().type, TOKEN_END);
    
    std::remove(tempFilename.c_str());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}