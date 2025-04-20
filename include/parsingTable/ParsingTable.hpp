#ifndef PARSING_TABLE_H
#define PARSING_TABLE_H

#include "GoToCell.hpp"
#include "ReduceCell.hpp"
#include "ShiftCell.hpp"
#include "AcceptCell.hpp"

#include <vector>
#include <memory>

class ParsingTable {
public:
    ParsingTable();
    ParsingTable(int numRow, int numCol);
    ~ParsingTable();
    bool addShift(int r, int c, int stateId);
    bool addReduce(int r, int c, int countSymbol);
    bool addGoTo(int r, int c, int stateId);
    bool addAccept(int r, int c);
    std::shared_ptr<Cell> getCell(int r, int c) const;
    
private:
    std::vector<std::vector<std::shared_ptr<Cell>>> table;
};
#endif