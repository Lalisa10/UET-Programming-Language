#include "parseTree/Node.hpp"

Node::Node(std::string label) : label(label) {

}

void Node::addChildren(std::shared_ptr<Node> children) {
    childrens.push_back(children);
}

std::string Node::getLabel() const {
    return label;
}

std::vector<std::shared_ptr<Node>> Node::getChildrens() const {
    return childrens;
}