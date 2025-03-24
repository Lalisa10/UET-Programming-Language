#include "lexer.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    try {
        ofstream out("output.txt");

        Lexer lexer(argv[1]);
        vector<Token> tokens = lexer.scanTokens();

        for (const Token& token : tokens) {
            out << token << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}