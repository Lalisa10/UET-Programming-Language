#include "parsingTable/GoToCell.hpp"

GoToCell::GoToCell() {

}

GoToCell::GoToCell(int stateId) : stateId(stateId) {

}

GoToCell::~GoToCell() {

} 

int GoToCell::getStateId() const {
    return stateId;
}

std::string GoToCell::getType() const {
    return "goTo";
}

std::string GoToCell::toString() const {
    return std::to_string(stateId);
}
