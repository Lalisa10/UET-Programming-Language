#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "Node.hpp"
#include <stack>

class ParseTree {
public:
    void addNode(std::shared_ptr<Node> newNode); // shift
    void reduce(std::shared_ptr<Node> newNode, int noReduceNode); // reduce
    std::shared_ptr<Node> getRoot() const;
    std::stack<std::shared_ptr<Node>> getStack() const;

private:
    std::shared_ptr<Node> root;
    std::stack<std::shared_ptr<Node>> intermNodes;
};

#endif