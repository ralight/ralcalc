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


static bool isRightAssociative(cToken token)
{
	return token == tkNegation || token == tkPower;
}

static bool isNumber(cToken token)
{
	return token == tkNumber || token == tkLastResult;
}


static bool isBinary(cToken token)
{
	return token == tkMod
		|| token == tkMultiply
		|| token == tkPlus
		|| token == tkMinus
		|| token == tkDivide
		|| token == tkPower
		|| token == tkMultiplyX;
}

static bool isFunction(cToken token)
{
	return token == tkLog
		|| token == tkLn
		|| token == tkSin
		|| token == tkCos
		|| token == tkTan
		|| token == tkASin
		|| token == tkACos
		|| token == tkATan
		|| token == tkSqrt;
}

static bool isOperator(cToken token)
{
	return token == tkMod
		|| token == tkMultiply
		|| token == tkPlus
		|| token == tkMinus
		|| token == tkDivide
		|| token == tkPower
		|| token == tkMultiplyX
		|| token == tkNegation;
}

static bool isOpenBracket(cToken token)
{
	return token == tkOpenBracket || token == tkCOpenBracket;
}

static bool isCloseBracket(cToken token)
{
	return token == tkCloseBracket || token == tkCCloseBracket;
}


static double popOperator(cToken *stack, int *idx)
{
	cToken token = tkEndToken;
	if(*idx > 0){
		token = stack[(*idx) - 1];
		(*idx)--;
	}
	//printf("pop op %s\n", ctoken_to_string(token));
	return token;
}


static void pushOperator(cToken *stack, int *idx, cToken token)
{
	stack[*idx] = token;
	(*idx)++;
	//printf("push op %s\n", ctoken_to_string(token));
}

static double popValue(double *stack, int *idx)
{
	double v = 0.0;
	if(*idx > 0){
		v = stack[(*idx) - 1];
		(*idx)--;
	}
	//printf("pop v %g\n", v);
	return v;
}


static void pushValue(double *stack, int *idx, double value)
{
	stack[*idx] = value;
	(*idx)++;
	//printf("push v %g\n", value);
}

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
		case tkNegation: result = valueTwo * -1.0; break;

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


static errType reduce(double *valueStack, int *valueIndex, cToken operator)
{
	errType err;
	double rhs = popValue(valueStack, valueIndex);
	double lhs = 0.0;
	if(isBinary(operator)){
		lhs = popValue(valueStack, valueIndex);
	}
	double result = doCalculation(lhs, rhs, operator, &err);
	pushValue(valueStack, valueIndex, result);
	return err;
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
	tokenItem *item;
	errType err = errNoError;

	cToken *operatorStack = NULL;
	double *valueStack = NULL;
	int operatorIndex = 0;
	int valueIndex = 0;

	if(!tokenList || !(*tokenList)){
		return errBadInput;
	}

	int tokenCount = 0;
	for(item = *tokenList; item; item = item->next){
		tokenCount++;
	}

	operatorStack = calloc(tokenCount, sizeof(cToken));
	valueStack = calloc(tokenCount, sizeof(double));
	if(!operatorStack || !valueStack){
		free(operatorStack);
		free(valueStack);
		return errMemory;
	}

	for(item = *tokenList; item; item = item->next){
		if(isNumber(item->type)){
			pushValue(valueStack, &valueIndex, item->value);
		}else if(isFunction(item->type)){
			pushOperator(operatorStack, &operatorIndex, item->type);
		}else if(isOperator(item->type) || isCloseBracket(item->type)){
			cToken optop = tkEndToken;
			while(operatorIndex > 0 && (optop = operatorStack[operatorIndex-1])
					&& !isOpenBracket(optop)
					&& (getPrecedence(optop) > getPrecedence(item->type)
					|| (getPrecedence(optop) == getPrecedence(item->type)
							&& !isRightAssociative(optop)))){

				err = reduce(valueStack, &valueIndex, optop);
				if(err != errNoError){
					return err;
				}
				operatorIndex--;
			}
			if(isCloseBracket(item->type)){
				if(isOpenBracket(optop)){
					popOperator(operatorStack, &operatorIndex);
				}else{
					return errBadInput;
				}
			}else{
				pushOperator(operatorStack, &operatorIndex, item->type);
			}
		}else if(isOpenBracket(item->type)){
			pushOperator(operatorStack, &operatorIndex, item->type);
		}
	}

	while(operatorIndex > 0){
		cToken optop = operatorStack[operatorIndex-1];
		operatorIndex--;
		if(valueIndex > 0){
			err = reduce(valueStack, &valueIndex, optop);
			if(err != errNoError){
				return err;
			}
		}else{
			return errBadInput;
		}
	}

	*result = popValue(valueStack, &valueIndex);
	free(operatorStack);
	free(valueStack);

	return err;
}

