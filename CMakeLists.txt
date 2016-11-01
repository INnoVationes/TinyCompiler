cmake_minimum_required(VERSION 3.6)
project(learn_2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES ExplLexicalAnalyzer.h ExplLexicalAnalyzer.cpp ExplParsing.h ExplParsing.cpp util.cpp util.h GLOBALS.h)
add_executable(learn_2 ${SOURCE_FILES})