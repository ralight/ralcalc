#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "datatypes.h"
#include "calculation.h"
#include "list.h"


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

