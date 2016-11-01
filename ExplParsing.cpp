//
// Created by liuyu on 16-10-22.
//

#include "ExplParsing.h"
#include "GLOBALS.h"
#include "util.h"
#include "ExplLexicalAnalyzer.h"

using namespace std;

static TreeNode *stmt_sequence();

static TreeNode *statement();

static TreeNode *var_stmt();

static TreeNode *if_stmt();

static TreeNode *repeat_stmt();

static TreeNode *assign_stmt();

static TreeNode *read_stmt();

static TreeNode *write_stmt();

static TreeNode *end_stmt();

static TreeNode *exp();

static TreeNode *get_assign_name();

static TreeNode *simple_exp();

static TreeNode *term();

static TreeNode *factor();

TokenType currentToken;
bool then = false;

void getNextToken() {
    currentToken = getToken();
}

void match(TokenType token) {
    if (currentToken == token) getNextToken();
}

void error(string ErrorMessage) {
    printf("\n>>> ");
    printf("Syntax error at line %d: %s", lineno, ErrorMessage.c_str());
}


TreeNode *Begin() {
    TreeNode *Start = newNode(BeginK);
    match(BEGIN);
    TreeNode *temp = stmt_sequence();
    if (Start != NULL) {
        Start->sibling = temp;
    }
    return Start;
}

TreeNode *stmt_sequence() {
    TreeNode *Start = statement();
    TreeNode *temp = Start;
    while (currentToken != ENDFILE && currentToken != END && currentToken != ELSE &&
           currentToken != UNTIL) {
        if (then && currentToken == SEMI) {
            then = false;
            match(SEMI);
            return Start;
        }
        match(SEMI);
        TreeNode *q = statement();
        if (q != NULL) {
            if (Start == NULL) Start = temp = q;
            else {
                temp->sibling = q;
                temp = q;
            }
        }
    }
    return Start;
}

TreeNode *statement() {
    TreeNode *temp = NULL;
    switch (currentToken) {
        case VAR:
            temp = var_stmt();
            break;
        case IF:
            temp = if_stmt();
            break;
        case REPEAT:
            temp = repeat_stmt();
            break;
        case ASSIGN:
            temp = assign_stmt();
            break;
        case READ:
            temp = read_stmt();
            break;
        case WRITE:
            temp = write_stmt();
            break;
        case END:
            temp = end_stmt();
            break;
        case ID:
            temp = assign_stmt();
            break;
        default:
            error("unexpected token -> ");
            break;
    }
    return temp;
}

TreeNode *var_stmt() {
    TreeNode *temp = newStmtNode(VarK);
    match(VAR);
    if (temp != NULL) {
        temp->child[0] = get_assign_name();
        match(DEFINE);
        temp->DT.DT = currentToken;
        match(currentToken);
    }
    return temp;
}


TreeNode *if_stmt() {
    TreeNode *temp = newStmtNode(Ifk);
    match(IF);
    if (temp != NULL) temp->child[0] = exp();
    match(THEN);
    if (temp != NULL) {
        then = true;
        temp->child[1] = stmt_sequence();
    }
    if (currentToken == ELSE) {
        match(ELSE);
        if (temp != NULL) temp->child[2] = stmt_sequence();
    }
    return temp;
}

TreeNode *repeat_stmt() {
    TreeNode *temp = newStmtNode(RepeatK);
    match(REPEAT);
    if (temp != NULL) temp->child[0] = stmt_sequence();
    match(UNTIL);
    if (temp != NULL) temp->child[1] = exp();
    return temp;
}

TreeNode *assign_stmt() {
    TreeNode *temp = newStmtNode(AssignK);
    if ((temp != NULL) && (currentToken == ID))
        temp->attr.name = tokenString;
    match(ID);
    match(ASSIGN);
    if (temp != NULL) temp->child[0] = exp();
    return temp;
}

TreeNode *read_stmt() {
    TreeNode *temp = newStmtNode(ReadK);
    match(READ);
    match(LPAREN);
    if (temp != NULL && currentToken == ID)
        temp->attr.name = tokenString;
    match(ID);
    match(RPAREN);
    return temp;
}

TreeNode *write_stmt() {
    TreeNode *temp = newStmtNode(WriteK);
    match(WRITE);
    match(LPAREN);
    if (temp != NULL && currentToken == ID)
        temp->attr.name = tokenString;
    match(ID);
    match(RPAREN);
    return temp;
}

TreeNode *end_stmt() {
    match(END);
    TreeNode *temp = newNode(EndK);
    return temp;
}

TreeNode *exp() {
    TreeNode *temp = simple_exp();
    if ((currentToken == LT) || (currentToken == EQ)) {
        TreeNode *p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = temp;
            p->attr.op = currentToken;
            temp = p;
        }
        match(currentToken);
        if (temp != NULL)
            temp->child[1] = simple_exp();
    }
    return temp;
}

TreeNode *get_assign_name() {
    TreeNode *temp = newExpNode(IdK);
    if (currentToken != DEFINE) {
        if (temp != NULL) {
            if (currentToken == COMMA)
                match(COMMA);
            if (currentToken == ID) {
                temp->attr.name = tokenString;
                match(ID);
                if (currentToken == ID || currentToken == COMMA)
                    temp->child[0] = get_assign_name();
            }
        }
    }
    return temp;
}


TreeNode *simple_exp() {
    TreeNode *temp = term();
    while ((currentToken == PLUS) || (currentToken == MINUS)) {
        TreeNode *p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = temp;
            p->attr.op = currentToken;
            temp = p;
            match(currentToken);
            temp->child[1] = term();
        }
    }
    return temp;
}

TreeNode *term() {
    TreeNode *temp = factor();
    while ((currentToken == TIMES) || (currentToken == OVER)) {
        TreeNode *p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = temp;
            p->attr.op = currentToken;
            temp = p;
            match(currentToken);
            temp->child[1] = factor();
        }
    }
    return temp;
}

TreeNode *factor() {
    TreeNode *temp = NULL;
    switch (currentToken) {
        case NUM:
            temp = newExpNode(ConstK);
            if ((temp != NULL) && (currentToken == NUM)) {
                temp->attr.val = atoi(tokenString);
                match(NUM);
            }
            break;
        case ID:
            temp = newExpNode(IdK);
            if ((temp != NULL) && (currentToken == ID)) {
                temp->attr.name = tokenString;
                match(ID);
                break;
            }
        case LPAREN:
            match(LPAREN);
            temp = exp();
            match(RPAREN);
            break;
        default:
            error("unexpected token -> ");
            printToken(currentToken, tokenString);
            currentToken = getToken();
            break;
    }
    return temp;
}

int main() {
    TreeNode *temp;
    set_up();
    currentToken = getToken();
    temp = Begin();
    printTree(temp);
    if (currentToken != ENDFILE)
        error("Code ends before file\n");
}