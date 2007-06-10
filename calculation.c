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


/*
 * doCalculation()
 *
 * Do actual maths based on an operator.
 *
 * Arguments:
 * 		valueOne - double value on left hand side of operator.
 * 		valueTwo - double value on right hand side of operator.
 * 		operator - operation to carry out.
 *
 * Returns:
 * 		Double result of the calculation or 0.0 on error.
 */
double doCalculation(double valueOne, double valueTwo, cToken operator)
{
	switch(operator){
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
		case tkCOpenBracket:
		case tkCCloseBracket:
		case tkEndToken:
			return valueTwo;
			/* FIXME - error condition */
			break;
	}

	return 0.0;
}

/* 
 * process()
 *
 * Recursive processing with nested brackets taking precedent and 
 * ^, * /, + - precedence for the other operators.
 *
 * When it reaches an end bracket then all of the previous tokens
 * are deleted up to the open bracket. As process() only gets called
 * after it has been validated that the brackets match up, and it
 * calls itself whenever it meets a new open bracket, this means that
 * only tokens within the current pair of brackets get deleted.
 *
 * As the recursion ensures that the most nested pair of brackets for
 * a given set is processed first, this function must only also ensure
 * that the tokens within a set of brackets are processed with the
 * correct precedence. Seems fine so far.
 */
double process(tokenItem **tokenList)
{
	double value = 0.0;
	double retval;
	tokenItem *item;
	int precedence;
	double valueOne;
	double valueTwo;
	int firstValue;
	cToken operator = tkEndToken;
	int tokenPrecedence = 0;

	if(!tokenList || !(*tokenList)) return 0.0;

	for(precedence = 3; precedence >= 0; precedence--){
		firstValue = 1;
		valueOne = 0.0;
		valueTwo = 0.0;
		item = (*tokenList);
		while(item){
			switch(item->type){
				case tkOpenBracket:
				case tkCOpenBracket:
					if(item->next){
						retval = process(&(item->next));
						insertNumberAfterToken(item, retval);
						deletePreviousToken(item->next); // delete open bracket
						item = (*tokenList); /* Reset to the beginning */
						precedence = 3; /* Always start with new precedence after brackets have been found */
					}
					break;

				case tkCloseBracket:
				case tkCCloseBracket:
					if(!precedence){
						while(item->prev->type != tkOpenBracket && item->prev->type != tkCOpenBracket){
							deletePreviousToken(item); // delete contents of brackets
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
					}else{
						/* If we see a close bracket, the loop should decrement
						 * rather than carry on. Simulate end of list.
						 */
						item = NULL;
					}
					break;

				case tkNumber:
				case tkLastResult:
					if(firstValue){
						valueOne = item->value;
						firstValue = 0;
						value = valueOne;
					}else{
						//firstValue = 1;
						valueTwo = item->value;
						if(operator != tkEndToken && tokenPrecedence == precedence){
							retval = doCalculation(valueOne, valueTwo, operator);
							insertNumberAfterToken(item, retval);
							deletePreviousToken(item); /* Delete operator */
							deletePreviousToken(item); /* Delete number */
							deletePreviousToken(item->next); /* deletes the old number */
							item = (*tokenList); /* Reset to the beginning */
							precedence++;
							value = 0.0;
							retval = 0.0;
						}
						valueOne = valueTwo;
						operator = tkEndToken;
					}
					break;

				case tkPlus:
				case tkMinus:
				case tkMultiply:
				case tkMultiplyX:
				case tkDivide:
				case tkPower:
					operator = item->type;
					tokenPrecedence = item->precedence;
					break;

				case tkExponent:
					/* FIXME */
					break;
				case tkEndToken:
					/* FIXME - error condition */
					break;
			}
			if(item) item = item->next;
		}
	}

	return value;
}

