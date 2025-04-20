#ifndef DRAWER_H
#define DRAWER_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

#include "parseTree/ParseTree.hpp"

class Drawer {
public:
    void drawTable(const std::vector<std::vector<std::string>> &table, std::string fileName);
    void drawTree(ParseTree tree, std::string name);

private:
    std::ofstream out;
    std::vector<size_t> getColumnWidths(const std::vector<std::vector<std::string>>& table);
    void printSeparator(const std::vector<size_t>& colWidths);
    void printRow(const std::vector<std::string>& row, const std::vector<size_t>& colWidths);

    int node_id;
    void write_dot(std::shared_ptr<Node> node, std::ofstream& out, int parent_id);
};

#endif