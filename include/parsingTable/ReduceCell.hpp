#ifndef REDUCE_CELL_H
#define REDUCE_CELL_H

#include "Cell.hpp"

class ReduceCell : public Cell {
public:
    ReduceCell();
    ReduceCell(int productionId);
    ~ReduceCell();
    int getProductionId() const;
    std::string getType() const override;
    std::string toString() const override;
private:
    int productionId;
};

#endif