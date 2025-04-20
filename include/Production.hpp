#ifndef PRODUCTION_H
#define PRODUCTION_H

#include "symbol/Symbol.hpp"
#include "symbol/Terminal.hpp"
#include "symbol/NonTerminal.hpp"
#include <vector>
#include <memory>

class Production : public std::enable_shared_from_this<Production>  {
public:
    Production(std::shared_ptr<NonTerminal> head);
    ~Production();
    std::shared_ptr<NonTerminal> getHead() const;
    std::vector<std::shared_ptr<Symbol>> getBody() const;
    std::shared_ptr<Production> addSymbol(std::shared_ptr<Symbol> symbol); 
    bool operator < (const Production& other) const;
    bool operator == (const Production& other) const;
    std::string toString() const;
private:
    std::shared_ptr<NonTerminal> head;
    std::vector<std::shared_ptr<Symbol>> body;
};

std::ostream& operator << (std::ostream& os, const Production& production);

#endif
