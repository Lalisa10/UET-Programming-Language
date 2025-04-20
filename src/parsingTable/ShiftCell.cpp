#include "parsingTable/ShiftCell.hpp"

ShiftCell::ShiftCell() {

}

ShiftCell::ShiftCell(int stateId) : stateId(stateId) {

}

ShiftCell::~ShiftCell() {

}

int ShiftCell::getStateId() const {
    return stateId;
}

std::string ShiftCell::getType() const {
    return "shift";
}

std::string ShiftCell::toString() const {
    return "s" + std::to_string(stateId);
}