#ifndef GO_TO_CELL
#define GO_TO_CELL

#include "Cell.hpp"

class GoToCell : public Cell {
public:
    GoToCell();
    GoToCell(int stateId);
    ~GoToCell();
    int getStateId() const;
    std::string getType() const override;
    std::string toString() const override;
private:
    int stateId;
};

#endif