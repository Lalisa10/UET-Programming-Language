#include "Item.hpp"

Item::Item() {

}

Item::Item(std::shared_ptr<Production> production, int dotPos) : production(production), dotPos(dotPos) {

}

Item::~Item() {

}

bool Item::operator < (const Item& other) const {
    if (dotPos != other.dotPos) return dotPos < other.dotPos;
    return *production < *other.production;
}

bool Item::operator == (const Item& other) const {
    return dotPos == other.dotPos && *production == *other.production;
}

int Item::getDotPos() const {
    return dotPos;
}

std::shared_ptr<Production> Item::getProduction() const {
    return production;
}

std::string Item::toString() const {
    
    std::string res = this->getProduction()->getHead()->getRepresentation() + "->";

    std::vector<std::shared_ptr<Symbol>> body = this->getProduction()->getBody();

    for (int i = 0; i < body.size(); i ++) {
        res += (i == dotPos ? "." : " ") + body[i]->getRepresentation();
    }

    if (dotPos == body.size()) res += ".";

    return res;
}

std::ostream& operator << (std::ostream& os, const Item& item) {
    os << item.toString();
    return os;
}