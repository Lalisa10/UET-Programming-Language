#include "parseTree/ParseTree.hpp"
#include <assert.h>

void ParseTree::addNode(std::shared_ptr<Node> newNode) {
    intermNodes.push(newNode);
}

void ParseTree::reduce(std::shared_ptr<Node> newNode, int noReduceNode) {
    while (noReduceNode --) {
        newNode->addChildren(intermNodes.top());
        intermNodes.pop(); 
    }
    intermNodes.push(newNode);
}

std::shared_ptr<Node> ParseTree::getRoot() const {
    //assert(intermNodes.size() == 1);
    return intermNodes.top();
}

std::stack<std::shared_ptr<Node>> ParseTree::getStack() const {
    return intermNodes;
}