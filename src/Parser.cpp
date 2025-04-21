#include "Parser.hpp"
#include "Drawer.hpp"

#include <queue>
#include <assert.h>
#include <functional>
#include <algorithm>
#include <iomanip>

Parser::Parser() {

}

Parser::~Parser() {

}

void Parser::run() {
    findEpsilon();
    buildFirst();
    buildFollow();
    buildParsingTable();
    printParsingTable();
    initStack();
}


ParsingState Parser::readInput(std::shared_ptr<Terminal> nextInput) {

    int currentState = stack.top();
    std::shared_ptr<Cell> behavior = parsingTable.getCell(currentState, symbolsIndex[*nextInput]);

    if (!behavior) {
        std::vector<std::shared_ptr<Symbol>> recommendList;

        for (auto symbol : symbolList) {
            if (!symbol->isTerminal()) continue;
            std::shared_ptr<Cell> cell = parsingTable.getCell(currentState, symbolsIndex[*symbol]);
            if (!cell) continue;
            recommendList.push_back(symbol);
        }

        std::cerr << "Expected ";
        std::cerr << "'" << *recommendList[0] << "'";
        for (int i = 1; i < recommendList.size(); i ++) std::cerr << " or " << "'" << *recommendList[i] << "'";
        std::cerr << " but found " << "'" << *nextInput << "'\n";

        //stack = recoveryStack;
        return ERROR;
    }

    // if (isSynchronizingToken(nextInput)) {
    //     recoveryStack = stack;
    // }

    if (behavior->getType() == "shift") {

        std::shared_ptr<ShiftCell> shiftCell = std::dynamic_pointer_cast<ShiftCell>(behavior);
        int nextState = shiftCell->getStateId();
        stack.push(nextState);
        parseTree.addNode(std::make_shared<Node>(nextInput->getRepresentation()));

        auto stack = parseTree.getStack();

        return SHIFT;

    } else if (behavior->getType() == "reduce") {
        std::shared_ptr<ReduceCell> reduceCell = std::dynamic_pointer_cast<ReduceCell>(behavior);
        int productionId = reduceCell->getProductionId();
        int bodySize = productionList[productionId]->getBody().size();
        auto head = productionList[productionId]->getHead();
        
        if (bodySize == 0) { // epsilon
            parseTree.addNode(std::make_shared<Node>("ε"));
            parseTree.reduce(std::make_shared<Node> (head->getRepresentation()), 1);
        } else {
            parseTree.reduce(std::make_shared<Node> (head->getRepresentation()), bodySize);
        }
        

        while (bodySize --) {
            stack.pop();
        }

        int topState = stack.top();
        
        std::shared_ptr<Cell> nextStateCell = parsingTable.getCell(topState, symbolsIndex[*head]);

        std::shared_ptr<GoToCell> goToCell;
        try {
            goToCell = std::dynamic_pointer_cast<GoToCell>(nextStateCell);
        } catch(...) {
            std::cerr << "Invalid GoToCell at (" << topState << ", " << symbolsIndex[*head] << ")\n"; 
        }
        
        stack.push(goToCell->getStateId());
        return REDUCE;
    } else if (behavior->getType() == "accept") {
        parseTree.reduce(std::make_shared<Node> ("start"), 1);
        auto stack = parseTree.getStack();
        return ACCEPT;
    }
    assert(1);
    return ERROR;
}

void Parser::buildCanonicalCollection() {

    ItemSet I0;

    std::shared_ptr<Production> firstProduction = productionList[0];
    Item firstItem(firstProduction, 0);
    I0.insert(firstItem);
    I0 = getClosure(I0);

    stateIndex[I0] = 0;

    std::queue<ItemSet> qu; 
    qu.push(I0);
    while (!qu.empty()) {
        ItemSet itemSet = qu.front();
        qu.pop();
        for (std::shared_ptr<Symbol> symbol : symbolList) {
            ItemSet nextItemSet = goTo(itemSet, symbol);
            if (nextItemSet.empty()) continue;
            if (stateIndex.find(nextItemSet) == stateIndex.end()) {
                stateIndex[nextItemSet] = getNoStates();
                qu.push(nextItemSet);
            }
        }
    }

    stateList.resize(getNoStates());
    for (auto [itemSet, id] : stateIndex) {
        stateList[id] = itemSet;
    }
}

void Parser::buildAutomaton() {
    automatAdj = std::vector<std::vector<int>> (getNoStates(), std::vector<int> (getNoSymbols(), -1));
    for (auto [itemSet, id] : stateIndex) {
        for (auto symbol : symbolList) {
            ItemSet nextItemSet = goTo(itemSet, symbol);
            if (nextItemSet.empty()) continue;
            automatAdj[id][symbolsIndex[*symbol]] = stateIndex[nextItemSet];
        }
    }
}

void Parser::buildFirst() {

    buildFirstGraph();
    firstList.resize(symbolList.size());
    for (auto symbol : symbolList) {
        int id = symbolsIndex[*symbol];
        firstList[id] = getFirst(symbol);
        //std::cout << "FIRST(" << *symbol << "):" << firstList[id] << "\n";
    }
}

void Parser::buildFollow() {
    buildFollowGraph();
    followList.resize(symbolList.size());
    for (auto symbol : symbolList) {
        if (symbol->isTerminal()) continue;
        int id = symbolsIndex[*symbol];
        followList[id] = getFollow(symbol);
        //std::cout << "FOLLOW(" << *symbol << "):" << followList[id] << "\n";
    }
}

void Parser::buildParsingTable() {
    buildCanonicalCollection();
    buildAutomaton();
    parsingTable = ParsingTable(getNoStates(), getNoSymbols());
    //std::cerr << getNoStates() << " " << getNoSymbols() << "\n";
     /*
     (a) If [A -> (alpha).a(beta) ] is in Ii and GOTO(I_i,a) = I_j , then set ACTION[i, a] to
     "shift j". Here a must be a terminal.
     */
    for (ItemSet is : stateList) {
        for (Item item : is) {
            int dotPos = item.getDotPos();
            auto production = item.getProduction();
            auto body = production->getBody();
            if (dotPos >= body.size()) continue;
            if (!body[dotPos]->isTerminal()) continue;

            auto terminal = body[dotPos];
            int row = stateIndex[is], col = symbolsIndex[*terminal];
            bool isAddingSuccess = parsingTable.addShift(row, col, automatAdj[row][col]);
            
            //std::cerr << item << " " << row << " " << *terminal << " shift" << automatAdj[row][col] << "\n";
        }
    }
    /*
    (b) If [A->\alpha.] is in I_i, then set ACTION[i, a] to "reduce A->\alpha" for all a in FOLLOW(A);
    here A may not not be S'. 
    */
    for (ItemSet is : stateList) {
        for (Item item : is) {
            int dotPos = item.getDotPos();
            auto production = item.getProduction();
            auto body = production->getBody();
            auto head = production->getHead();
            int head_id = symbolsIndex[*head];

            if (dotPos != body.size()) continue;

            for (auto terminal : followList[head_id]) {
                int row = stateIndex[is], col = symbolsIndex[*terminal];
                int prod_id = productionIndex[*production];
                parsingTable.addReduce(row, col, prod_id);
            }
        }
    }
    /*
    (c) If [S'->S.] is in I_i, then set ACTION[i, $] to "accept"
    */
    std::shared_ptr<Production> firstProduction = productionList[0];
    Item acceptItem(firstProduction, firstProduction->getBody().size());
    for (ItemSet is : stateList) {
        for (Item item : is) {
            if (item == acceptItem) {
                int row = stateIndex[is], col = -1;

                for (auto symbol : symbolList) {
                    if (symbol->getRepresentation() == "$") {
                        assert(col == -1);
                        col = symbolsIndex[*symbol];
                    }
                }

                assert(col != -1);
                parsingTable.addAccept(row, col);
            }
        }
    }

    // add GoTo to parsingTable
    for (auto [itemSet, id] : stateIndex) {
        for (auto symbol : symbolList) {
            int nextState = automatAdj[id][symbolsIndex[*symbol]];
            if (!symbol->isTerminal() && nextState != -1)
                parsingTable.addGoTo(id, symbolsIndex[*symbol], nextState);
        }
    }
}

void Parser::printParsingTable() {
    
    std::vector<std::vector<std::string>> table(1 + getNoStates());

    table[0].push_back("");
    for (int i = 0; i < symbolList.size(); i ++) {
        table[0].push_back(std::to_string(i) + ":" + symbolList[i]->getRepresentation());
    }

    for (int i = 0; i < stateList.size(); i ++) {
        table[i + 1].push_back(std::to_string(i));
        for (int j = 0; j < symbolList.size(); j ++) {
            auto cell = parsingTable.getCell(i, j);
            if (cell)
                table[i + 1].push_back(cell->toString());
            else
                table[i + 1].push_back("");
        }
    }

    Drawer drawer;
    drawer.drawTable(table, "Parsing Table.txt");
}

void Parser::initStack() {
    stack.push(0);
}

void Parser::findEpsilon() {
    epsilonable.assign(symbolList.size(), false);

    for (auto production : productionList) {
        auto body = production->getBody();
        auto head = production->getHead();
        if (body.empty()) {
            int headId = symbolsIndex[*head];
            epsilonable[headId] = true;
        }
    }

    bool found;
    do {
        found = false;
        for (auto production : productionList) {
            bool check = true;
            auto body = production->getBody();
            auto head = production->getHead();
            int headId = symbolsIndex[*head];

            if (epsilonable[headId]) continue;

            for (auto symbol : body) {
                int id = symbolsIndex[*symbol];
                check &= (!symbol->isTerminal() && epsilonable[id]);
            }

            if (check) {
                found = true;
                epsilonable[headId] = true;
            } 
        }
    } while(found);
}

void dfs(int u, std::vector<bool> &visited, std::vector<std::vector<int>> adj) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v]) {
            dfs(v, visited, adj);
        }
    }
}

std::vector<std::shared_ptr<Symbol>> Parser::getFirst(std::shared_ptr<Symbol> symbol) {
    int id = symbolsIndex[*symbol];
    std::vector<bool> visited(symbolList.size(), false);

    dfs(id, visited, firstAdj);

    std::vector<std::shared_ptr<Symbol>> res;
    for (int i = 0; i < visited.size(); i ++) {
        if (visited[i] && symbolList[i]->isTerminal() && symbolList[i]->getRepresentation() != "eps") {
            res.push_back(symbolList[i]);
        }
    }

    return res;
}

std::vector<std::shared_ptr<Symbol>> Parser::getFollow(std::shared_ptr<Symbol> symbol) {
    std::vector<std::shared_ptr<Symbol>> res;

    if (symbol->isTerminal()) return res;

    int id = symbolsIndex[*symbol];
    std::vector<bool> visited(symbolList.size(), false);

    dfs(id, visited, followAdj);

    for (int i = 0; i < visited.size(); i ++) {
        if (visited[i] && symbolList[i]->isTerminal()) {
            res.push_back(symbolList[i]);
        }
    }

    return res;
}
void Parser::buildFirstGraph() {
    firstAdj = std::vector<std::vector<int>> (getNoSymbols(), std::vector<int> ());
    for (auto production : productionList) {
        auto head = production->getHead();
        auto body = production->getBody();
        int headId = symbolsIndex[*head];
        for (auto symbol : body) {
            int id = symbolsIndex[*symbol];
            firstAdj[headId].push_back(id);
            if (!isEpsable(symbol)) {
                break;
            }
        }
    }
}

void Parser::buildFollowGraph() {
    followAdj = std::vector<std::vector<int>> (getNoSymbols(), std::vector<int> ());

    // Rule 1: Place $ in FOLLOW(S), where S is the start symbol
    for (int i = 0; i < symbolList.size(); i ++) {
        if (symbolList[i]->getRepresentation() == "$") {
            followAdj[0].push_back(i); // Index of start symbol is always 0 conventionally
        }
    }
    // Rule 2: if there is a production A->aBβ, then everything in FIRST(β) except ε is in FOLLOW(B)
    for (auto production : productionList) {
        auto body = production->getBody();
        for (int i = 0; i + 1 < body.size(); i ++) {
            auto B = body[i];
            int B_id = symbolsIndex[*B];
            if (B->isTerminal()) continue;
            for (int j = i + 1; j < body.size(); j ++) {
                auto beta = body[j];
                int beta_id = symbolsIndex[*beta];
                for (auto follow : firstList[beta_id]) {
                    assert(follow->getRepresentation() != "eps");
                    followAdj[B_id].push_back(symbolsIndex[*follow]);
                    //std::cerr << *B << " is followed by " << *follow << "\n";
                }
                if (!isEpsable(beta)) {
                    break;
                }
            }
        }
    }
    // Rule 3: if there is a production A->αB or A->αBβ, where FIRST(β) contains ε, then everything in
    // FOLLOW(A) is in FOLLOW(B)
    for (auto production : productionList) {
        auto A = production->getHead();
        int A_id = symbolsIndex[*A];
        auto body = production->getBody();
        bool check = true;
        for (int i = body.size() - 1; i >= 0 && check; i --) {
            auto B = body[i];
            if (B->isTerminal()) break;
            int B_id = symbolsIndex[*B];
            followAdj[B_id].push_back(A_id);
            // std::cerr << "FOLLOW(" << B->getRepresentation() << ") contains FOLLOW(" << A->getRepresentation() << ")\n";
            // if (B->getRepresentation() == "E'" && A->getRepresentation() == "E") {
            //     std::cerr << "E' can be epsilon: " << isEpsable(B) << "\n";
            // }
            check &= isEpsable(B);
        }
    }

    // Unique adjacent list
    for (auto &adj : followAdj) {
        std::sort(adj.begin(), adj.end());
        adj.erase(std::unique(adj.begin(), adj.end()), adj.end());
    }

}


ItemSet Parser::getClosure(ItemSet I) {

    ItemSet res;

    for (auto item : I) {
        res.insert(item);
    }

    std::map<std::string, bool> added;
    bool found = true;
    while (found) {
        found = false;
        std::vector<Item> newItems;

        for (Item i : res) {
            int dotPos = i.getDotPos();
            std::shared_ptr<Production> production = i.getProduction();
            std::vector<std::shared_ptr<Symbol>> body = production->getBody();

            if (dotPos >= body.size()) {
                continue;
            }

            if (body[dotPos]->isTerminal()) {
                continue;
            }

            std::shared_ptr<NonTerminal> newItemsHead = 
                std::dynamic_pointer_cast<NonTerminal> (body[dotPos]);
            if (added.find(newItemsHead->getRepresentation())
                == added.end()) {

                    for (std::shared_ptr<Production> newProduction : productionList) {
                        if (newProduction->getHead() == newItemsHead) {
                            Item newItem = Item(newProduction, 0);
                            newItems.push_back(newItem);
                        }
                    }
                    added[newItemsHead->getRepresentation()] = true;
                    found = true;
            } else {

            }
        }

        for (Item item : newItems) {
            res.insert(item);
        }
    }
    return res;
}

ItemSet Parser::goTo(ItemSet I, std::shared_ptr<Symbol> X) {

    ItemSet res;

    for (Item item : I) {

        std::shared_ptr<Production> production = item.getProduction();
        std::vector<std::shared_ptr<Symbol>> body = production->getBody();
        int dotPos = item.getDotPos();

        if (dotPos >= body.size()) continue;

        if (body[dotPos]->getRepresentation() == X->getRepresentation()) {
            Item newItem(production, dotPos + 1);
            res.insert(newItem);
        }

    }

    res = getClosure(res);
    return res;
}

bool Parser::isEpsable(std::shared_ptr<Symbol> symbol) {
    int id = symbolsIndex[*symbol];
    return epsilonable[id];
}


void Parser::addSymbol(std::shared_ptr<Symbol> symbol) {
    symbolsIndex[*symbol] = getNoSymbols();
    symbolList.push_back(symbol);
}

void Parser::addProduction (std::shared_ptr<Production> p) {
    productionIndex[*p] = getNoProductions();
    productionList.push_back(p);
}

std::vector<std::shared_ptr<Production>> Parser::getProductionList() const {
    return productionList;
}

int Parser::getNoStates() const {
    return stateIndex.size();
}

int Parser::getNoSymbols() const {
    return symbolList.size();
}

int Parser::getNoProductions() const {
    return productionList.size();
}

ParseTree Parser::getParseTree() const {
    return parseTree;
}

void Parser::addSynchronizingToken(std::shared_ptr<Terminal> symbol) {
    synchronizingTokens.push_back(symbol);
}

bool Parser::isSynchronizingToken(std::shared_ptr<Terminal> symbol) const {
    for (auto s : synchronizingTokens) {
        if (s->getRepresentation() == symbol->getRepresentation()) return true;
    }
    return false;
}