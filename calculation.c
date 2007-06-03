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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "datatypes.h"
#include "calculation.h"
#include "tokens.h"


double doCalculation(double valueOne, double valueTwo, cToken lastToken)
{
	switch(lastToken){
		case tkPlus:
			return valueOne + valueTwo;
			break;
		case tkMinus:
			return valueOne - valueTwo;
			break;
		case tkMultiply:
		case tkMultiplyX:
			return valueOne * valueTwo;
			break;
		case tkDivide:
			return valueOne / valueTwo;
			break;
		case tkPower:
			return pow(valueOne, valueTwo);
			break;
		case tkExponent:
			/* FIXME */
			break;

		case tkNumber:
		case tkLastResult:
		case tkOpenBracket:
		case tkCloseBracket:
		case tkEndToken:
			return valueTwo;
			/* FIXME - error condition */
			break;
	}

	return 0.0;
}

/* 
 * Recursive processing with nested brackets taking 
 * precedent and left to right precedence within 
 * brackets.
 */
double process(tokenItem **tokenList, const char *line)
{
	double value = 0.0;
	double retval;
	cToken lastToken = tkEndToken;
	int firstNumber = 1;
	tokenItem *item;

	if(!tokenList || !(*tokenList)) return 0.0;

	item = (*tokenList);
	while(item){
		switch(item->type){
			case tkOpenBracket:
				retval = process(&(item->next), line);
				if(!firstNumber){
					value = doCalculation(value, retval, lastToken);
				}else{
					value = retval;
					firstNumber = 0;
				}
				break;

			case tkCloseBracket:
				while(item->prev->type != tkOpenBracket){
					deletePreviousToken(item);
				}
				if(item->next){
					item = item->next;
					deletePreviousToken(item);
				}else{
					if(item->prev){
						item->prev = NULL;
					}
					free(item);
					item = NULL;
				}
				(*tokenList) = item;
				return value;
				break;

			case tkNumber:
			case tkLastResult:
				if(!firstNumber){
					value = doCalculation(value, item->value, lastToken);
				}else{
					value = item->value;
					firstNumber = 0;
				}
				break;

			case tkPlus:
			case tkMinus:
			case tkMultiply:
			case tkMultiplyX:
			case tkDivide:
			case tkPower:
				lastToken = item->type;
				break;

			case tkExponent:
				/* FIXME */
				break;
			case tkEndToken:
				/* FIXME - error condition */
				break;
		}
		item = item->next;
	}

	return value;
}

