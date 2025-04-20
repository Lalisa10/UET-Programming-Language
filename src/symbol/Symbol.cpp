#include "symbol/Symbol.hpp"

// Symbol::Symbol() {
//     this->iTerminal = false;
// }

Symbol::Symbol(std::string representation) : representation(representation), iTerminal(false) {

}

std::string Symbol::getRepresentation() const {
    return representation;
}

bool Symbol::isTerminal() const {
    return iTerminal;
}

bool Symbol::operator < (const Symbol& other) const {
    return representation < other.representation;
}

bool Symbol::operator != (const Symbol& other) const {
    return representation != other.representation;
}

std::ostream& operator << (std::ostream& os, const Symbol& symbol) {
    os << symbol.getRepresentation();
    return os;
}