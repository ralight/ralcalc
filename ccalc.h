#ifndef CCALC_H
#define CCALC_H

#include "datatypes.h"

void printError(const char *line, int pos, errType error);
int validate(tokenItem *tokenList, const char *line);
int tokenise(tokenItem *tokenList, const char *line);

#endif
