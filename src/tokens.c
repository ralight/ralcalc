/*
 * File: tokens.c Author: Roger Light
 * Project: ralcalc
 * Desc: Parse a set of text strings into a list of tokens ready for calculation.
 *
 * Copyright (C) 2007-2009 Roger Light.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libintl.h>
#include <math.h>

#include "config.h"
#include "output.h"
#include "tokens.h"

#define BUFSIZE 100

struct tokeniserState {
	char buffer[BUFSIZE];
	const char *line;
	tokenItem *tokenList;
	int spaces;
	int bufferPos;
	int rc;
	unsigned int i;
	cToken lastToken;
	bool inNumber;
};

static errType addToken(tokenItem *tokenList, cToken token, double value, int length);


/*
 * addNumber()
 *
 * Parse a number and add it to the end of the token list.
 */
static errType addNumber(tokenItem *tokenList, const char *buffer, int bufferPos, int spaces)
{
	int i;
	char str[bufferPos+2];
	int haveMultiplier = 0;
	int haveDecimalPoint = 0;
	int haveExponent = 0;
	int multiplierPos = -1;
	double multiplier = 1.0;
	double number = 0.0;

	if(!tokenList || !buffer || bufferPos < 0){
		return errBadInput;
	}

	strncpy(str, buffer, bufferPos+2);

	for(i = bufferPos-1; i >= 0; i--){
		switch(str[i]){
			case 'Q':
				multiplier = 1.0e30;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'R':
				multiplier = 1.0e27;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'Y':
				multiplier = 1.0e24;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'Z':
				multiplier = 1.0e21;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'E':
				multiplier = 1.0e18;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'P':
				multiplier = 1.0e15;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'T':
				multiplier = 1.0e12;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'G':
				multiplier = 1.0e9;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'M':
				multiplier = 1.0e6;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'k':
				multiplier = 1.0e3;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'm':
				multiplier = 1.0e-3;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'u':
				multiplier = 1.0e-6;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'n':
				multiplier = 1.0e-9;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'p':
				multiplier = 1.0e-12;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'f':
				multiplier = 1.0e-15;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'a':
				multiplier = 1.0e-18;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'z':
				multiplier = 1.0e-21;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'y':
				multiplier = 1.0e-24;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'r':
				multiplier = 1.0e-27;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'q':
				multiplier = 1.0e-30;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case '.':
			case ',':
				haveDecimalPoint++;
				break;
			case 'e':
				haveExponent++;
				break;
		}
	}

	if(haveMultiplier > 1 || haveDecimalPoint > 1 || haveExponent > 1){
		return errBadNumber;
	}
	if(haveMultiplier && multiplierPos != bufferPos - 1){
		return errBadNumber;
	}

	/* FIXME - should this be added to the list regardless of error? */
	number = strtod(str, NULL);
	return addToken(tokenList, tkNumber, number*multiplier, bufferPos + spaces);
}


/*
 * addSimpleToken()
 *
 * Add a simple token (no value) to the end of the list.
 * This is a convenience function to avoid confusion with the
 * extra two arguments to addToken() that are unused in
 * simple tokens.
 */
static errType addSimpleToken(tokenItem *tokenList, cToken token, int spaces)
{
	return addToken(tokenList, token, 0.0, 1 + spaces);
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
static errType addToken(tokenItem *tokenList, cToken token, double value, int length)
{
	tokenItem *newItem = NULL;
	tokenItem *list;

	if(!tokenList){
		return errBadInput;
	}

	/* Find tail */
	list = tokenList;
	while(list->next){
		list = list->next;
	}

	newItem = calloc(1, sizeof(tokenItem));
	if(!newItem){
		return errMemory;
	}
	newItem->next = NULL;

	switch(token){
		case tkPlus:
		case tkMinus:
		case tkNegation:
		case tkMultiply:
		case tkMultiplyX:
		case tkDivide:
		case tkPower:
		case tkMod:
		case tkOpenBracket:
		case tkCloseBracket:
		case tkCOpenBracket:
		case tkCCloseBracket:
		case tkLog:
		case tkLn:
		case tkSin:
		case tkCos:
		case tkTan:
		case tkASin:
		case tkACos:
		case tkATan:
		case tkSqrt:
			newItem->type = token;
			newItem->value = 0.0;
			newItem->length = length;
			list->next = newItem;
			newItem->prev = list;
			break;
		case tkNumber:
		case tkLastResult:
		case tkPi:
			newItem->type = token;
			newItem->value = value;
			newItem->length = length;
			list->next = newItem;
			newItem->prev = list;
			break;
		default:
			fprintf(stderr, _("Unknown token (%d)\n"), token);
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

	if(!tokenList){
		return errBadInput;
	}

	item = tokenList;
	while(item){
		precedence = 0;
		switch(item->type){
			case tkNegation:
				precedence = 5;
				break;
			case tkPlus:
			case tkMinus:
				precedence = 1;
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
				precedence = 3;
				break;
			case tkMultiply:
			case tkMultiplyX:
			case tkDivide:
			case tkMod:
				precedence = 3;
				break;
			case tkPower:
				precedence = 4;
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
			case tkPi:
			case tkExp:
			case tkEndToken:
				precedence = 0;
				break;
		}

		item->precedence = precedence;

		item = item->next;
	}

	return errNoError;
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

	if(!item || !item->prev){
		return errBadInput;
	}
	if(item->prev->type == tkEndToken){
		return errBadInput;
	}

	deleteItem = item->prev;

#ifdef DEBUG
	fprintf(stderr, "Deleting '%s' (%g)\n", ctoken_to_string(deleteItem->type), deleteItem->value);
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
static errType insertAfterToken(tokenItem *item, cToken token)
{
	tokenItem *newItem;

	if(!item){
		return errBadInput;
	}

	newItem = calloc(1, sizeof(tokenItem));
	if(!newItem){
		return errMemory;
	}

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
static errType insertBeforeToken(tokenItem *item, cToken token, double value, int length)
{
	tokenItem *newItem;

	if(!item){
		return errBadInput;
	}

	newItem = calloc(1, sizeof(tokenItem));
	if(!newItem){
		return errMemory;
	}

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

	if(!item){
		return errBadInput;
	}

#ifdef DEBUG
	fprintf(stderr, "Inserting %g\n", value);
#endif

	newItem = calloc(1, sizeof(tokenItem));
	if(!newItem){
		return errMemory;
	}

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


static void printErrorIfNeeded(struct tokeniserState *ts, errType err, int quiet)
{
	if(err != errNoError){
		ts->rc = err;
		printError(ts->line, ts->i-1, err, quiet);
	}
}


static void addPotentialNumber(struct tokeniserState *ts, int quiet)
{
	if(ts->inNumber){
		errType err = addNumber(ts->tokenList, ts->buffer, ts->bufferPos, ts->spaces);
		printErrorIfNeeded(ts, err, quiet);
		ts->inNumber = 0;
		ts->spaces = 0;
	}
}


/*
 * tokenise()
 *
 * Process the line input and convert it into a list of tokens
 */
int tokenise(tokenItem *tokenList, const char *line, double lastResult, int quiet)
{
	char lastchar = '\0';
	errType err;
	struct tokeniserState ts = {0};

	if(!tokenList || !line){
		return -1;
	}

	ts.line = line;
	ts.tokenList = tokenList;

	for(ts.i = 0; ts.i < strlen(line); ts.i++){
		switch(line[ts.i]){
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
			case ',':
				if(ts.inNumber){
					ts.buffer[ts.bufferPos] = line[ts.i];
					ts.bufferPos++;
				}else{
					memset(ts.buffer, 0, sizeof(ts.buffer));
					ts.buffer[0] = line[ts.i];
					ts.bufferPos = 1;
					ts.inNumber = 1;
				}
				break;

			case 'e':
			case 'Q':
			case 'R':
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
			case 'r':
			case 'q':
				if(ts.i < strlen(line)-1 && line[ts.i] == 'p' && line[ts.i+1] == 'i'){
					/* pi */
					addPotentialNumber(&ts, quiet);

					err = addToken(ts.tokenList, tkNumber, M_PI, 2 + ts.spaces);
					printErrorIfNeeded(&ts, err, quiet);
					ts.spaces = 0;
					ts.i++;
				}else if(ts.i < strlen(line)-2 && line[ts.i] == 'e' && line[ts.i+1] == 'x' && line[ts.i+2] == 'p'){
					/* exp */
					addPotentialNumber(&ts, quiet);

					err = addToken(ts.tokenList, tkNumber, M_E, 3 + ts.spaces);
					printErrorIfNeeded(&ts, err, quiet);
					ts.spaces = 0;
					ts.i+=2;
				}else if(ts.i < strlen(line)-3 && line[ts.i] == 'a' && line[ts.i+1] == 's' && line[ts.i+2] == 'i' && line[ts.i+3] == 'n'){
					/* asin */
					addPotentialNumber(&ts, quiet);

					err = addToken(ts.tokenList, tkASin, 0.0, 4 + ts.spaces);
					printErrorIfNeeded(&ts, err, quiet);
					ts.spaces= 0;
					ts.i+=3;
				}else if(ts.i < strlen(line)-3 && line[ts.i] == 'a' && line[ts.i+1] == 'c' && line[ts.i+2] == 'o' && line[ts.i+3] == 's'){
					/* acos */
					addPotentialNumber(&ts, quiet);

					err = addToken(ts.tokenList, tkACos, 0.0, 4 + ts.spaces);
					printErrorIfNeeded(&ts, err, quiet);
					ts.spaces = 0;
					ts.i+=3;
				}else if(ts.i < strlen(line)-3 && line[ts.i] == 'a' && line[ts.i+1] == 't' && line[ts.i+2] == 'a' && line[ts.i+3] == 'n'){
					/* atan */
					addPotentialNumber(&ts, quiet);

					err = addToken(ts.tokenList, tkATan, 0.0, 4 + ts.spaces);
					printErrorIfNeeded(&ts, err, quiet);
					ts.spaces = 0;
					ts.i+=3;
				}else if(ts.inNumber){
					ts.buffer[ts.bufferPos] = line[ts.i];
					ts.bufferPos++;
				}else{
					ts.rc = errBadNumber;
					printError(line, ts.i-1, errBadNumber, quiet);
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
			case '%':
				if(ts.inNumber){
					if(lastchar == 'e' && line[ts.i] == '-'){
						ts.buffer[ts.bufferPos] = line[ts.i];
						ts.bufferPos++;
					}else{
						err = addNumber(ts.tokenList, ts.buffer, ts.bufferPos, ts.spaces);
						printErrorIfNeeded(&ts, err, quiet);
						ts.inNumber = 0;
						ts.spaces = 0;
					}
				}
				if(!ts.inNumber || lastchar != 'e' || line[ts.i] != '-'){
					/*
					 * If we're at the "-" of an "1e-3", then don't
					 * add the "-" as a token.
					 */
					err = addSimpleToken(ts.tokenList, line[ts.i], ts.spaces);
					printErrorIfNeeded(&ts, err, quiet);
					ts.lastToken = line[ts.i];
					ts.spaces = 0;
				}
				break;

			case '_':
				err = addToken(ts.tokenList, tkLastResult, lastResult, 1 + ts.spaces);
				printErrorIfNeeded(&ts, err, quiet);
				ts.lastToken = '_';
				ts.spaces = 0;
				break;

			case ' ':
				ts.spaces++;
				break;

			case 10:
			case 13:
				/* End of line, space - ignore */
				break;

			case 'l':
				addPotentialNumber(&ts, quiet);
				if((ts.i < strlen(line) - 1) && line[ts.i+1] == 'n'){
					err = addToken(ts.tokenList, tkLn, 0.0, 2 + ts.spaces);
					printErrorIfNeeded(&ts, err, quiet);
					ts.spaces = 0;
					ts.i++;
				}else if((ts.i < strlen(line) - 2) && line[ts.i+1] == 'o' && line[ts.i+2] == 'g'){
					err = addToken(ts.tokenList, tkLog, 0.0, 3 + ts.spaces);
					printErrorIfNeeded(&ts, err, quiet);
					ts.spaces = 0;
					ts.i += 2;
				}else{
					printError(line, ts.i, errUnknownToken, quiet);
				}
				break;

			case 's':
				addPotentialNumber(&ts, quiet);
				if((ts.i < strlen(line) - 2) && line[ts.i+1] == 'i' && line[ts.i+2] == 'n'){
					err = addToken(ts.tokenList, tkSin, 0.0, 3 + ts.spaces);
					printErrorIfNeeded(&ts, err, quiet);
					ts.spaces = 0;
					ts.i += 2;
				}else if((ts.i < strlen(line) - 3) && line[ts.i+1] == 'q' && line[ts.i+2] == 'r' && line[ts.i+3] == 't'){
					err = addToken(ts.tokenList, tkSqrt, 0.0, 4 + ts.spaces);
					printErrorIfNeeded(&ts, err, quiet);
					ts.spaces = 0;
					ts.i += 3;
				}else{
					printError(line, ts.i, errUnknownToken, quiet);
				}
				break;

			case 'c':
				addPotentialNumber(&ts, quiet);
				if((ts.i < strlen(line) - 2) && line[ts.i+1] == 'o' && line[ts.i+2] == 's'){
					err = addToken(ts.tokenList, tkCos, 0.0, 3 + ts.spaces);
					printErrorIfNeeded(&ts, err, quiet);
					ts.spaces = 0;
					ts.i += 2;
				}else{
					printError(line, ts.i, errUnknownToken, quiet);
				}
				break;

			case 't':
				addPotentialNumber(&ts, quiet);
				if((ts.i < strlen(line) - 2) && line[ts.i+1] == 'a' && line[ts.i+2] == 'n'){
					err = addToken(ts.tokenList, tkTan, 0.0, 3 + ts.spaces);
					printErrorIfNeeded(&ts, err, quiet);
					ts.spaces = 0;
					ts.i += 2;
				}else{
					printError(line, ts.i, errUnknownToken, quiet);
				}
				break;

			default:
				ts.rc = errUnknownToken;
				printError(line, ts.i, errUnknownToken, quiet);
				break;
		}
		lastchar = line[ts.i];
	}
	addPotentialNumber(&ts, quiet);

	tokenList->next = ts.tokenList->next;
	return ts.rc;
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
int validate(tokenItem *tokenList, const char *line, int quiet)
{
	cToken lastToken;
	tokenItem *item;
	int openBrackets = 0;
	int closeBrackets = 0;
	int currentPos = 0;
	int negateValue = 0;
	errType err;
	int rc = errNoError;

	if(!tokenList || !tokenList->next){
		return errBadInput;
	}

	item = tokenList->next;
	lastToken = tkEndToken;

	while(item){
		currentPos += item->length;
		negateValue--; /* this allows us to detect when we have e.g. 4+-2 -> so we're looking backwards by two
		                * operators at once rather than just one. */

		switch(item->type){
			case tkNumber:
			case tkLastResult:
			case tkPi:
			case tkExp:
				if(lastToken == tkCloseBracket || lastToken == tkCCloseBracket){
					err = insertAfterToken(item, tkMultiply);
					if(err != errNoError){
						printError(line, currentPos, err, quiet);
						rc = err;
					}
				}else if(lastToken == tkNumber || lastToken == tkLastResult){
					/* Should never happen because tokenise() will produce
					 * an "invalid number" error.
					 */
					printError(line, currentPos, errDuplicateNumber, quiet);
					rc = errBadNumber;
				}else if(lastToken == tkMinus){
					if(negateValue == 1){
						/* Negate the value by changing the
						 * previous tkMinus to a tkNegation.
						 */
						item->prev->type = tkNegation;
					}
				}
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
				if(lastToken == tkCloseBracket || lastToken == tkCCloseBracket || lastToken == tkNumber || lastToken == tkLastResult){
					err = insertBeforeToken(item, tkMultiply, 0.0, 1);
					if(err != errNoError){
						printError(line, currentPos, err, quiet);
						rc = err;
					}
				}
				break;

			case tkPlus:
			case tkMultiply:
			case tkMultiplyX:
			case tkDivide:
			case tkPower:
			case tkMod:
				if(lastToken != tkNumber && lastToken != tkLastResult && lastToken != tkCloseBracket && lastToken != tkCCloseBracket){
					printError(line, currentPos-1, errInvalidOperator, quiet);
					rc = errInvalidOperator;
				}
				break;

			case tkMinus:
				/* prevent an inappropriate number of "-" signs */
				if(lastToken == tkMinus && negateValue == 1){
					printError(line, currentPos-1, errInvalidOperator, quiet);
					rc = errInvalidOperator;
				}else if(lastToken != tkNumber && lastToken != tkLastResult && lastToken != tkCloseBracket && lastToken != tkCCloseBracket){
					negateValue = 2;
				}
				break;

			case tkOpenBracket:
			case tkCOpenBracket:
				/* Insert * between )( */
				if(lastToken == tkCloseBracket || lastToken == tkCCloseBracket){
					err = insertAfterToken(item->prev, tkMultiply);
					if(err != errNoError){
						printError(line, currentPos, err, quiet);
						rc = err;
					}
				}else if(lastToken == tkMinus){
					if(negateValue == 1){
						deletePreviousToken(item);
						err = insertBeforeToken(item, tkNumber, -1.0, 1);
						if(err != errNoError){
							printError(line, currentPos, err, quiet);
							rc = err;
						}
						err = insertBeforeToken(item, tkMultiply, 0.0, 1);
						if(err != errNoError){
							printError(line, currentPos, err, quiet);
							rc = err;
						}
					}
				}
				openBrackets++;
				break;

			case tkCloseBracket:
			case tkCCloseBracket:
				if(lastToken != tkNumber && lastToken != tkLastResult && lastToken != tkCloseBracket && lastToken != tkCCloseBracket){
					printError(line, currentPos-1, errInvalidBracket, quiet);
					rc = errInvalidBracket;
				}
				closeBrackets++;

				if(closeBrackets > openBrackets){
					printError(line, currentPos-1, errInvalidBracket, quiet);
					rc = errInvalidBracket;
				}
				break;

			default:
				printError(line, currentPos, errUnknownToken, quiet);
				rc = errUnknownToken;
				break;
		}
		lastToken = item->type;
		item = item->next;
	}

	/* Check for bracket count mismatch */
	if(openBrackets != closeBrackets){
		printError(line, currentPos, errMismatchedBrackets, quiet);
		rc = errMismatchedBrackets;
	}

	/* Check that we haven't ended on an invalid operator, e.g. "= 1+2/"  */
	if(lastToken == tkPlus || lastToken == tkMinus \
			|| lastToken == tkMultiply || lastToken == tkMultiplyX || lastToken == tkDivide \
			|| lastToken == tkPower || lastToken == tkOpenBracket || lastToken == tkCOpenBracket \
			|| lastToken == tkMod || lastToken == tkLog || lastToken == tkLn \
			|| lastToken == tkSin || lastToken == tkCos || lastToken == tkTan \
			|| lastToken == tkASin || lastToken == tkACos || lastToken == tkATan \
			|| lastToken == tkSqrt){
		printError(line, currentPos-1, errInvalidOperator, quiet);
		rc = errInvalidOperator;
	}

	return rc;
}


const char *ctoken_to_string(cToken token)
{
	switch(token){
		case tkNumber:
			return "tkNumber";
		case tkMod:
			return "tkMod";
		case tkCOpenBracket:
			return "tkCOpenBracket";
		case tkCCloseBracket:
			return "tkCCloseBracket";
		case tkMultiply:
			return "tkMultiply";
		case tkPlus:
			return "tkPlus";
		case tkMinus:
			return "tkMinus";
		case tkDivide:
			return "tkDivide";
		case tkOpenBracket:
			return "tkOpenBracket";
		case tkCloseBracket:
			return "tkCloseBracket";
		case tkPower:
			return "tkPower";
		case tkLastResult:
			return "tkLastResult";
		case tkMultiplyX:
			return "tkMultiplyX";
		case tkNegation:
			return "tkNegation";
		case tkLog:
			return "tkLog";
		case tkLn:
			return "tkLn";
		case tkSin:
			return "tkSin";
		case tkCos:
			return "tkCos";
		case tkTan:
			return "tkTan";
		case tkASin:
			return "tkASin";
		case tkACos:
			return "tkACos";
		case tkATan:
			return "tkATan";
		case tkSqrt:
			return "tkSqrt";
		case tkPi:
			return "tkPi";
		case tkExp:
			return "tkExp";
		case tkEndToken:
			return "tkEndToken";

	}

	return "invalid";
}
