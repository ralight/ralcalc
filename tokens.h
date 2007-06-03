#ifndef TOKENS_H
#define TOKENS_H

#include "datatypes.h"

int validate(tokenItem *tokenList, const char *line);
int tokenise(tokenItem *tokenList, const char *line, double lastResult);
errType insertAfterToken(tokenItem *tokenList, cToken token);
errType addToken(tokenItem *tokenList, cToken token, double value, int length);
errType addNumber(tokenItem *tokenList, const char *buffer, int bufferPos);
errType addSimpleToken(tokenItem *tokenList, cToken token);
errType deletePreviousToken(tokenItem *item);
void freeList(tokenItem *tokenList);

#endif
