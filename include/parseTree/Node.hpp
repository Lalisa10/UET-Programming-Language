#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <memory>

class Node {
public:
    Node(std::string label);
    void addChildren(std::shared_ptr<Node> children);
    std::string getLabel() const;
    std::vector<std::shared_ptr<Node>> getChildrens() const;
private:
    std::string label;
    std::vector<std::shared_ptr<Node>> childrens;
};

#endif