#include "symbol/Terminal.hpp"

// Terminal::Terminal() : Symbol() {
//     this->iTerminal = true;
// }

Terminal::Terminal(std::string representation) : Symbol(representation) {
    this->iTerminal = true;
}

Terminal::~Terminal() {
    
}