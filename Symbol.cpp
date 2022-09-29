#include "parser.h"

namespace parser {
using namespace std;

// class Symbol
// class SymbolList
void SymbolList::add(const Symbol &symbol) {
    if ((auto &iter = find(symbol)) == sl.end()) {
        sl.push_back(Symbol(symbol));
    }
}
} // namespace parser
