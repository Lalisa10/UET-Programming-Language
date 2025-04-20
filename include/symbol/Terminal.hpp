#ifndef TERMINAL_H
#define TERMINAL_H

#include "Symbol.hpp"
#include "Token.hpp"

class Terminal : public Symbol {
private:

public:
    //Terminal();
    Terminal(std::string representation);
    ~Terminal();
};

#endif