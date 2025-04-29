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


/*
 * addNumber()
 *
 * Parse a number and add it to the end of the token list.
 */
errType addNumber(tokenItem *tokenList, const char *buffer, int bufferPos, int spaces)
{
	int i;
	char str[bufferPos+2];
	int haveMultiplier = 0;
	int haveDecimalPoint = 0;
	int haveExponent = 0;
	int multiplierPos = -1;
	double multiplier = 1.0;
	double number = 0.0;

	if(!tokenList || !buffer || bufferPos < 0) return errBadInput;

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
errType addSimpleToken(tokenItem *tokenList, cToken token, int spaces)
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

	if(!tokenList) return errBadInput;

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

	if(!item || !item->prev) return errBadInput;
	if(item->prev->type == tkEndToken) return errBadInput;

	deleteItem = item->prev;

#ifdef DEBUG
	fprintf(stderr, "Deleting '%c' (%g)\n", deleteItem->type, deleteItem->value);
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
	fprintf(stderr, "Inserting %g\n", value);
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
int tokenise(tokenItem *tokenList, const char *line, double lastResult, int quiet)
{
	unsigned int i;
	cToken lastToken = tkEndToken;
	char buffer[100];
	char lastchar = '\0';
	int bufferPos = 0;
	int inNumber = 0;
	int rc = errNoError;
	errType err;
	int spaces = 0;

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
			case ',':
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
				if(i < strlen(line)-1 && line[i] == 'p' && line[i+1] == 'i'){
					/* pi */
					if(inNumber){
						err = addNumber(tokenList, buffer, bufferPos, spaces);
						if(err != errNoError){
							rc = err;
							printError(line, i-1, err, quiet);
						}
						inNumber = 0;
						spaces = 0;
					}else{
						err = addToken(tokenList, tkNumber, M_PI, 2 + spaces);
						if(err != errNoError){
							rc = err;
							printError(line, i-1, err, quiet);
						}
						spaces = 0;
					}
					i++;
				}else if(i < strlen(line)-2 && line[i] == 'e' && line[i+1] == 'x' && line[i+2] == 'p'){
					/* exp */
					if(inNumber){
						err = addNumber(tokenList, buffer, bufferPos, spaces);
						if(err != errNoError){
							rc = err;
							printError(line, i-1, err, quiet);
						}
						inNumber = 0;
						spaces = 0;
					}else{
						err = addToken(tokenList, tkNumber, M_E, 3 + spaces);
						if(err != errNoError){
							rc = err;
							printError(line, i-1, err, quiet);
						}
						spaces = 0;
					}
					i+=2;
				}else if(i < strlen(line)-3 && line[i] == 'a' && line[i+1] == 's' && line[i+2] == 'i' && line[i+3] == 'n'){
					/* asin */
					if(inNumber){
						err = addNumber(tokenList, buffer, bufferPos, spaces);
						if(err != errNoError){
							rc = err;
							printError(line, i-1, err, quiet);
						}
						inNumber = 0;
						spaces = 0;
					}else{
						err = addToken(tokenList, tkASin, 0.0, 4 + spaces);
						if(err != errNoError){
							rc = err;
							printError(line, i-1, err, quiet);
						}
						spaces= 0;
					}
					i+=3;
				}else if(i < strlen(line)-3 && line[i] == 'a' && line[i+1] == 'c' && line[i+2] == 'o' && line[i+3] == 's'){
					/* acos */
					if(inNumber){
						err = addNumber(tokenList, buffer, bufferPos, spaces);
						if(err != errNoError){
							rc = err;
							printError(line, i-1, err, quiet);
						}
						inNumber = 0;
						spaces = 0;
					}else{
						err = addToken(tokenList, tkACos, 0.0, 4 + spaces);
						if(err != errNoError){
							rc = err;
							printError(line, i-1, err, quiet);
						}
						spaces = 0;
					}
					i+=3;
				}else if(i < strlen(line)-3 && line[i] == 'a' && line[i+1] == 't' && line[i+2] == 'a' && line[i+3] == 'n'){
					/* atan */
					if(inNumber){
						err = addNumber(tokenList, buffer, bufferPos, spaces);
						if(err != errNoError){
							rc = err;
							printError(line, i-1, err, quiet);
						}
						inNumber = 0;
						spaces = 0;
					}else{
						err = addToken(tokenList, tkATan, 0.0, 4 + spaces);
						if(err != errNoError){
							rc = err;
							printError(line, i-1, err, quiet);
						}
						spaces = 0;
					}
					i+=3;
				}else if(inNumber){
					buffer[bufferPos] = line[i];
					bufferPos++;
				}else{
					rc = errBadNumber;
					printError(line, i-1, errBadNumber, quiet);
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
				if(inNumber){
					if(lastchar == 'e' && line[i] == '-'){
						buffer[bufferPos] = line[i];
						bufferPos++;
					}else{
						err = addNumber(tokenList, buffer, bufferPos, spaces);
						if(err != errNoError){
							rc = err;
							printError(line, i-1, err, quiet);
						}
						inNumber = 0;
						spaces = 0;
					}
				}
				if(!inNumber || lastchar != 'e' || line[i] != '-'){ 
					/* 
					 * If we're at the "-" of an "1e-3", then don't
					 * add the "-" as a token.
					 */
					err = addSimpleToken(tokenList, line[i], spaces);
					if(err != errNoError){
						rc = err;
						printError(line, i, err, quiet);
					}
					lastToken = line[i];
					spaces = 0;
				}
				break;

			case '_':
				err = addToken(tokenList, tkLastResult, lastResult, 1 + spaces);
				if(err != errNoError){
					rc = err;
					printError(line, i, err, quiet);
				}
				lastToken = '_';
				spaces = 0;
				break;

			case ' ':
				spaces++;
				break;

			case 10:
			case 13:
				/* End of line, space - ignore */
				break;

			case 'l':
				if(inNumber){
					err = addNumber(tokenList, buffer, bufferPos, spaces);
					if(err != errNoError){
						rc = err;
						printError(line, i-1, err, quiet);
					}
					inNumber = 0;
					spaces = 0;
				}
				if((i < strlen(line) - 1) && line[i+1] == 'n'){
					err = addToken(tokenList, tkLn, 0.0, 2 + spaces);
					if(err != errNoError){
						rc = err;
						printError(line, i, err, quiet);
					}
					spaces = 0;
					i++;
				}else if((i < strlen(line) - 2) && line[i+1] == 'o' && line[i+2] == 'g'){
					err = addToken(tokenList, tkLog, 0.0, 3 + spaces);
					if(err != errNoError){
						rc = err;
						printError(line, i, err, quiet);
					}
					spaces = 0;
					i += 2;
				}else{
					printError(line, i, errUnknownToken, quiet);
				}
				break;

			case 's':
				if(inNumber){
					err = addNumber(tokenList, buffer, bufferPos, spaces);
					if(err != errNoError){
						rc = err;
						printError(line, i-1, err, quiet);
					}
					inNumber = 0;
					spaces = 0;
				}
				if((i < strlen(line) - 2) && line[i+1] == 'i' && line[i+2] == 'n'){
					err = addToken(tokenList, tkSin, 0.0, 3 + spaces);
					if(err != errNoError){
						rc = err;
						printError(line, i, err, quiet);
					}
					spaces = 0;
					i += 2;
				}else if((i < strlen(line) - 3) && line[i+1] == 'q' && line[i+2] == 'r' && line[i+3] == 't'){
					err = addToken(tokenList, tkSqrt, 0.0, 4 + spaces);
					if(err != errNoError){
						rc = err;
						printError(line, i, err, quiet);
					}
					spaces = 0;
					i += 3;
				}else{
					printError(line, i, errUnknownToken, quiet);
				}
				break;

			case 'c':
				if(inNumber){
					err = addNumber(tokenList, buffer, bufferPos, spaces);
					if(err != errNoError){
						rc = err;
						printError(line, i-1, err, quiet);
					}
					inNumber = 0;
					spaces = 0;
				}
				if((i < strlen(line) - 2) && line[i+1] == 'o' && line[i+2] == 's'){
					err = addToken(tokenList, tkCos, 0.0, 3 + spaces);
					if(err != errNoError){
						rc = err;
						printError(line, i, err, quiet);
					}
					spaces = 0;
					i += 2;
				}else{
					printError(line, i, errUnknownToken, quiet);
				}
				break;

			case 't':
				if(inNumber){
					err = addNumber(tokenList, buffer, bufferPos, spaces);
					if(err != errNoError){
						rc = err;
						printError(line, i-1, err, quiet);
					}
					inNumber = 0;
					spaces = 0;
				}
				if((i < strlen(line) - 2) && line[i+1] == 'a' && line[i+2] == 'n'){
					err = addToken(tokenList, tkTan, 0.0, 3 + spaces);
					if(err != errNoError){
						rc = err;
						printError(line, i, err, quiet);
					}
					spaces = 0;
					i += 2;
				}else{
					printError(line, i, errUnknownToken, quiet);
				}
				break;

			default:
				rc = errUnknownToken;
				printError(line, i, errUnknownToken, quiet);
				break;
		}
		lastchar = line[i];
	}
	if(inNumber){
		err = addNumber(tokenList, buffer, bufferPos, spaces);
		if(err != errNoError){
			rc = err;
			printError(line, i-2, err, quiet);
		}
		inNumber = 0;
		spaces = 0;
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

	if(!tokenList || !tokenList->next) return errBadInput;

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
					err = insertAfterToken(item, tkMultiply);
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

