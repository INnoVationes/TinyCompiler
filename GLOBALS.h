//
// Created by liuyu on 16-10-22.
//

#ifndef LEARN_2_GLOBALS_H
#define LEARN_2_GLOBALS_H

#include <iostream>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>

#define MAXTOKENLEN 40
#define MAXRESERVED 13
#define MAXCHILDREN 3

static int lineno = 0;


typedef enum {
    ENDFILE, ERROR,
    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE, VAR, BEGIN, INTEGER, DOUBLE, STRING,
    ID, NUM,
    ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI, COMMA, DEFINE
} TokenType;

typedef struct {
    TokenType kind;
    int row = -1;
    int column = -1;
    double value;
    std::string ID;
} Token;

typedef enum { BeginK , StmtK, ExpK , EndK } NodeKind;//基本结构类型
typedef enum { VarK,Ifk, RepeatK, AssignK, ReadK, WriteK } StmtKind;//语句类型
typedef enum { OpK, ConstK, IdK } ExpKind;//表达式类型
typedef enum {Void,Interger,Boolean}ExpType;//表达式类型类型

typedef struct treeNode {
    struct treeNode *child[3];//语法节点
    struct treeNode *sibling;//下一个语法节点

    int lineno;
    NodeKind nodeKind;
    union{ StmtKind stmt; ExpKind exp;} Kind;
    union{ TokenType op; int val; std::string name; }attr;
    union{ TokenType DT; std::string name; }DT;
    ExpType type;
}TreeNode;

#endif //LEARN_2_GLOBALS_H
