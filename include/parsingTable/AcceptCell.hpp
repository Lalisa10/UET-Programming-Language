#ifndef ACCEPT_CELL_H
#define ACCEPT_CELL_H

#include "Cell.hpp"

class AcceptCell : public Cell {
public:
    AcceptCell();
    ~AcceptCell();
    std::string getType() const override;
    std::string toString() const override;
private:

};

#endif