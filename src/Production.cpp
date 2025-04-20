#include "../include/Production.hpp"

Production::Production(std::shared_ptr<NonTerminal> head) : head(head) {}

Production::~Production() {}

std::shared_ptr<NonTerminal> Production::getHead() const {
    return head;
}

std::vector<std::shared_ptr<Symbol>> Production::getBody() const {
    return body;
}

std::shared_ptr<Production> Production::addSymbol(std::shared_ptr<Symbol> symbol) {
    this->body.push_back(symbol);
    return shared_from_this();
}

bool Production::operator < (const Production& other) const {
    if (*head != *other.head) {
        return head < other.head;
    }
    return body < other.body;
}

bool Production::operator == (const Production& other) const {
    return head == other.head && body == other.body;
}

std::string Production::toString() const {
    std::string res = getHead()->getRepresentation() + " ->";
    for (std::shared_ptr<Symbol> symbol : body) {
        res += " " + symbol->getRepresentation();
    }
    return res;
}

std::ostream& operator << (std::ostream& os, const Production& production) {
    os << production.toString();
    return os;
}