/*
 * Copyright (c) 2007, Roger Light
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO 
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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