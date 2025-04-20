#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <iostream>

class Symbol {
protected:
    std::string representation;
    bool iTerminal;

public:
    //Symbol();
    Symbol(std::string representation);
    virtual ~Symbol() = default;
    std::string getRepresentation() const;  
    bool isTerminal() const;
    bool operator < (const Symbol& other) const;
    bool operator != (const Symbol& other) const;
};

std::ostream& operator << (std::ostream& os, const Symbol& symbol);

#endif