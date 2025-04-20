#include "parsingTable/AcceptCell.hpp"

AcceptCell::AcceptCell() {

}

AcceptCell::~AcceptCell() {

}

std::string AcceptCell::getType() const {
    return "accept";
}

std::string AcceptCell::toString() const {
    return "acc";
}
