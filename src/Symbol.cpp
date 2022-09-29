#include "Symbol.h"

namespace parser {
using namespace std;

// class Symbol
ostream &operator<<(ostream &os, Symbol &symbol) {
    os << symbol.toStringWithCount();
    return os;
}

// class SymbolList
vector<Symbol>::iterator SymbolList::find(Symbol &symbol) {
    for (auto iter = sl.begin(); iter != sl.end(); iter++) {
        if (symbol == *iter) {
            return iter;
        }
    }
    return sl.end();
}

vector<Symbol>::iterator SymbolList::find(const string &notation,
                                          const string &property) {
    Symbol sym(notation, property);
    return find(sym);
}

void SymbolList::add(Symbol &symbol) {
    auto idxSymbol = find(symbol);
    if (idxSymbol == sl.end()) {
        // Symbol that has never appeared before
        sl.push_back(Symbol(symbol));
        numSymbol++;
    } else {
        // Symbol that has already exists in symbolList
        idxSymbol->incCount();
    }
}

void SymbolList::add(const string &notation, const string &property) {
    Symbol sym(notation, property);
    add(sym);
}

/**
 * find symbol in SymbolList by pattern(notation or property)
 */
Symbol SymbolList::find(const string &pattern) {
    for (auto iter = sl.begin(); iter != sl.end(); iter++) {
        if (iter->getNotation() == pattern || iter->getProperty() == pattern) {
            return *iter;
        }
    }
    return Symbol();
}

Symbol &SymbolList::operator[](const int &i) { return sl[i]; }

ostream &operator<<(ostream &os, SymbolList &symbolList) {
    int numSymbol = symbolList.getNumSymbol();
    os << numSymbol << " symbols" << endl;
    for (int i = 0; i < numSymbol; i++) {
        os << symbolList[i] << endl;
    }
    return os;
}

/**
 * output SymbolList as string
 */
string SymbolList::toString() {
    stringstream ss;
    ss << (*this);
    return ss.str();
}

/**
 * print SymbolList to stdout
 */
void SymbolList::print() { cout << this->toString(); }
} // namespace parser
