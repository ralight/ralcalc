#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ccalc.h"
#include "list.h"


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
				if(lastToken == tkCloseBracket){
					err = insertAfterToken(item, tkMultiply);
					if(err != errNoError){
						printError(line, currentPos, err);
						rc = 1;
					}
				}else if(lastToken == tkNumber){
					printError(line, currentPos, errDuplicateNumber);
					rc = 1;
				}else if(lastToken == tkMinus){
					if(negateValue == 1){
						// Negate the value
						item->value *= -1;
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
				if(lastToken != tkNumber && lastToken != tkCloseBracket && lastToken != tkEndToken){
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
				}else if(lastToken != tkNumber && lastToken != tkCloseBracket){
					negateValue = 2;
				}
				break;

			case tkOpenBracket:
				if(lastToken == tkCloseBracket){
					err = insertAfterToken(item, tkMultiply);
					if(err != errNoError){
						printError(line, currentPos, err);
						rc = 1;
					}
				}
				openBrackets++;
				break;

			case tkCloseBracket:
				if(lastToken != tkNumber && lastToken != tkCloseBracket){
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
			|| lastToken == tkPower || lastToken == tkOpenBracket){
		printError(line, currentPos-1, errInvalidOperator);
		rc = 1;
	}
	
	return rc;
}


void printError(const char *line, int pos, errType error)
{
	int i;

	printf("\nError: %s\n       ", line);
	for(i = 0; i < pos; i++){
		printf(" ");
	}
	printf("^");
	switch(error){
		case errMemory:
			printf(" out of memory\n");
			break;
		case errBadInput:
			printf(" bad input to function (internal)\n");
		case errUnknownToken:
			printf(" unknown token\n");
			break;
		case errBadNumber:
			printf(" bad number\n");
			break;
		case errMismatchedBrackets:
			printf(" mismatched brackets\n");
			break;
		case errDuplicateNumber:
			printf(" duplicate number\n");
			break;
		case errInvalidOperator:
			printf(" invalid operator\n");
			break;
		case errInvalidBracket:
			printf(" invalid bracket\n");
			break;
		default:
			printf(" unknown error (%d)\n", error);
			break;
	}
}


int tokenise(tokenItem *tokenList, const char *line)
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
 * Recursive processing with nested brackets taking 
 * precedent and left to right precedence within 
 * brackets.
 */
double process(tokenItem **tokenList, const char *line)
{
	double value = 0.0;
	double retval;
	cToken lastToken;
	int firstNumber = 1;
	tokenItem *item;

	if(!tokenList || !(*tokenList)) return 0.0;

	item = (*tokenList);
	while(item){
		switch(item->type){
			case tkOpenBracket:
				retval = process(&(item->next), line);
				if(!firstNumber){
					switch(lastToken){
						case tkPlus:
							value += retval;
							break;
						case tkMinus:
							value -= retval;
							break;
						case tkMultiply:
						case tkMultiplyX:
							value *= retval;
							break;
						case tkDivide:
							value /= retval;
							break;
						case tkPower:
							value = pow(value, retval);
							break;
						case tkExponent:
							/* FIXME */
							break;

						case tkNumber:
						case tkOpenBracket:
						case tkCloseBracket:
						case tkEndToken:
							/* FIXME - error condition */
							break;
					}
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
					switch(lastToken){
						case tkPlus:
							value += item->value;
							break;
						case tkMinus:
							value -= item->value;
							break;
						case tkMultiply:
						case tkMultiplyX:
							value *= item->value;
							break;
						case tkDivide:
							value /= item->value;
							break;
						case tkPower:
							value = pow(value, item->value);
							break;
						default:
							break;
					}
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


void doubleToString(double value, char *string, int len)
{
	memset(string, 0, len);

	if(value > 1000000000000000000000000.0){
		snprintf(string, len, "%gG", value / 1000000000000000000000000.0);
	}else if(value > 1000000000000000000000.0){
		snprintf(string, len, "%gG", value / 1000000000000000000000.0);
	}else if(value > 1000000000000000000.0){
		snprintf(string, len, "%gG", value / 1000000000000000000.0);
	}else if(value > 1000000000000000.0){
		snprintf(string, len, "%gG", value / 1000000000000000.0);
	}else if(value > 1000000000000.0){
		snprintf(string, len, "%gG", value / 1000000000000.0);
	}else if(value > 1000000000.0){
		snprintf(string, len, "%gG", value / 1000000000.0);
	}else if(value > 1000000.0){
		snprintf(string, len, "%gM", value / 1000000.0);
	}else if(value > 1000.0){
		snprintf(string, len, "%gk", value / 1000.0);
	}else if(value < 0.001){
		snprintf(string, len, "%gm", value * 1000.0);
	}else if(value < 0.000001){
		snprintf(string, len, "%gu", value * 1000000.0);
	}else if(value < 0.000000001){
		snprintf(string, len, "%gn", value * 1000000000.0);
	}else if(value < 0.000000000001){
		snprintf(string, len, "%gp", value * 1000000000000.0);
	}else if(value < 0.000000000000001){
		snprintf(string, len, "%gf", value * 1000000000000000.0);
	}else if(value < 0.000000000000000001){
		snprintf(string, len, "%ga", value * 1000000000000000000.0);
	}else if(value < 0.000000000000000000001){
		snprintf(string, len, "%gz", value * 1000000000000000000000.0);
	}else if(value < 0.000000000000000000000001){
		snprintf(string, len, "%gy", value * 1000000000000000000000000.0);
	}else{
		snprintf(string, len, "%g", value);
	}
}


int main(int argc, char *argv[])
{
	int i, j, k;
	char *line = NULL;
	int hasError = 0;
	int len = 1;
	int rc;
	tokenItem tokenList;
	tokenItem *thisItem;
	int errorPos;
	char resultStr[100];

	for(i = 1; i < argc; i++){
		len += strlen(argv[i]);
	}

	line = calloc(len, sizeof(char));

	i = 0;
	for(j = 1; j < argc; j++){
		for(k = 0; k < strlen(argv[j]); k++){
			if(argv[j][k] != ' '){
				line[i] = argv[j][k];
				i++;
			}
		}
	}

	/* First element always defined and not dynamic for less hassle */
	tokenList.next = NULL;
	tokenList.type = tkEndToken;

	rc = tokenise(&tokenList, line);
	if(rc>0) hasError = 1;

	rc = validate(&tokenList, line);
	if(rc>0){
		hasError = 1;
		thisItem = tokenList.next;
		errorPos = 0;

		while(thisItem && rc > 1){
			errorPos += thisItem->length;
			thisItem = thisItem->next;
			rc--;
		}

	}

	if(!hasError && tokenList.next){
		doubleToString(process(&(tokenList.next), line), resultStr, 100);
		printf("%s = %s\n", line, resultStr);
	}

	free(line);
	if(tokenList.next) freeList(tokenList.next);

	return 0;
}

