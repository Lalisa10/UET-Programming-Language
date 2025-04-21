#ifndef PARSER_H
#define PARSER_H

#include "Production.hpp"
#include "symbol/NonTerminal.hpp"
#include "symbol/Terminal.hpp"
#include "Item.hpp"
#include "parsingTable/ParsingTable.hpp"
#include "parseTree/ParseTree.hpp"

#include <stack>
#include <set>
#include <vector>
#include <map>
#include <utility>

using ItemSet = std::set<Item>;

enum ParsingState {
    SHIFT,
    REDUCE,
    ACCEPT,
    ERROR
} ;

class Parser {
public:
    Parser();
    ~Parser();
    ItemSet getClosure(ItemSet I);
    ItemSet goTo(ItemSet I, std::shared_ptr<Symbol> X);
    void run();
    void addSymbol(std::shared_ptr<Symbol> t);
    // void addNonTerminal(std::shared_ptr<NonTerminal> nt);
    void addProduction(std::shared_ptr<Production> p);
    void addSynchronizingToken(std::shared_ptr<Terminal> symbol);
    std::vector<std::shared_ptr<Production>> getProductionList() const;
    int getNoStates() const;
    int getNoSymbols() const;
    int getNoProductions() const;
    bool isSynchronizingToken(std::shared_ptr<Terminal> symbol) const;
    ParsingState readInput(std::shared_ptr<Terminal> nextInput);
    ParseTree getParseTree() const;
private:
    std::vector<ItemSet> stateList;
    std::vector<std::shared_ptr<Symbol>> symbolList;
    std::vector<std::shared_ptr<Production>> productionList;
    std::vector<std::shared_ptr<Terminal>> synchronizingTokens; // used for panic mode recovery
    std::map<ItemSet, int> stateIndex;
    std::map<Symbol, int> symbolsIndex;
    std::map<Production, int> productionIndex;
    std::vector<std::vector<int>> automatAdj;
    std::vector<std::vector<int>> firstAdj;
    std::vector<std::vector<int>> followAdj;
    std::vector<std::vector<std::shared_ptr<Symbol>>> firstList;
    // firstList[i] = every terminal in FIRST(symbolList[i]) except epsilon,
    // to know whether FIRST(X) contain epsilon, use isEpsable() method.
    std::vector<std::vector<std::shared_ptr<Symbol>>> followList;
    std::vector<bool> epsilonable;
    ParsingTable parsingTable;
    // [i][j] mean action when in state i, meet terminal j-th.
    ParseTree parseTree;
    std::stack<int> stack;
    std::stack<int> recoveryStack;

    void buildCanonicalCollection();
    void buildAutomaton();
    void buildFirst();
    void buildFollow();
    void buildParsingTable();
    void findEpsilon();
    void buildFirstGraph();
    void buildFollowGraph();
    void printParsingTable();
    void initStack();

    std::vector<std::shared_ptr<Symbol>> getFirst(std::shared_ptr<Symbol> symbol);
    std::vector<std::shared_ptr<Symbol>> getFollow(std::shared_ptr<Symbol> symbol);
    bool isEpsable(std::shared_ptr<Symbol>);
};

template<typename T>
std::ostream& operator << (std::ostream& os, const std::set<T>& S) {
    os << "{\n";
    for (auto item : S) {
        os << item << "\n";
    }
    os << "}\n";
    return os;
}

template<typename T>
std::ostream& operator << (std::ostream& os, const std::vector<std::shared_ptr<T>>& vec) {
    os << "[ ";
    for (auto item : vec) {
        os << *item << ", ";
    }
    os << "]\n";
    return os;
}

#endif