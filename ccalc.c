#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ccalc.h"
#include "list.h"

int validate(tokenItem *tokenList)
{
	cToken lastToken;
	tokenItem *list;
	int openBrackets = 0;
	int closeBrackets = 0;
	int token = 1;

	if(!tokenList || !tokenList->next) return -1;

	list = tokenList->next;
	lastToken = list->type;

	while(list->next){
		list = list->next;
		switch(list->type){
			case tkNumber:
				if(lastToken == tkOpenBracket){
					if(insertAfterToken(list, tkMultiply)) return token;
				}else if(lastToken == tkNumber){
					return token;
				}
				break;
			case tkPlus:
			case tkMinus:
			case tkMultiply:
			case tkDivide:
			case tkPower:
				if(lastToken != tkNumber || lastToken != tkCloseBracket){
					return token;
				}
				break;
			case tkOpenBracket:
				if(lastToken == tkCloseBracket){
					if(insertAfterToken(list, tkMultiply)) return token;
				}
				openBrackets++;
				break;
			case tkCloseBracket:
				if(lastToken != tkNumber){
					return token;
				}
				closeBrackets++;
				break;
			default:
				return token;
		}
		lastToken = list->type;
		token++;
	}

	if(openBrackets != closeBrackets) return -2;
	if(lastToken == tkPlus || lastToken == tkMinus \
			|| lastToken == tkMultiply || lastToken == tkDivide \
			|| lastToken == tkPower){
		return token;
	}
	
	return 0;
}


void printError(const char *line, int pos, errType error)
{
	int i;

	printf("\nError: %s\n       ", line);
	for(i = 0; i < pos - 1; i++){
		printf(" ");
	}
	printf("^");
	switch(error){
		case errBadNumber:
			printf(" bad number\n");
			break;
		default:
			printf(" unknown error\n");
			break;
	}
}


int tokenise(tokenItem *tokenList, const char *line, char *errors)
{
	int i;
	cToken lastToken = tkEndToken;
	char buffer[100];
	int bufferPos = 0;
	int inNumber = 0;
	int rc = 0;

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
				if(inNumber){
					if(addNumber(tokenList, buffer, bufferPos)){
						rc = 1;
						printError(line, i, errBadNumber);
					}
					inNumber = 0;
				}
				if(addSimpleToken(tokenList, line[i])) return i+1;
				lastToken = line[i];
				break;
		}
	}
	if(inNumber){
		if(addNumber(tokenList, buffer, bufferPos)) return i+1;
		inNumber = 0;
	}

	return rc;
}

int main(int argc, char *argv[])
{
	int i, j, k;
	char *line = NULL;
	char *errors = NULL;
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
	errors = calloc(len, sizeof(char));

	i = 0;
	for(j = 1; j < argc; j++){
		for(k = 0; k < strlen(argv[j]); k++){
			if(argv[j][k] != ' '){
				line[i] = argv[j][k];
				errors[i] = ' ';
				i++;
			}
		}
	}

	printf("%s\n", line);

	tokenList.next = NULL;
	tokenList.type = tkEndToken;
	rc = tokenise(&tokenList, line, errors);
	if(!tokenList.next) return 0;
	if(rc>0){
		hasError = 1;
		errors[rc+2] = '^';
	}
	rc = validate(&tokenList);
	if(rc>0){
		hasError = 1;
		thisItem = tokenList.next;
		errorPos = 0;

		while(thisItem && rc > 1){
			errorPos += thisItem->length;
			thisItem = thisItem->next;
			rc--;
		}

		errors[errorPos] = '^';
	}

	if(hasError){
		printf("Error in input:\n%s\n%s\n", line, errors);
	}

	free(line);
	if(tokenList.next){
		freeList(tokenList.next);
	}

	return 0;
}

