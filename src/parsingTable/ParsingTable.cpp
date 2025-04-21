#include "parsingTable/ParsingTable.hpp"
#include <iostream>

ParsingTable::ParsingTable() {

}

ParsingTable::ParsingTable(int r, int c) {
    table = std::vector<std::vector<std::shared_ptr<Cell>>> (r, std::vector<std::shared_ptr<Cell>> (c, nullptr));
}

ParsingTable::~ParsingTable() {

}

bool ParsingTable::addGoTo(int r, int c, int stateId) {
    if (table[r][c]) {
        std::cout << "Can't replace cell (" << r << ", " << c << ") with GoTo(" << stateId << ")! Already " << table[r][c]->toString() << "\n";
        return false;
    }
    table[r][c] = std::make_shared<GoToCell>(stateId);
    return true;
}

bool ParsingTable::addShift(int r, int c, int stateId) {
    if (table[r][c]) {
        std::cout << "Can't replace cell (" << r << ", " << c << ") with s" << stateId << "! Already " << table[r][c]->toString() << "\n";
        return false;
    }
    table[r][c] = std::make_shared<ShiftCell>(stateId);
    return true;
}

bool ParsingTable::addReduce(int r, int c, int productionId) {
    if (table[r][c]) {
        std::cout << "Can't replace cell (" << r << ", " << c << ") with r" << productionId << "! Already " << table[r][c]->toString() << "\n";
        return false;
    }
    table[r][c] = std::make_shared<ReduceCell>(productionId);
    return true;
}

bool ParsingTable::addAccept(int r, int c) {
    if (table[r][c]) {
        std::cout << "Cell (" << r << ", " << c << ") is not null but still okay!" << "\n";
    }
    table[r][c] = std::make_shared<AcceptCell>();
    return true;
}

std::shared_ptr<Cell> ParsingTable::getCell(int r, int c) const {
    return table[r][c];
}
