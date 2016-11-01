//
// Created by liuyu on 16-10-15.
//

#ifndef EXPLLEXICALANALYZER_H
#define EXPLLEXICALANALYZER_H

#include "GLOBALS.h"

TokenType getToken(void);
void set_up();
extern char tokenString[MAXTOKENLEN + 1];

#endif //LEARN_2_EXPLLEXICALANALYZER_H
