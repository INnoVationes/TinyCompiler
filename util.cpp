//
// Created by liuyu on 16-10-22.
//
#include "util.h"
#include "GLOBALS.h"

void printToken(TokenType token, const char *tokenString) {
    switch (token) {
        case IF:
        case THEN:
        case ELSE:
        case END:
        case REPEAT:
        case UNTIL:
        case READ:
        case WRITE:
        case BEGIN:
        case VAR:
        case INTEGER:
        case DOUBLE:
        case STRING:
            printf("reserved word: %s\n", tokenString);
            break;
        case DEFINE:
            printf(":\n");
            break;
        case COMMA:
            printf(",\n");
            break;
        case ASSIGN:
            printf(":=\n");
            break;
        case LT:
            printf("<\n");
            break;
        case EQ:
            printf("=\n");
            break;
        case LPAREN:
            printf("(\n");
            break;
        case RPAREN:
            printf(")\n");
            break;
        case SEMI:
            printf(";\n");
            break;
        case PLUS:
            printf("+\n");
            break;
        case MINUS:
            printf("-\n");
            break;
        case TIMES:
            printf("*\n");
            break;
        case OVER:
            printf("/\n");
            break;
        case ENDFILE:
            break;
        case NUM:
            printf("NUM, val= %s\n", tokenString);
            break;
        case ID:
            printf("ID, name= %s\n", tokenString);
            break;
        case ERROR:
            printf("ERROR: %s\n", tokenString);
            break;
        default:
            printf("Unknown token: %d\n", token);
    }
}

TreeNode *newNode(NodeKind nodekind) {
    TreeNode *temp = (TreeNode *) malloc(sizeof(TreeNode));
    for (int i = 0; i < MAXCHILDREN; i++) temp->child[i] = NULL;
    temp->sibling = NULL;
    temp->nodeKind = nodekind;
    temp->lineno = lineno;
    return temp;
}


TreeNode *newStmtNode(StmtKind kind) {
    TreeNode *temp = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (temp == NULL)
        printf("Out of memory error at line %d\n", lineno);
    else {
        for (i = 0; i < MAXCHILDREN; i++) temp->child[i] = NULL;
        temp->sibling = NULL;
        temp->nodeKind = StmtK;
        temp->Kind.stmt = kind;
        temp->lineno = lineno;
    }
    return temp;
}

TreeNode *newExpNode(ExpKind kind) {
    TreeNode *temp = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (temp == NULL)
        printf("Out of memory error at line %d\n", lineno);
    else {
        for (i = 0; i < MAXCHILDREN; i++) temp->child[i] = NULL;
        temp->sibling = NULL;
        temp->nodeKind = ExpK;
        temp->Kind.exp = kind;
        temp->lineno = lineno;
        temp->type = Void;
    }
    return temp;
}


void printTree(TreeNode *tree) {
    int i;
    while (tree != NULL) {
        if (tree->nodeKind == BeginK) printf("Begin\n");
        else if (tree->nodeKind == EndK)printf("End\n");
        else if (tree->nodeKind == StmtK) {
            switch (tree->Kind.stmt) {
                case VarK:
                    printf("Define variable as ");
                    switch (tree->DT.DT) {
                        case INTEGER:
                            printf("Int\n");
                            break;
                        case DOUBLE:
                            printf("Double\n");
                            break;
                        case STRING:
                            printf("String\n");
                            break;
                    }
                    break;
                case Ifk:
                    printf("If\n");
                    break;
                case RepeatK:
                    printf("Repeat\n");
                    break;
                case AssignK:
                    printf("Assign to: %s\n", tree->attr.name.c_str());
                    break;
                case ReadK:
                    printf("Read: %s\n", tree->attr.name.c_str());
                    break;
                case WriteK:
                    printf("Write: %s\n", tree->attr.name.c_str());
                    break;
                default:
                    printf("Unknown ExpNode kind\n");
                    break;
            }
        } else if (tree->nodeKind == ExpK) {
            switch (tree->Kind.exp) {
                case OpK:
                    printf("Op: ");
                    printToken(tree->attr.op, "\0");
                    break;
                case ConstK:
                    printf("Const: %d\n", tree->attr.val);
                    break;
                case IdK:
                    printf("Id: %s\n", tree->attr.name.c_str());
                    break;
                default:
                    printf("Unknown ExpNode kind\n");
                    break;
            }
        } else printf("Unknown node kind\n");
        for (i = 0; i < MAXCHILDREN; i++)
            printTree(tree->child[i]);
        tree = tree->sibling;
    }
}