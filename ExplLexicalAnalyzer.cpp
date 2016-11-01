//
// Created by liuyu on 16-10-15.
//

#include "util.h"
#include "ExplLexicalAnalyzer.h"
#include "GLOBALS.h"

using namespace std;

typedef enum {
    START, INASSIGN, INCOMMENT, INNUM, INID, DONE
} StateType;

char tokenString[MAXTOKENLEN + 1];

#define BUFLEN 256

static int linepos = 0;
static char lineBuf[BUFLEN];
static int bufsize = 0;
static int EOF_flag = false;
static string filename;
static fstream get;
static int columnpos = 0;
bool TraceScan = false;
StateType state;

static struct {
    const char *str;
    TokenType tok;
} reservedWords[MAXRESERVED]
        = {{"if",       IF},
           {"then",     THEN},
           {"else",     ELSE},
           {"end",      END},
           {"repeat",   REPEAT},
           {"until",    UNTIL},
           {"read",     READ},
           {"write",    WRITE},
           {"begin",    BEGIN},
           {"var",      VAR},
           {"interger", INTEGER},
           {"double",   DOUBLE},
           {"string",   STRING}};


static char
getNextChar() {
    if (linepos >= bufsize) {
        lineno = 0;
        if (state != START)
            return ' ';
        if (get.getline(lineBuf, BUFLEN - 1)) {
            printf("%d: %s\n", columnpos++, lineBuf);
            bufsize = (int) strlen(lineBuf);
            linepos = 0;
            return lineBuf[linepos++];
        } else {
            return EOF;
        }
    } else return lineBuf[linepos++];
}


static TokenType reservedLookup(char *s) {
    int i;
    for (i = 0; i < MAXRESERVED; i++)
        if (!strcmp(s, reservedWords[i].str))
            return reservedWords[i].tok;
    return ID;
}

//退回一个字符
static void ungetNextChar(void) { if (!EOF_flag) linepos--; }

//打印分析结果


//词法分析
TokenType getToken(void) {
    int tokenStringIndex = 0;
    TokenType currentToken;
    state = START;
    bool save;
    while (state != DONE) {
        char c = getNextChar();
        save = true;
        switch (state) {
            case START:
                if (isdigit(c))
                    state = INNUM;
                else if (isalpha(c))
                    state = INID;
                else if (c == ':')
                    state = INASSIGN;
                else if ((c == ' ') || (c == '\t') || (c == '\n'))
                    save = false;
                else if (c == '{') {
                    save = false;
                    state = INCOMMENT;
                } else {
                    state = DONE;
                    switch (c) {
                        case EOF:
                            return ENDFILE;
                        case ',':
                            currentToken = COMMA;
                            break;
                        case '=':
                            currentToken = EQ;
                            break;
                        case '<':
                            currentToken = LT;
                            break;
                        case '+':
                            currentToken = PLUS;
                            break;
                        case '-':
                            currentToken = MINUS;
                            break;
                        case '*':
                            currentToken = TIMES;
                            break;
                        case '/':
                            currentToken = OVER;
                            break;
                        case '(':
                            currentToken = LPAREN;
                            break;
                        case ')':
                            currentToken = RPAREN;
                            break;
                        case ';':
                            currentToken = SEMI;
                            break;
                        default:
                            currentToken = ERROR;
                            break;
                    }
                }
                break;
            case INCOMMENT:
                save = false;
                if (c == EOF) {
                    state = DONE;
                    currentToken = ENDFILE;
                } else if (c == '}') state = START;
                break;
            case INASSIGN:
                state = DONE;
                if (c == '=')
                    currentToken = ASSIGN;
                else {
                    currentToken = DEFINE;
                    ungetNextChar();
                }
                break;
            case INNUM:
                if (!isdigit(c)) {
                    ungetNextChar();
                    save = false;
                    state = DONE;
                    currentToken = NUM;
                }
                break;
            case INID:
                if (!isalpha(c)) {
                    tokenString[tokenStringIndex] = '\0';
                    if (!strcmp(tokenString, "begin") || !strcmp(tokenString, "end")) {
                        save = false;
                        state = DONE;
                        currentToken = ID;
                        break;
                    }
                    ungetNextChar();
                    save = false;
                    state = DONE;
                    currentToken = ID;
                }
                break;
            case DONE:
                break;
        }
        if ((save) && (tokenStringIndex <= MAXTOKENLEN) && (state != START && !isspace(c)))
            tokenString[tokenStringIndex++] = c;
        if (state == DONE) {
            tokenString[tokenStringIndex] = '\0';
            if (currentToken == ID)
                currentToken = reservedLookup(tokenString);
        }
    }
    if (TraceScan) {
        printf("\t%d: ", lineno++);
        printToken(currentToken, tokenString);
    }
    return currentToken;
}

void set_up() {
    if (cin >> filename && filename == "q") {
        filename = "/home/liuyu/文档/test";
    }
    get.open(filename, ios::in);
}


