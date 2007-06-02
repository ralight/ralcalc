#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ccalc.h"
#include "list.h"

int assignPrecedence(tokenItem *tokenList)
{
	/*
	 * Everything should start off with a precedence 
	 * of zero when it is created.
	 */
	int maxPrecedence = 0;
	int currentPrecedence = 0;

	while(tokenList){
		switch(tokenList->type){
			case tkMultiply:
			case tkDivide:
			case tkPower:
			case tkOpenBracket:
			case tkExponent:
				currentPrecedence++;
				if(currentPrecedence > maxPrecedence){
					maxPrecedence = currentPrecedence;
				}
				break;
			case tkCloseBracket:
				currentPrecedence--;
				break;
			case tkNumber:
			case tkPlus:
			case tkMinus:
				// Leave with current precedence
				break;
			case tkEndToken:
				break;
		}
		tokenList->precedence = currentPrecedence;
		//printf("%d ", currentPrecedence);
		tokenList = tokenList->next;
	}
	//printf("\n");

	return maxPrecedence;
}


int validate(tokenItem *tokenList, const char *line)
{
	cToken lastToken;
	tokenItem *list;
	int openBrackets = 0;
	int closeBrackets = 0;
	int currentPos = 0;
	int negateValue = 0;
	errType err;
	int rc = 0;

	if(!tokenList || !tokenList->next) return -1;

	list = tokenList->next;
	lastToken = tkEndToken;

	while(list){
		currentPos += list->length;
		negateValue--; /* this allows us to detect when we have e.g. 4+-2 -> so we're looking backwards by two
						* operators at once rather than just one. */
		switch(list->type){
			case tkNumber:
				if(lastToken == tkCloseBracket){
					err = insertAfterToken(list, tkMultiply);
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
						list->value *= -1;
						// Delete the "-" from the token list
						deletePreviousToken(tokenList, list);
					}
				}
				break;
				
			case tkPlus:
			case tkMultiply:
			case tkDivide:
			case tkPower:
				if(lastToken != tkNumber && lastToken != tkCloseBracket && lastToken != tkEndToken){
					printError(line, currentPos-1, errInvalidOperator);
					rc = 1;
				}
				break;
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
					err = insertAfterToken(list, tkMultiply);
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
		lastToken = list->type;
		list = list->next;
	}

	if(openBrackets != closeBrackets){
		printError(line, currentPos, errMismatchedBrackets);
		rc = 1;
	}
	if(lastToken == tkPlus || lastToken == tkMinus \
			|| lastToken == tkMultiply || lastToken == tkDivide \
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
 * Extremely simplistic left to right no operator precedent 
 * processing
 */
errType process(tokenItem *tokenList, const char *line)
{
	double result;
	int firstNumber = 1;
	cToken lastToken;
	int maxPrecedence;

	if(!tokenList) return errBadInput;

	maxPrecedence = assignPrecedence(tokenList);

	while(tokenList){
		switch(tokenList->type){
			case tkNumber:
				if(!firstNumber){
					switch(lastToken){
						case tkPlus:
							result += tokenList->value;
							break;
						case tkMinus:
							result -= tokenList->value;
							break;
						case tkMultiply:
							result *= tokenList->value;
							break;
						case tkDivide:
							result /= tokenList->value;
							break;
						case tkPower:
							result = pow(result, tokenList->value);
							break;
						default:
							break;
					}
				}else{
					result = tokenList->value;
					firstNumber = 0;
				}
				break;
			case tkPlus:
			case tkMinus:
			case tkMultiply:
			case tkDivide:
			case tkPower:
				lastToken = tokenList->type;
				break;
			default:
				break;
		}
		tokenList = tokenList->next;
	}

	printf("%s = %g\n", line, result);
	return errNoError;
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

	assignPrecedence(tokenList.next);
	if(!hasError && tokenList.next){
		process(tokenList.next, line);
	}

	free(line);
	if(tokenList.next) freeList(tokenList.next);

	return 0;
}

