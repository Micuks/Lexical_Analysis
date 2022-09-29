cmake_minimum_required(VERSION 3.13) # cmake version check
project(Lexical_Analyser)
set(CMAKE_CXX_STANDARD 11) # enable c++11 standard

include_directories(src)

add_executable(Lexical_Analyser
  src/main.cpp
  src/Lex.cpp
  src/Lex.h
  src/Symbol.cpp
  src/Symbol.h)
