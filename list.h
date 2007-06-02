#ifndef LIST_H
#define LIST_H

int insertAfterToken(tokenItem *tokenList, cToken token);
int addToken(tokenItem *tokenList, cToken token, double value, int length);
int addNumber(tokenItem *tokenList, const char *buffer, int bufferPos);
int addSimpleToken(tokenItem *tokenList, cToken token);
void freeList(tokenItem *tokenList);

#endif
