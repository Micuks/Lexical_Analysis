#ifndef SYMBOL_H
#define SYMBOL_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace parser {
using namespace std;
class Symbol {
  public:
    Symbol() : notation("NULL"), property("NULL"), count(1) {}
    Symbol(string _notation, string _property, int _count = 1)
        : notation(_notation), property(_property), count(_count) {}
    string getNotation() { return notation; }
    string getProperty() { return property; }
    void incCount() { count++; }
    int getCount() { return count; }
    string toString() {
      return "<" + notation + ", " + property + ">";
    }
    string toStringWithCount() {
        return "<" + notation + ", " + property + ">"
          + " appeared " + to_string(getCount()) + " times";
    }
    bool operator==(const Symbol &rSymbol) {
        // Two symbols have same notation and property
        return
          (notation == rSymbol.notation && property == rSymbol.property);
    }
    friend ostream &operator<<(ostream &os, Symbol &symbol);

  private:
    // symbol' notation
    string notation;
    // symbol's property
    string property;
    // count of symbol appearance time
    int count;
};

class SymbolList {
  public:
    SymbolList() : numSymbol(0) {}
    void add(Symbol &symbol);
    void
    add(const string &notation,
        const string &property);
    vector<Symbol>::iterator find(Symbol &symbol);
    vector<Symbol>::iterator find(const string &notation,
                                  const string &property);
    int getNumSymbol() const { return numSymbol; }
    Symbol find(const string &pattern);
    string toString();
    void print();
    Symbol &operator[](const int &i);
    friend ostream &operator<<(ostream &os, SymbolList &symbolList);

  private:
    vector<Symbol> sl;
    int numSymbol;
};
} // namespace parser
#endif
