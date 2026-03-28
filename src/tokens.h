/*
 * Copyright (C) 2007-2026 Roger Light.
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

int assignPrecedence(tokenItem *tokenList);
errType deletePreviousToken(tokenItem *item);
void freeList(tokenItem *tokenList);
errType insertNumberAfterToken(tokenItem *item, double value);
int tokenise(tokenItem *tokenList, const char *line, double lastResult, int quiet);
int validate(tokenItem *tokenList, const char *line, int quiet);
const char *ctoken_to_string(cToken token);

#endif
