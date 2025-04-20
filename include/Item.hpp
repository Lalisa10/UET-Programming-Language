#ifndef ITEM_H
#define ITEM_H

#include "Production.hpp"
#include <memory>

class Item {
public:
    Item();
    Item(std::shared_ptr<Production> production, int dotPos);
    ~Item();
    bool operator < (const Item& other) const;
    bool operator == (const Item& other) const;
    int getDotPos() const;
    std::shared_ptr<Production> getProduction() const;
    std::string toString() const;
private:
    std::shared_ptr<Production> production;
    int dotPos;
};

std::ostream& operator << (std::ostream& os, const Item& item);

#endif