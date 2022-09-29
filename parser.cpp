#include "parser.h"

namespace parser {
using namespace std;

int main(int argc, char **argv) {
    ifstream fs;
    Lex lex(FNAME);
    lex.process();
    lex.print();

    return 0;
}
} // namespace parser