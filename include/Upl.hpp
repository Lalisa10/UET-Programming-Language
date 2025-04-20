#ifndef UPL_H
#define UPL_H

#include "symbol/Terminal.hpp"
#include "symbol/NonTerminal.hpp"
#include "Item.hpp"
#include "Parser.hpp"
#include <memory>

namespace upl_grammar {

    extern std::shared_ptr<NonTerminal> program,
                stmts,
                stmt,
                expression,
                assignment,
                declaration,
                init,
                condition,
                update,
                relop,
                addition,
                multiplication,
                factor,
                dtype
                ;

    extern std::shared_ptr<Terminal>  begin,
                end,
                epsilon,
                semicolon,
                if_,
                then,
                else_,
                do_,
                while_,
                for_,
                print,
                id,
                number,
                int_,
                bool_,
                plus,
                mul,
                left_paren,
                right_paren,
                left_curly,
                right_curly,
                assign,
                greater_than,
                greater_equal,
                equal,
                true_,
                false_,
                dollar;
    
    extern std::vector<std::shared_ptr<Production>> productionList;

    extern Parser parser;

    void run();
};

namespace example_grammar {
    extern std::shared_ptr<NonTerminal> 
        E,
        E_,
        T,
        T_,
        F;

    extern std::shared_ptr<Terminal> 
        plus,
        mul,
        left_paren,
        right_paren,
        id,
        eps,
        dollar;
        
    extern std::vector<std::shared_ptr<Production>> productionList;

    extern Parser parser;

    void run();
};

namespace grammar_4_28 {
    extern std::shared_ptr<NonTerminal> 
        S,
        E,
        E_,
        T,
        T_,
        F;

    extern std::shared_ptr<Terminal> 
        plus,
        mul,
        left_paren,
        right_paren,
        id,
        eps;
        
    extern std::vector<std::shared_ptr<Production>> productionList;

    extern Parser parser;

    void run();
};
#endif