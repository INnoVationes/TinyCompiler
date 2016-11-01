//
// Created by liuyu on 16-10-22.
//

#ifndef LEARN_2_UTIL_H
#define LEARN_2_UTIL_H

#include "GLOBALS.h"

void printToken(TokenType token, const char *tokenString);

TreeNode * newNode(NodeKind);

TreeNode * newStmtNode(StmtKind);


TreeNode * newExpNode(ExpKind);




void printTree( TreeNode * );


#endif //LEARN_2_UTIL_H
