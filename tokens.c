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
#include <string.h>

#include "output.h"
#include "tokens.h"


/*
 * addNumber()
 *
 * Parse a number and add it to the end of the token list.
 */
errType addNumber(tokenItem *tokenList, const char *buffer, int bufferPos)
{
	int i;
	char str[bufferPos+2];
	int haveMultiplier = 0;
	int haveDecimalPoint = 0;
	int multiplierPos = -1;
	double multiplier = 1.0;
	double number = 0.0;

	if(!tokenList || !buffer || bufferPos < 0) return errBadInput;

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

	if(haveMultiplier > 1 || haveDecimalPoint > 1) return errBadNumber;
	if(haveMultiplier && multiplierPos != bufferPos - 1) return errBadNumber;

	/* FIXME - should this be added to the list regardless of error? */
	number = strtod(str, NULL);
	return addToken(tokenList, tkNumber, number*multiplier, bufferPos);
}


/*
 * addSimpleToken()
 *
 * Add a simple token (no value) to the end of the list.
 * This is a convenience function to avoid confusion with the
 * extra two arguments to addToken() that are unused in 
 * simple tokens.
 */
errType addSimpleToken(tokenItem *tokenList, cToken token)
{
	return addToken(tokenList, token, 0.0, 1);
}


/*
 * addToken()
 *
 * Add a token to the end of the list.
 *
 * This assumes that the list already exists as is the case
 * when called by main().
 *
 * Possible return values:
 *
 * errBadInput (tokenList is NULL)
 * errMemory (unable to allocate new item)
 * errNoError
 */
errType addToken(tokenItem *tokenList, cToken token, double value, int length)
{
	tokenItem *newItem = NULL;
	tokenItem *list;

	if(!tokenList) return errBadInput;

	/* Find tail */
	list = tokenList;
	while(list->next) list = list->next;

	newItem = calloc(1, sizeof(tokenItem));
	if(!newItem) return errMemory;
	newItem->next = NULL;

	switch(token){
		case tkPlus:
		case tkMinus:
		case tkMultiply:
		case tkMultiplyX:
		case tkDivide:
		case tkPower:
		case tkOpenBracket:
		case tkCloseBracket:
		case tkCOpenBracket:
		case tkCCloseBracket:
			newItem->type = token;
			newItem->value = 0.0;
			newItem->length = length;
			list->next = newItem;
			newItem->prev = list;
			break;
		case tkNumber:
		case tkLastResult:
			newItem->type = token;
			newItem->value = value;
			newItem->length = length;
			list->next = newItem;
			newItem->prev = list;
			break;
		default:
			printf("Unknown token (%d)\n", token);
			free(newItem);
			return errUnknownToken;
			break;
	}

	return errNoError;
}


/*
 * assignPrecedence()
 *
 * Go through all tokens and assign precedence to the
 * operators in the token list. 
 *
 * It's much cleaner to do this here than in tokenise()
 * because of the other error checking done there.
 *
 * Possible return values:
 *
 * errBadInput (tokenList is NULL)
 * errNoError
 */
int assignPrecedence(tokenItem *tokenList)
{
	tokenItem *item;
	int precedence;

	if(!tokenList) return 1;

	item = tokenList;
	while(item){
		precedence = 0;
		switch(item->type){
			case tkPlus:
			case tkMinus:
				precedence = 1;
				break;
			case tkMultiply:
			case tkMultiplyX:
			case tkDivide:
				precedence = 2;
				break;
			case tkPower:
				precedence = 3;
				break;

			/* 
			 * These tokens should never have to worry about
			 * precedence.
			 */
			case tkOpenBracket:
			case tkCloseBracket:
			case tkCOpenBracket:
			case tkCCloseBracket:
			case tkNumber:
			case tkLastResult:
			case tkExponent:
			case tkEndToken:
				precedence = 0;
				break;
		}

		item->precedence = precedence;

		item = item->next;
	}

	return 0;
}


/*
 * deletePreviousToken()
 *
 * Delete the previous token in the list.
 * This is used to delete the negation token when a number has been 
 * negated and also to remove a bracket pair and contents when its
 * result has been calculated.
 */
errType deletePreviousToken(tokenItem *item)
{
	tokenItem *deleteItem;

	if(!item || !item->prev) return errBadInput;
	if(item->prev->type == tkEndToken) return errBadInput;

	deleteItem = item->prev;

#ifdef DEBUG
	printf("Deleting '%c' (%g)\n", deleteItem->type, deleteItem->value);
#endif
	
	item->prev = deleteItem->prev;
	if(item->prev){
		item->prev->next = item;
	}

	free(deleteItem);

	return errNoError;
}


/*
 * freeList()
 *
 * Clean up the memory associated with the token list.
 */
void freeList(tokenItem *tokenList)
{
	tokenItem *nextItem;

	while(tokenList){
		nextItem = tokenList->next;
		free(tokenList);
		tokenList = nextItem;
	}
}


/*
 * insertAfterToken()
 *
 * Insert a simple token in the middle of the list after the current item.
 * This is used to turn (1+2)(3+4) into (1+2)*(3+4) for easier processing.
 * An alternative would be to presume the user had made a mistake and 
 * exit with an error.
 */
errType insertAfterToken(tokenItem *item, cToken token)
{
	tokenItem *newItem;

	if(!item) return errBadInput;

	newItem = calloc(1, sizeof(tokenItem));
	if(!newItem) return errMemory;

	newItem->type = token;
	newItem->next = item->next;
	item->next = newItem;
	if(newItem->next){
		newItem->next->prev = newItem;
	}
	newItem->prev = item;
	newItem->length = 1;

	return errNoError;
}


/*
 * insertBeforeToken()
 *
 * Insert a complex token in the middle of the list before the current item.
 * This is used to turn -(1+2) into -1.0*(1+2).
 */
errType insertBeforeToken(tokenItem *item, cToken token, double value, int length)
{
	tokenItem *newItem;

	if(!item) return errBadInput;

	newItem = calloc(1, sizeof(tokenItem));
	if(!newItem) return errMemory;

	newItem->type = token;
	newItem->value = value;
	newItem->length = length;

	newItem->next = item;
	newItem->prev = item->prev;
	if(newItem->prev){
		newItem->prev->next = newItem;
	}
	item->prev = newItem;

	return errNoError;
}


/*
 * insertNumberAfterToken()
 *
 * Insert a number token in the middle of the list after the current item.
 */
errType insertNumberAfterToken(tokenItem *item, double value)
{
	tokenItem *newItem;

	if(!item) return errBadInput;

#ifdef DEBUG
	printf("Inserting %g\n", value);
#endif

	newItem = calloc(1, sizeof(tokenItem));
	if(!newItem) return errMemory;

	newItem->type = tkNumber;
	newItem->value = value;
	newItem->next = item->next;
	item->next = newItem;
	if(newItem->next){
		newItem->next->prev = newItem;
	}
	newItem->prev = item;
	newItem->length = 1;

	return errNoError;
}


/*
 * tokenise()
 *
 * Process the line input and convert it into a list of tokens
 */
int tokenise(tokenItem *tokenList, const char *line, double lastResult)
{
	int i;
	cToken lastToken = tkEndToken;
	char buffer[100];
	int bufferPos = 0;
	int inNumber = 0;
	int rc = 0;
	errType err;

	if(!tokenList || !line) return -1;

	memset(buffer, 0, 100);

	for(i = 0; i < strlen(line); i++){
		switch(line[i]){
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '.':
			case 'Y':
			case 'Z':
			case 'E':
			case 'P':
			case 'T':
			case 'G':
			case 'M':
			case 'k':
			case 'm':
			case 'u':
			case 'n':
			case 'p':
			case 'f':
			case 'a':
			case 'z':
			case 'y':
				if(inNumber){
					buffer[bufferPos] = line[i];
					bufferPos++;
				}else{
					memset(buffer, 0, 100);
					buffer[0] = line[i];
					bufferPos = 1;
					inNumber = 1;
				}
				break;

			case '+':
			case '-':
			case 'x':
			case '*':
			case '/':
			case '[':
			case ']':
			case '(':
			case ')':
			case '^':
				if(inNumber){
					err = addNumber(tokenList, buffer, bufferPos);
					if(err != errNoError){
						rc = 1;
						printError(line, i-1, err);
					}
					inNumber = 0;
				}
				err = addSimpleToken(tokenList, line[i]);
				if(err != errNoError){
					rc = 1;
					printError(line, i, err);
				}
				lastToken = line[i];
				break;

			case '_':
				err = addToken(tokenList, tkLastResult, lastResult, 1);
				if(err != errNoError){
					rc = 1;
					printError(line, i, err);
				}
				lastToken = '_';
				break;

			default:
				rc = 1;
				printError(line, i, errUnknownToken);
				break;
		}
	}
	if(inNumber){
		err = addNumber(tokenList, buffer, bufferPos);
		if(err != errNoError){
			rc = 1;
			printError(line, i-2, err);
		}
		inNumber = 0;
	}

	return rc;
}


/* 
 * validate()
 *
 * Work through all tokens making sure that all brackets match
 * and that no illegal combinations of tokens exist: 9++2 for 
 * example.
 *
 * Also negates numbers where necessary: 9+-2.
 * This should probably be dealt with somewhere else.
 */
int validate(tokenItem *tokenList, const char *line)
{
	cToken lastToken;
	tokenItem *item;
	int openBrackets = 0;
	int closeBrackets = 0;
	int currentPos = 0;
	int negateValue = 0;
	errType err;
	int rc = 0;

	if(!tokenList || !tokenList->next) return -1;

	item = tokenList->next;
	lastToken = tkEndToken;

	while(item){
		currentPos += item->length;
		negateValue--; /* this allows us to detect when we have e.g. 4+-2 -> so we're looking backwards by two
						* operators at once rather than just one. */

		switch(item->type){
			case tkNumber:
			case tkLastResult:
				if(lastToken == tkCloseBracket || lastToken == tkCCloseBracket){
					err = insertAfterToken(item, tkMultiply);
					if(err != errNoError){
						printError(line, currentPos, err);
						rc = 1;
					}
				}else if(lastToken == tkNumber || lastToken == tkLastResult){
					printError(line, currentPos, errDuplicateNumber);
					rc = 1;
				}else if(lastToken == tkMinus){
					if(negateValue == 1){
						// Negate the value
						item->value *= -1.0;
						// Delete the "-" from the token list
						deletePreviousToken(item);
					}
				}
				break;
				
			case tkPlus:
			case tkMultiply:
			case tkMultiplyX:
			case tkDivide:
			case tkPower:
				if(lastToken != tkNumber && lastToken != tkLastResult && lastToken != tkCloseBracket && lastToken != tkCCloseBracket && lastToken != tkEndToken){
					printError(line, currentPos-1, errInvalidOperator);
					rc = 1;
				}
				break;

			/* FIXME - negation shouldn't take precedent over other things */
			case tkMinus:
				/* prevent an inappropriate number of "-" signs */
				if(lastToken == tkMinus && negateValue == 1){
					printError(line, currentPos-1, errInvalidOperator);
					rc = 1;
				}else if(lastToken != tkNumber && lastToken != tkLastResult && lastToken != tkCloseBracket && lastToken != tkCCloseBracket){
					negateValue = 2;
				}
				break;

			case tkOpenBracket:
			case tkCOpenBracket:
				if(lastToken == tkCloseBracket || lastToken == tkCCloseBracket){
					err = insertAfterToken(item, tkMultiply);
					if(err != errNoError){
						printError(line, currentPos, err);
						rc = 1;
					}
				}else if(lastToken == tkMinus){
					if(negateValue == 1){
						deletePreviousToken(item);
						err = insertBeforeToken(item, tkNumber, -1.0, 1);
						if(err != errNoError){
							printError(line, currentPos, err);
							rc = 1;
						}
						err = insertBeforeToken(item, tkMultiply, 0.0, 1);
						if(err != errNoError){
							printError(line, currentPos, err);
							rc = 1;
						}
					}
				}
				openBrackets++;
				break;

			case tkCloseBracket:
			case tkCCloseBracket:
				if(lastToken != tkNumber && lastToken != tkLastResult && lastToken != tkCloseBracket && lastToken != tkCCloseBracket){
					printError(line, currentPos-1, errInvalidBracket);
					rc = 1;
				}
				closeBrackets++;
				break;

			default:
				printError(line, currentPos, errUnknownToken);
				rc = 1;
				break;
		}
		lastToken = item->type;
		item = item->next;
	}

	if(openBrackets != closeBrackets){
		printError(line, currentPos, errMismatchedBrackets);
		rc = 1;
	}
	if(lastToken == tkPlus || lastToken == tkMinus \
			|| lastToken == tkMultiply || lastToken == tkMultiplyX || lastToken == tkDivide \
			|| lastToken == tkPower || lastToken == tkOpenBracket || lastToken == tkCOpenBracket){
		printError(line, currentPos-1, errInvalidOperator);
		rc = 1;
	}
	
	return rc;
}

