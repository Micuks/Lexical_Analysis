#ifndef PARSER_H
#define PARSER_H

#include "Symbol.h"
#include "Util.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

const int BUFFER_SIZE = 1024;
const std::string FNAME = "sin";

namespace parser {
using namespace std;

class Lex {
  public:
    Lex(const string &filename);

    ~Lex() {
        try {
            fs.close();
        } catch (ifstream::failure &e) {
            cout << "Exception closing file.\n";
        }
    }
    void process();
    void print();

  private:
    unordered_set<string> keywords;
    ifstream fs;
    SymbolList sl;
    char buffer[BUFFER_SIZE];
    int numLines;
    int numChar;
    char ch;       // store next char
    string bufStr; // store current string
    int pForward;  // forward pointer of buffer
    int pBackward;

    void get_char() {
        pForward = (pForward + 1) % BUFFER_SIZE;
        ch = buffer[pForward];
    }
    bool is_letter() { return (isalpha(ch) || ch == '_'); }
    bool is_digit() { return (ch >= '0' && ch <= '9'); }
    bool is_keyword() { return (keywords.find(bufStr) != keywords.end()); }
    void cat() {
        bufStr.push_back(ch);
    } // concat ch to the end of bufStr.
    void clrStr() { bufStr.clear(); }
    void unget_char() {
        pForward = (pForward - 1) % BUFFER_SIZE;
        // decrease char counter when
        // ch is not EOF
        if (ch != EOF) {
            numChar--;
        }
        // decrease line counter when
        // ch is end of line symbol ';'
        if (ch == ';') {
            numLines--;
        }
    }

    void addSymbol(const string &notation, const string &property = "") {
        Symbol sym(notation, property);
        sl.add(sym);
        cout << sym.toString();
    }

    // log zone
    void logError(const string &err) {
        string msg = "ERROR: " + err + "in L" + to_string(numLines);
        cout << msg << endl;
    }
};
} // namespace parser
#endif
