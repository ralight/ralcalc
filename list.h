#ifndef LIST_H
#define LIST_H

errType insertAfterToken(tokenItem *tokenList, cToken token);
errType addToken(tokenItem *tokenList, cToken token, double value, int length);
errType addNumber(tokenItem *tokenList, const char *buffer, int bufferPos);
errType addSimpleToken(tokenItem *tokenList, cToken token);
errType deletePreviousToken(tokenItem *tokenList, tokenItem *item);
void freeList(tokenItem *tokenList);

#endif
