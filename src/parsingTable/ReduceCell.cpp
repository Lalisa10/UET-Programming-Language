#include "parsingTable/ReduceCell.hpp"

ReduceCell::ReduceCell() {

}

ReduceCell::ReduceCell(int productionId) : productionId(productionId) {

}

ReduceCell::~ReduceCell() {

}

int ReduceCell::getProductionId() const {
    return productionId;
}

std::string ReduceCell::getType() const {
    return "reduce";
}

std::string ReduceCell::toString() const {
    return "r" + std::to_string(productionId);
}