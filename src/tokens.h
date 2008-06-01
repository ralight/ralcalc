/*
 * Copyright (C) 2007,2008 Roger Light.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */

#ifndef TOKENS_H
#define TOKENS_H

#include "datatypes.h"

errType addNumber(tokenItem *tokenList, const char *buffer, int bufferPos);
errType addSimpleToken(tokenItem *tokenList, cToken token);
errType addToken(tokenItem *tokenList, cToken token, double value, int length);
int assignPrecedence(tokenItem *tokenList);
errType deletePreviousToken(tokenItem *item);
void freeList(tokenItem *tokenList);
errType insertAfterToken(tokenItem *tokenList, cToken token);
errType insertBeforeToken(tokenItem *item, cToken token, double value, int length);
errType insertNumberAfterToken(tokenItem *item, double value);
int tokenise(tokenItem *tokenList, const char *line, double lastResult, int quiet);
int validate(tokenItem *tokenList, const char *line, int quiet);

#endif
