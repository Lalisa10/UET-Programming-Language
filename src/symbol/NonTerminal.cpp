#include "symbol/NonTerminal.hpp"

NonTerminal::NonTerminal(std::string representation) : Symbol(representation){
    this->iTerminal = false;
}

NonTerminal::~NonTerminal() {

}