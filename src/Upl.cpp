#include "Upl.hpp"
#include "Token.hpp"
#include "Drawer.hpp"

extern int yylex();

namespace upl_grammar {
    
    std::shared_ptr<NonTerminal>
                start = std::make_shared<NonTerminal> ("S"),
                program = std::make_shared<NonTerminal> ("program"),
                stmts = std::make_shared<NonTerminal> ("stmts"),
                stmt = std::make_shared<NonTerminal> ("stmt"),
                expression = std::make_shared<NonTerminal> ("expression"),
                assignment = std::make_shared<NonTerminal> ("assignment"),
                declaration = std::make_shared<NonTerminal> ("declaration"),
                init = std::make_shared<NonTerminal> ("init"),
                condition = std::make_shared<NonTerminal> ("condition"),
                update = std::make_shared<NonTerminal> ("update"),
                relop = std::make_shared<NonTerminal> ("relop"),
                addition = std::make_shared<NonTerminal> ("addition"),
                multiplication = std::make_shared<NonTerminal> ("multiplication"),
                factor = std::make_shared<NonTerminal> ("factor"),
                dtype = std::make_shared<NonTerminal> ("dtype")
                ;

    std::shared_ptr<Terminal>
                begin = std::make_shared<Terminal> ("begin"),
                end = std::make_shared<Terminal> ("end"),
                epsilon = std::make_shared<Terminal> ("eps"),
                semicolon = std::make_shared<Terminal> (";"),
                if_ = std::make_shared<Terminal> ("if"),
                then = std::make_shared<Terminal> ("then"),
                else_ = std::make_shared<Terminal> ("else"),
                do_ = std::make_shared<Terminal> ("do"),
                while_ = std::make_shared<Terminal> ("while"),
                for_ = std::make_shared<Terminal> ("for"),
                print = std::make_shared<Terminal> ("print"),
                id = std::make_shared<Terminal> ("id"),
                number = std::make_shared<Terminal> ("number"),
                int_ = std::make_shared<Terminal> ("int"),
                bool_ = std::make_shared<Terminal> ("bool"),
                plus = std::make_shared<Terminal> ("+"),
                mul = std::make_shared<Terminal> ("*"),
                left_paren = std::make_shared<Terminal> ("("),
                right_paren = std::make_shared<Terminal> (")"),
                left_curly = std::make_shared<Terminal> ("{"),
                right_curly = std::make_shared<Terminal> ("}"),
                assign = std::make_shared<Terminal> ("="),
                greater_than = std::make_shared<Terminal> (">"),
                greater_equal = std::make_shared<Terminal> (">="),
                equal = std::make_shared<Terminal> ("=="),
                true_ = std::make_shared<Terminal> ("true"),
                false_ = std::make_shared<Terminal> ("false"),
                dollar = std::make_shared<Terminal> ("$");
    
    std::vector<std::shared_ptr<Production>> productionList = {
        std::make_shared<Production>(start)->addSymbol(program),
        std::make_shared<Production>(program)->addSymbol(begin)->addSymbol(stmts)->addSymbol(end),

        std::make_shared<Production>(stmts)->addSymbol(stmts)->addSymbol(stmt),
        std::make_shared<Production>(stmts), //->epsilon

        std::make_shared<Production>(stmt)->addSymbol(expression)->addSymbol(semicolon),
        std::make_shared<Production>(stmt)->addSymbol(assignment)->addSymbol(semicolon),
        std::make_shared<Production>(stmt)->addSymbol(declaration)->addSymbol(semicolon),

        std::make_shared<Production>(stmt)->addSymbol(if_)->addSymbol(left_paren)->addSymbol(expression)
        ->addSymbol(right_paren)->addSymbol(then)->addSymbol(stmt),

        std::make_shared<Production>(stmt)->addSymbol(if_)->addSymbol(left_paren)->addSymbol(expression)
        ->addSymbol(right_paren)->addSymbol(then)->addSymbol(stmt)->addSymbol(else_)->addSymbol(stmt),
        
        std::make_shared<Production>(stmt)->addSymbol(do_)->addSymbol(stmt)->addSymbol(while_)
        ->addSymbol(left_paren)->addSymbol(expression)->addSymbol(right_paren)->addSymbol(semicolon),

        std::make_shared<Production>(stmt)->addSymbol(for_)->addSymbol(left_paren)->addSymbol(init)
        ->addSymbol(semicolon)->addSymbol(condition)->addSymbol(semicolon)->addSymbol(update)
        ->addSymbol(right_paren)->addSymbol(stmt),

        std::make_shared<Production>(stmt)->addSymbol(print)->addSymbol(left_paren)->addSymbol(expression)
        ->addSymbol(right_paren)->addSymbol(semicolon),

        std::make_shared<Production>(stmt)->addSymbol(left_curly)->addSymbol(stmts)->addSymbol(right_curly),

        std::make_shared<Production>(expression)->addSymbol(expression)->addSymbol(relop)
        ->addSymbol(addition),

        std::make_shared<Production>(expression)->addSymbol(addition),

        std::make_shared<Production>(addition)->addSymbol(addition)->addSymbol(plus)->addSymbol(multiplication),

        std::make_shared<Production>(addition)->addSymbol(multiplication),

        std::make_shared<Production>(multiplication)->addSymbol(multiplication)->addSymbol(mul)->addSymbol(factor),

        std::make_shared<Production>(multiplication)->addSymbol(factor),

        std::make_shared<Production>(factor)->addSymbol(id),
        std::make_shared<Production>(factor)->addSymbol(number),
        std::make_shared<Production>(factor)->addSymbol(true_),
        std::make_shared<Production>(factor)->addSymbol(false_),
        std::make_shared<Production>(factor)->addSymbol(left_paren)->addSymbol(expression)
        ->addSymbol(right_paren),

        std::make_shared<Production>(declaration)->addSymbol(dtype)->addSymbol(assignment),

        std::make_shared<Production>(assignment)->addSymbol(id)->addSymbol(assign)->addSymbol(expression),

        std::make_shared<Production>(init)->addSymbol(declaration),
        std::make_shared<Production>(init)->addSymbol(assignment),
        std::make_shared<Production>(init),

        std::make_shared<Production>(condition)->addSymbol(expression),
        std::make_shared<Production>(condition),

        std::make_shared<Production>(update)->addSymbol(assignment),
        std::make_shared<Production>(update),

        std::make_shared<Production>(dtype)->addSymbol(int_),
        std::make_shared<Production>(dtype)->addSymbol(bool_),

        std::make_shared<Production>(relop)->addSymbol(greater_than),
        std::make_shared<Production>(relop)->addSymbol(greater_equal),
        std::make_shared<Production>(relop)->addSymbol(equal),

    };

    Parser parser;

    void run() {
        parser.addSymbol(start);
        parser.addSymbol(program);
        parser.addSymbol(stmts);
        parser.addSymbol(stmt);
        parser.addSymbol(expression);
        parser.addSymbol(assignment);
        parser.addSymbol(declaration);
        parser.addSymbol(init);
        parser.addSymbol(condition);
        parser.addSymbol(update);
        parser.addSymbol(relop);
        parser.addSymbol(addition);
        parser.addSymbol(multiplication);
        parser.addSymbol(factor);
        parser.addSymbol(dtype);
        
        parser.addSymbol(begin);
        parser.addSymbol(end);
        parser.addSymbol(epsilon);
        parser.addSymbol(semicolon );
        parser.addSymbol(if_); 
        parser.addSymbol(then);
        parser.addSymbol(else_);
        parser.addSymbol(do_);
        parser.addSymbol(while_);
        parser.addSymbol(for_);
        parser.addSymbol(print);
        parser.addSymbol(id); 
        parser.addSymbol(number); 
        parser.addSymbol(int_);
        parser.addSymbol(bool_);
        parser.addSymbol(plus);
        parser.addSymbol(mul);
        parser.addSymbol(left_paren);
        parser.addSymbol(right_paren);
        parser.addSymbol(left_curly);
        parser.addSymbol(right_curly);
        parser.addSymbol(assign);
        parser.addSymbol(greater_than);
        parser.addSymbol(greater_equal);
        parser.addSymbol(equal);
        parser.addSymbol(true_);
        parser.addSymbol(false_);
        parser.addSymbol(dollar);

        parser.addSynchronizingToken(semicolon);

        for (auto prod : productionList) {
            parser.addProduction(prod);
        }
        
        parser.run();

        /*
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
        */
        bool accepted = false;
        bool failed = false;
        for (int token = yylex(); !accepted && !failed;) {
            TokenType token_type = current_token.type;
            if (token == 0) {
                token_type = DOLLAR;
            }

            ParsingState parsingState;

            switch (token_type) {
                case NUMBER:
                    parsingState = parser.readInput(number);
                    break;
                case BEGIN_TOKEN:
                    parsingState = parser.readInput(begin);
                    break;
                case END_TOKEN:
                    parsingState = parser.readInput(end);
                    break;
                case FOR:
                    parsingState = parser.readInput(for_);
                    break;
                case DO:
                    parsingState = parser.readInput(do_);
                    break;
                case WHILE:
                    parsingState = parser.readInput(while_);
                    break;
                case IF:
                    parsingState = parser.readInput(if_);
                    break;
                case THEN:
                    parsingState = parser.readInput(then);
                    break;
                case ELSE:
                    parsingState = parser.readInput(else_);
                    break;
                case PRINT:
                    parsingState = parser.readInput(print);
                    break;
                case INT:
                    parsingState = parser.readInput(int_);
                    break;
                case BOOL:
                    parsingState = parser.readInput(bool_);
                    break;
                case TRUE:
                    parsingState = parser.readInput(true_);
                    break;
                case FALSE:
                    parsingState = parser.readInput(false_);
                    break;
                case MULTIPLY:
                    parsingState = parser.readInput(mul);
                    break;
                case PLUS:
                    parsingState = parser.readInput(plus);
                    break;
                case GREATER_THAN:
                    parsingState = parser.readInput(greater_than);
                    break;
                case GREATER_EQUAL:
                    parsingState = parser.readInput(greater_equal);
                    break;
                case EQUAL:
                    parsingState = parser.readInput(equal);
                    break;
                case ASSIGN:
                    parsingState = parser.readInput(assign);
                    break;
                case SEMICOLON:
                    parsingState = parser.readInput(semicolon);
                    break;
                case LEFT_PAREN:
                    parsingState = parser.readInput(left_paren);
                    break;
                case RIGHT_PAREN:
                    parsingState = parser.readInput(right_paren);
                    break;
                case LEFT_CURLY:
                    parsingState = parser.readInput(left_curly);
                    break;
                case RIGHT_CURLY:
                    parsingState = parser.readInput(right_curly);
                    break;
                case IDENTIFIER:
                    parsingState = parser.readInput(id);
                    break;
                case DOLLAR:
                    parsingState = parser.readInput(dollar);
                    break;
                default:
                    parsingState = ERROR;
                    break;
            }
            
            bool continue_ = false;
            switch (parsingState) {
                case SHIFT:
                    break;
                case REDUCE:
                    continue_ = true;
                    break;
                case ACCEPT:
                    accepted = true;
                    break;
                case ERROR:
                    std::cerr << "Wrong syntax at line " << current_token.line << " column " << current_token.col << "\n";
                    failed = true;
                    break;
                default:
                    std::cout << "Unknown Parsing state!\n";
                break;
            }

            if (continue_) continue; // when REDUCE, not read next token
            token = yylex();
        }
        if (accepted) std::cerr << "Parsing successfully!\n";
        Drawer drawer;
        drawer.drawTree(parser.getParseTree(), "ParseTree");
    }
};

namespace example_grammar {

    std::shared_ptr<NonTerminal> 
        E = std::make_shared<NonTerminal>("E"),
        E_ = std::make_shared<NonTerminal>("E'"),
        T = std::make_shared<NonTerminal>("T"),
        F = std::make_shared<NonTerminal>("F");

    std::shared_ptr<Terminal>
        id = std::make_shared<Terminal>("id"),
        plus = std::make_shared<Terminal>("+"),
        mul = std::make_shared<Terminal>("*"),
        left_paren = std::make_shared<Terminal>("("),
        right_paren = std::make_shared<Terminal>(")"),
        dollar = std::make_shared<Terminal>("$"),
        eps = std::make_shared<Terminal>("eps");

    std::vector<std::shared_ptr<Production>> productionList = {
        std::make_shared<Production>(E_)->addSymbol(E),
        std::make_shared<Production>(E)->addSymbol(E)->addSymbol(plus)->addSymbol(T),
        std::make_shared<Production>(E)->addSymbol(T),
        std::make_shared<Production>(T)->addSymbol(T)->addSymbol(mul)->addSymbol(F),
        std::make_shared<Production>(T)->addSymbol(F),
        std::make_shared<Production>(F)->addSymbol(left_paren)->addSymbol(E)->addSymbol(right_paren),
        std::make_shared<Production>(F)->addSymbol(id),
    };

    Parser parser;

    void run() {
        parser.addSymbol(E_);
        parser.addSymbol(E);
        parser.addSymbol(T);
        parser.addSymbol(F);

        parser.addSymbol(plus);
        parser.addSymbol(mul);
        parser.addSymbol(left_paren);
        parser.addSymbol(right_paren);
        parser.addSymbol(id);
        parser.addSymbol(eps);
        parser.addSymbol(dollar);

        for (auto prod : productionList) {
            parser.addProduction(prod);
        }
        
        parser.run();

        bool accepted = false;
        bool failed = false;
        for (int token = yylex(); !accepted && !failed;) {
            TokenType token_type = current_token.type;
            if (token == 0) {
                token_type = DOLLAR;
            }

            ParsingState parsingState;

            switch (token_type) {
                case IDENTIFIER:
                    parsingState = parser.readInput(id);
                    break;
                case MULTIPLY:
                    parsingState = parser.readInput(mul);
                    break;
                case PLUS:
                    parsingState = parser.readInput(plus);
                    break;
                case LEFT_PAREN:
                    parsingState = parser.readInput(left_paren);
                    break;
                case RIGHT_PAREN:
                    parsingState = parser.readInput(right_paren);
                    break;
                case DOLLAR:
                    parsingState = parser.readInput(dollar);
                    break;
                default:
                    parsingState = ERROR;
                    break;
            }
            
            bool continue_ = false;

            switch (parsingState) {
                case SHIFT:
                    break;
                case REDUCE:
                    continue_ = true;
                    break;
                case ACCEPT:
                    accepted = true;
                    break;
                case ERROR:
                    std::cout << "Wrong syntax at line " << current_token.line << "\n";
                    failed = true;
                    break;
                default:
                    std::cout << "Unknown Parsing state!\n";
                break;
            }

            if (continue_) continue; // when REDUCE, not read next token
            token = yylex();
        }
        if (accepted) std::cout << "Parsing successfully!\n";
    }

    

} 

namespace grammar_4_28 {

    std::shared_ptr<NonTerminal>
        S = std::make_shared<NonTerminal>("S"),
        E = std::make_shared<NonTerminal>("E"),
        E_ = std::make_shared<NonTerminal>("E'"),
        T = std::make_shared<NonTerminal>("T"),
        T_ = std::make_shared<NonTerminal>("T'"),
        F = std::make_shared<NonTerminal>("F");

    std::shared_ptr<Terminal> 
        plus = std::make_shared<Terminal>("+"),
        mul = std::make_shared<Terminal>("*"),
        left_paren = std::make_shared<Terminal>("("),
        right_paren = std::make_shared<Terminal>(")"),
        id = std::make_shared<Terminal>("id"),
        eps = std::make_shared<Terminal>(""),
        dollar = std::make_shared<Terminal>("$");

    std::vector<std::shared_ptr<Production>> productionList = {
        std::make_shared<Production>(S)->addSymbol(E),
        std::make_shared<Production>(E)->addSymbol(T)->addSymbol(E_),
        std::make_shared<Production>(E_)->addSymbol(plus)->addSymbol(T)->addSymbol(E_),
        std::make_shared<Production>(E_)->addSymbol(eps),
        std::make_shared<Production>(T)->addSymbol(F)->addSymbol(T_),
        std::make_shared<Production>(T_)->addSymbol(mul)->addSymbol(F)->addSymbol(T_),
        std::make_shared<Production>(T_)->addSymbol(eps),
        std::make_shared<Production>(F)->addSymbol(left_paren)->addSymbol(E)->addSymbol(right_paren),
        std::make_shared<Production>(F)->addSymbol(id),
    };

    Parser parser;

    void run() {
        parser.addSymbol(S);
        parser.addSymbol(E);
        parser.addSymbol(E_);
        parser.addSymbol(T);
        parser.addSymbol(T_);
        parser.addSymbol(F);

        parser.addSymbol(plus);
        parser.addSymbol(mul);
        parser.addSymbol(left_paren);
        parser.addSymbol(right_paren);
        parser.addSymbol(id);
        parser.addSymbol(eps);
        parser.addSymbol(dollar);

        for (auto prod : productionList) {
            parser.addProduction(prod);
        }
        
        parser.run();
    }


} 