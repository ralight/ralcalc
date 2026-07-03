/*
 * File: calculation.c Author: Roger Light
 * Project: ralcalc
 * Desc: Perform a calculation based on a list of tokens.
 *
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
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <errno.h>
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
double doCalculation(double valueOne, double valueTwo, cToken operator, errType *err)
{
#ifdef DEBUG
	fprintf(stderr, "doCalculation(%g, %g, %s)\n", valueOne, valueTwo, ctoken_to_string(operator));
#endif
	double result = 0.0;

	errno = 0;
	*err = errNoError;
	switch(operator){
		case tkPlus: result = valueOne + valueTwo; break;
		case tkMinus: result = valueOne - valueTwo; break;
		case tkNegation: result = valueOne * -1.0; break;

		case tkMultiply:
		case tkMultiplyX: result = valueOne * valueTwo; break;

		case tkDivide:
			if(valueTwo == 0.0){
				*err = errDivisionByZero;
				return 0.0;
			}
			result = valueOne / valueTwo;
			break;
		case tkPower: result = pow(valueOne, valueTwo); break;
		case tkMod:
			if(valueTwo == 0.0){
				*err = errDivisionByZero;
				return 0.0;
			}
			result = fmod(valueOne, valueTwo);
			break;
		case tkLn: result = log(valueTwo); break;
		case tkLog: result = log10(valueTwo); break;
		case tkSin: result = sin(valueTwo); break;
		case tkCos: result = cos(valueTwo); break;
		case tkTan: result = tan(valueTwo); break;
		case tkASin: result = asin(valueTwo); break;
		case tkACos: result = acos(valueTwo); break;
		case tkATan: result = atan(valueTwo); break;
		case tkSqrt: result = sqrt(valueTwo); break;

		case tkNumber:
		case tkLastResult:
		case tkOpenBracket:
		case tkCloseBracket:
		case tkCOpenBracket:
		case tkCCloseBracket:
		case tkEndToken:
			result = valueTwo;
			/* FIXME - error condition */
			break;
	}

	if(errno == EDOM || errno == ERANGE || isnan(result)){
		*err = errDomain;
	}
	return result;
}


/*
 * process()
 *
 * Recursive processing with nested brackets taking precedent and
 * ^, * /, + - precedence for the other operators.
 *
 * When it reaches an end bracket then all of the previous tokens are deleted
 * up to the open bracket. As process() only gets called after it has been
 * validated that the brackets match up, and it calls itself whenever it meets
 * a new open bracket, this means that only tokens within the current pair of
 * brackets get deleted.
 *
 * As the recursion ensures that the most nested pair of brackets for a given
 * set is processed first, this function must only also ensure that the tokens
 * within a set of brackets are processed with the correct precedence. Seems
 * fine so far.
 */
errType process(tokenItem **tokenList, double *result)
{
	double value = 0.0;
	tokenItem *item;
	int precedence;
	double valueOne;
	double valueTwo;
	int firstValue;
	cToken operator = tkEndToken;
	int tokenPrecedence = 0;
	errType err = errNoError;

	if(!tokenList || !(*tokenList)){
		return errBadInput;
	}

	for(precedence = 5; precedence >= 0; precedence--){

#ifdef DEBUG
		item = (*tokenList);
		fprintf(stderr, "Tokens:");
		while(item){
			if(item->type == tkNumber){
				fprintf(stderr, " (%g)", item->value);
			}else{
				fprintf(stderr, " %s", ctoken_to_string(item->type));
			}
			item = item->next;
		}
		fprintf(stderr, "\n");
#endif

		firstValue = 1;
		valueOne = 0.0;
		valueTwo = 0.0;
		item = (*tokenList);
		while(item){
			switch(item->type){
				case tkOpenBracket:
				case tkCOpenBracket:
					if(item->next){
						double retval;
						err = process(&(item->next), &retval);
						if(err != errNoError){
							return err;
						}
						insertNumberAfterToken(item, retval);
						deletePreviousToken(item->next); // delete open bracket
						item = (*tokenList); /* Reset to the beginning */
						precedence = 5; /* Always start with new precedence after brackets have been found */
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
						*result = value;
						return err;
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
						valueTwo = item->value;

						if((operator == tkLog || operator == tkLn || operator == tkSin \
								|| operator == tkCos || operator == tkTan || operator == tkSqrt \
								|| operator == tkASin || operator == tkACos || operator == tkATan) \
								&& tokenPrecedence == precedence){
							double retval = doCalculation(valueOne, valueTwo, operator, &err);
							insertNumberAfterToken(item, retval);
							deletePreviousToken(item); /* Delete operator */
							deletePreviousToken(item->next); /* Delete the old number */
							item = (*tokenList); /* Reset to the beginning */
							precedence++;
							value = 0.0;
						}else{
							if(operator != tkEndToken && tokenPrecedence == precedence){
								double retval = doCalculation(valueOne, valueTwo, operator, &err);
								insertNumberAfterToken(item, retval);
								deletePreviousToken(item); /* Delete operator */
								deletePreviousToken(item); /* Delete number */
								deletePreviousToken(item->next); /* deletes the old number */
								item = (*tokenList); /* Reset to the beginning */
								precedence++;
								value = 0.0;
							}
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
				case tkMod:
					operator = item->type;
					tokenPrecedence = item->precedence;
					break;

				case tkLog:
				case tkLn:
				case tkSin:
				case tkCos:
				case tkTan:
				case tkASin:
				case tkACos:
				case tkATan:
				case tkSqrt:
					valueOne = 1.0;
					firstValue = 0;
					operator = item->type;
					tokenPrecedence = item->precedence;
					break;

				case tkNegation:
					if(precedence == item->precedence){
						item = item->next;
						if(item && (item->type == tkNumber || item->type == tkLastResult)){
							item->value *= -1.0;
							deletePreviousToken(item); /* Delete negation */
							item = (*tokenList);
							precedence++;
							value = 0.0;
							operator = tkEndToken;
						}else{
							/* FIXME - could do with an error code here */
						}
					}
					break;

				case tkEndToken:
					/* FIXME - error condition */
					break;
			}
			if(item){
				item = item->next;
			}
		}
	}

	*result = value;
	return err;
}

