#ifndef SHIFT_CELL_H
#define SHIFT_CELL_H

#include "Cell.hpp"

class ShiftCell : public Cell {
public:
    ShiftCell();
    ShiftCell(int stateId);
    ~ShiftCell();
    int getStateId() const;
    std::string getType() const override;
    std::string toString() const override;
private:
    int stateId;
};

#endif