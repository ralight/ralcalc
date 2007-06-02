#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ccalc.h"

int insertAfterToken(tokenItem *tokenList, cToken token)
{
	tokenItem *newItem;

	if(!tokenList) return 1;

	newItem = calloc(1, sizeof(tokenItem));
	if(!newItem) return 1;

	newItem->type = token;
	newItem->next = tokenList->next;
	newItem->length = 1;
	tokenList->next = newItem;

	return 0;
}


int addToken(tokenItem *tokenList, cToken token, double value, int length)
{
	tokenItem *newItem;
	tokenItem *list;

	/* Find tail */
	list = tokenList;
	while(list->next) list = list->next;

	newItem = calloc(1, sizeof(tokenItem));

	switch(token){
		case tkPlus:
		case tkMinus:
		case tkMultiply:
		case tkDivide:
		case tkPower:
		case tkOpenBracket:
		case tkCloseBracket:
			newItem->type = token;
			newItem->value = 0.0;
			newItem->length = length;
			newItem->next = NULL;
			list->next = newItem;
			break;
		case tkNumber:
			newItem->type = token;
			newItem->value = value;
			newItem->length = length;
			newItem->next = NULL;
			list->next = newItem;
			break;
		default:
			printf("Unknown token (%d)\n", token);
			free(newItem);
			return 1;
			break;
	}

	return 0;
}

int addNumber(tokenItem *tokenList, const char *buffer, int bufferPos)
{
	int i;
	char str[bufferPos+2];
	int haveMultiplier = 0;
	int haveDecimalPoint = 0;
	int multiplierPos = -1;
	double multiplier = 1.0;
	double number = 0.0;

	strncpy(str, buffer, bufferPos+2);

	for(i = bufferPos-1; i >= 0; i--){
		switch(str[i]){
			case 'Y':
				multiplier = 1000000000000000000000000.0;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'Z':
				multiplier = 1000000000000000000000.0;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'E':
				multiplier = 1000000000000000000.0;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'P':
				multiplier = 1000000000000000.0;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'T':
				multiplier = 1000000000000.0;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'G':
				multiplier = 1000000000.0;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'M':
				multiplier = 1000000.0;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'k':
				multiplier = 1000.0;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'm':
				multiplier = 0.001;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'u':
				multiplier = 0.000001;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'n':
				multiplier = 0.000000001;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'p':
				multiplier = 0.000000000001;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'f':
				multiplier = 0.000000000000001;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'a':
				multiplier = 0.000000000000000001;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'z':
				multiplier = 0.000000000000000000001;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'y':
				multiplier = 0.000000000000000000000001;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case '.':
				haveDecimalPoint++;
		}
	}

	if(haveMultiplier > 1 || haveDecimalPoint > 1) return 1;
	if(haveMultiplier && multiplierPos != bufferPos - 1) return 1;

	number = strtod(str, NULL);
	if(addToken(tokenList, tkNumber, number, bufferPos)) return 1;

	return 0;
}

int addSimpleToken(tokenItem *tokenList, cToken token)
{
	return addToken(tokenList, token, 0.0, 1);
}

