#include "Lex.h"
#include "Symbol.h"

using namespace parser;
using namespace std;

int main(int argc, char **argv) {
    ifstream fs;
    // 如果命令行参数为空, 则从默认输入文件sin读入待分析源代码
    // 否则读入命令行参数给出的文件作为待分析源代码
    string fName = FNAME;
    if(argc > 1) {
        fName = argv[1];
    }
    Lex lex(fName);
    lex.process();
    lex.print();

    return 0;
}
