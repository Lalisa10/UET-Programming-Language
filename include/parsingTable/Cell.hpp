#ifndef CELL_H
#define CELL_H

#include <string>

class Cell {
public:
    Cell();
    virtual ~Cell() = default;
    virtual std::string getType() const;
    virtual std::string toString() const; 
private:

};

#endif