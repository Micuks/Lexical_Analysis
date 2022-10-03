#include "Lex.h"

namespace parser {
using namespace std;

// Lex
Lex::Lex(const string &filename) : numLines(0), numChar(0), pForward(-1) {
    // load c source file
    try {
        fs.open(FNAME);
    } catch (ifstream::failure &e) {
        cout << "Exception opening file.\n";
    }
    vector<string> vecKeywords(
        {"auto",   "switch",   "case",     "for",    "do",     "while",
         "int",    "char",     "float",    "double", "if",     "else",
         "return", "break",    "continue", "const",  "enum",   "extern",
         "goto",   "register", "restrict", "short",  "signed", "unsigned",
         "sizeof", "static",   "inline",   "struct", "class",  "typedef",
         "union",  "void",     "volatile"});
    keywords = unordered_set<string>(
            vecKeywords.begin(), vecKeywords.end());
};

void Lex::process() {
    // end of analysis process sign
    bool end = false;
    // alias to fs
    ifstream &f = this->fs;
    try {
        f.read(buffer, BUFFER_SIZE - 1);
    } catch (ifstream::failure &e) {
        cout << "Read to buffer failure.\n";
    }
    if (f.gcount() < BUFFER_SIZE - 1) {
        buffer[f.gcount()] = EOF;
    }
    buffer[BUFFER_SIZE - 1] = EOF;

    // initial state
    int state = 0;
    while (!end) {
        // cout << "pForward[" << pForward << "]" << endl;
        get_char();
        // cout << "pForward[" << pForward << "]" << endl;
        // cout << "ch[" << ch << "]" << endl;
        if (ch != EOF) {
            numChar++;
        }
        if (ch == EOF && pForward == BUFFER_SIZE - 1) {
            f.read(buffer, BUFFER_SIZE - 1);
            if (f.gcount() < BUFFER_SIZE - 1) {
                buffer[f.gcount()] = EOF;
            }
            continue;
        }
        // EOF in source file, should not be transparent to Lex
        if (ch == EOF && pForward != BUFFER_SIZE - 1) {
            end = true;
        }
        if (ch == '\n') {
            numLines++;
        }

        switch (state) {
        case 0:
            if (is_letter()) {
                // letter state
                state = 1;
                cat();
            } else if (is_digit()) {
                if (ch != '0') {
                    // integer state
                    state = 2;
                    cat();
                } else if (ch == '0') {
                    // TODO: implement hex, bin and octtal decimal mode.
                    // state =
                    // cat();
                }
            } else {
                switch (ch) {
                case '<':
                    state = 8;
                    break;
                case '>':
                    state = 9;
                    break;
                case '=':
                    state = 10;
                    break;
                case '!':
                    state = 11;
                    break;
                case '&':
                    state = 12;
                    break;
                case '|':
                    state = 13;
                    break;
                case '~':
                    state = 14;
                    break;
                case '+':
                    state = 15;
                    break;
                case '-':
                    state = 16;
                    break;
                case '*':
                    state = 17;
                    break;
                case '/':
                    state = 18;
                    break;
                case '%':
                    state = 19;
                    break;
                case '^':
                    state = 20;
                    break;
                case '(':
                    addSymbol("block_symbol", "(");
                    // TODO case '?'
                    break;
                case ')':
                    addSymbol("block_symbol", ")");
                    break;
                case '{':
                    addSymbol("block_symbol", "{");
                    break;
                case '}':
                    addSymbol("block_symbol", "}");
                    break;
                case '[':
                    addSymbol("block_symbol", "[");
                    break;
                case ']':
                    addSymbol("block_symbol", "]");
                    break;
                case '\'':
                    state = 21;
                    break;
                case '"':
                    state = 22;
                    break;
                case '.':
                    addSymbol("postfix operator", ".");
                case ',':
                    addSymbol("delimeter", ",");
                    break;
                case ';':
                    addSymbol("delimeter", ";");
                    break;
                case ' ':
                case '	':
                case '\n':
                case EOF:
                    break;
                default:
                    stringstream ss;
                    ss << "Illegal char \'" << ch << "\' ";
                    string err = ss.str();
                    logError(err);
                    break;
                }
            }
            break;
        case 1: // Identifer state
            if (is_letter() || is_digit()) {
                cat();
                state = 1;
            } else {
                state = 0;
                if (is_keyword()) {
                    addSymbol("keyword", bufStr);
                } else {
                    addSymbol("identifer", bufStr);
                }
                // Unget last char from buffer.
                unget_char();
                // Clear symbol cache bufStr.
                clrStr();
            }
            break;
        case 2: // Integer state
            if (is_digit()) {
                cat();
                state = 2;
            } else if (ch == '.') {
                cat();
                state = 3;
            } else if (ch == 'E' || ch == 'e') {
                cat();
                state = 5;
            } else {
                state = 0;
                addSymbol("integer", bufStr);
                unget_char();
                clrStr();
            }
            break;
        case 3: // '.' state
            if (is_digit()) {
                cat();
                state = 4;
            } else {
                state = 0;
                // concat 0 to bufStr
                bufStr.push_back('0');
                addSymbol("float", bufStr);
                unget_char();
                clrStr();
            }
            break;
        case 4: // after '.' state
            if (is_digit()) {
                cat();
                state = 4;
            } else if (ch == 'E' || ch == 'e') {
                // exp state
                cat();
                state = 5;
            } else {
                state = 0;
                addSymbol("float", bufStr);
                unget_char();
                clrStr();
            }
            break;
        case 5:
            if (is_digit()) {
                cat();
                state = 7;
            } else if (ch == '+' || ch == '-') {
                cat();
                state = 6;
            } else {
                state = 0;
                logError("Expected exponent");
                unget_char();
                clrStr();
            }
            break;
        case 6:
            if (is_digit()) {
                cat();
                state = 7;
            } else {
                state = 0;
                logError("Expected exponent");
                unget_char();
                clrStr();
            }
            break;
        case 7:
            if (is_digit()) {
                cat();
                state = 7;
            } else {
                state = 0;
                addSymbol("float", bufStr);
                unget_char();
                clrStr();
            }
            break;
        case 8:
            if (ch == '=') {
                addSymbol("relop", "<=");
                state = 0;
            } else if (ch == '<') {
                addSymbol("bitop", "<<");
                state = 0;
            } else {
                addSymbol("relop", "<");
                state = 0;
                unget_char();
            }
            break;
        case 9:
            if (ch == '=') {
                addSymbol("relop", ">=");
                state = 0;
            } else if (ch == '>') {
                addSymbol("bitop", ">>");
                state = 0;
            } else {
                addSymbol("relop", ">");
                state = 0;
                unget_char();
            }
            break;
        case 10:
            if (ch == '=') {
                addSymbol("relop", "==");
                state = 0;
            } else {
                addSymbol("assign-op", "=");
                state = 0;
                unget_char();
            }
            break;
        case 11:
            if (ch == '=') {
                addSymbol("relop", "!=");
                state = 0;
            } else {
                addSymbol("logic-op", "!");
                state = 0;
                unget_char();
            }
            break;
        case 12:
            if (ch == '=') {
                addSymbol("bitop", "&=");
                state = 0;
            } else if (ch == '&') {
                addSymbol("logic-op", "&&");
                state = 0;
            } else {
                addSymbol("bitop", "&");
                state = 0;
                unget_char();
            }
            break;
        case 13:
            if (ch == '=') {
                addSymbol("bitop", "|=");
                state = 0;
            } else if (ch == '|') {
                addSymbol("logic-op", "||");
                state = 0;
            } else {
                addSymbol("bitop", "|");
                state = 0;
                unget_char();
            }
            break;
        case 14:
            if (ch == '=') {
                addSymbol("bitop", "~=");
                state = 0;
            } else {
                addSymbol("bitop", "~");
                state = 0;
                unget_char();
            }
            break;
        case 15:
            if (ch == '+') {
                addSymbol("arith-op", "++");
                state = 0;
            } else if (ch == '=') {
                addSymbol("arith-op", "+=");
                state = 0;
            } else {
                addSymbol("arith-op", "+");
                state = 0;
                unget_char();
            }
            break;
        case 16:
            if (ch == '=') {
                addSymbol("arith-op", "-=");
                state = 0;
            } else if (ch == '-') {
                addSymbol("arith-op", "--");
                state = 0;
            } else {
                addSymbol("arith-op", "-");
                state = 0;
                unget_char();
            }
            break;
        case 17:
            if (ch == '=') {
                addSymbol("arith-op", "*=");
                state = 0;
            } else {
                addSymbol("arith-op", "*");
                state = 0;
                unget_char();
            }
            break;
        case 18:
            if (ch == '/') {
                // "//" comment state
                state = 181;
                break;
            } else if (ch == '*') {
                // "/*" comment state
                state = 182;
                break;
            } else if (ch == '=') {
                addSymbol("arith-op", "/=");
                state = 0;
            } else {
                addSymbol("arith-op", "/");
                state = 0;
                unget_char();
            }
            break;
        case 181:
            // state //
            if (ch == '\n') {
                // end of "//" comment line
                state = 0;
            } else {
                state = 181;
            }
            break;
        case 182:
            // state /*
            if (ch == '*') {
                // transfer to state /* *
                state = 183;
            } else {
                // stay in state /*
                state = 182;
            }
        case 183:
            // state /* *
            if (ch == '/') {
                // state /* */, exit comment state
                state = 0;
            } else if (ch == '*') {
                // state /* ****...., stay in current state
                state = 183;
            } else {
                // fallback to state /*
                state = 182;
            }
            break;
        case 19:
            if (ch == '=') {
                addSymbol("arith-op", "%=");
                state = 0;
            } else {
                addSymbol("arith-op", "%");
                state = 0;
                unget_char();
            }
            break;
        case 20:
            if (ch == '=') {
                addSymbol("bitop", "^=");
                state = 0;
            } else {
                addSymbol("bitop", "^");
                state = 0;
                unget_char();
            }
            break;
        case 21:
            if (ch == '\'') {
                // empty char process
                state = 0;
                logError("Empty char");
                unget_char();
            } else {
                cat();
                state = 211; // check whether next char is '
            }
            break;
        case 211:
            if (ch == '\'') {
                // end of char, may or may not be valid
                if (bufStr.length() == 1) {
                    // valid char, return to state 0
                    state = 0;
                    addSymbol("char", bufStr);
                    clrStr();
                } else {
                    // invalid char, longer than 1 character.
                    state = 0;
                    logError("Char exceeding the limit length ");
                    clrStr();
                }
            } else {
                state = 211;
                cat();
            }
            break;
        case 22:
            if (ch == '"') {
                // ERROR: empty string
                state = 0;
                logError("Empty string");
            } else {
                state = 221;
                cat();
            }
            break;
        case 221:
            if (ch == '"') {
                // end of string
                state = 0;
                addSymbol("string", bufStr);
                clrStr();
            } else {
                // string is not ended
                state = 221;
                cat();
            }
            break;
        default:
            logError("Unknown state");
            break;
        }
    }
}

void Lex::print() {
    // print number of lines, chars
    cout << endl;
    cout << "======Statistic information======" << endl;
    cout << numLines << " lines, " << numChar << " characters, ";
    // print SymbolList
    sl.print();
    cout << "======End of statistic information======" << endl;
}
} // namespace parser
