# C 语言词法分析程序

## 概述

- 分别用 C++和 FLEX 设计和实现了一个 C 语言词法分析程序;
- 可以识别出用 C 语言编写的源程序中的每个单词符号, 运算符, 数字, 字符(包括转义字符)等,
  并以<记号, 属性>的形式 输出每个单词符号;
- 可以识别并跳过源程序中的注释;
- 可以统计源程序中的语句行数, 各类单词个数, 字符总数, 并输出统计结果;
- 可以识别 C 语言程序源代码中存在的词法错误, 并报告错误出现的位置;
- 对源程序中出现的错误进行适当的恢复, 让词法分析可以继续进行;
- 对源程序进行一次扫描, 即可检查并报告源程序中存在的所有词法错误, 并输出
  源程序中所有的记号.

## 项目文件结构

```bash
├── CMakeLists.txt
├── docs
│   ├── code_lists
│   │   ├── sin_1
│   │   ├── sin_1_flex
│   │   ├── sin_1_output
│   │   ├── sin_2
│   │   ├── sin_2_flex
│   │   └── sin_2_output
│   ├── content
│   │   ├── 1.tex
│   │   ├── 2.tex
│   │   ├── 3.tex
│   │   ├── 4.tex
│   │   ├── 5.tex
│   │   ├── 6.tex
│   │   ├── 7.tex
│   │   ├── 8.tex
│   │   └── 9.tex
│   ├── figures
│   │   ├── Page0.png
│   │   └── Page1.png
│   ├── report.pdf
│   ├── report.tex
│   └── title
│       ├── logo_bupt.png
│       └── title.tex
├── lex.i
├── lex.yy.c
├── readme.md
└── src
    ├── Lex.cpp
    ├── Lex.h
    ├── Symbol.cpp
    ├── Symbol.h
    ├── Util.h
    └── main.cpp
```
