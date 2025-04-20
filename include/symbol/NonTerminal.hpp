#ifndef NON_TERMINAL_H
#define NON_TERMINAL_H

#include "Symbol.hpp"

class NonTerminal : public Symbol {
private:

public:
    NonTerminal(std::string representation);
    ~NonTerminal();
};

#endif