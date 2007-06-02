#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
	tkNumber,
	tkPlus,
	tkMinus,
	tkMultiply,
	tkDivide,
	tkPower,
	tkOpenBracket,
	tkCloseBracket,
	/* these below should be parsed as a number
	tkExponent,
	*/

	tkEndToken
} cToken;

typedef struct _tokenItem{
	struct _tokenItem *next;
	cToken type;
	double value;
	int length;
} tokenItem;

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

int tokenise(tokenItem *tokenList, const char *line)
{
	int i;
	cToken lastToken = tkEndToken;
	char buffer[100];
	int bufferPos = 0;
	int inNumber = 0;

	if(!tokenList || !line) return -1;

	memset(buffer, 0, 100);

	for(i = 0; i < strlen(line); i++){
		printf("%d %s.\n", i, buffer);
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
				if(inNumber){
					if(addNumber(tokenList, buffer, bufferPos)) return i+1;
					inNumber = 0;
				}
				if(addSimpleToken(tokenList, tkPlus)) return i+1;
				lastToken = tkPlus;
				break;
			case '-':
				if(inNumber){
					if(addNumber(tokenList, buffer, bufferPos)) return i+1;
					inNumber = 0;
				}
				if(addSimpleToken(tokenList, tkMinus)) return i+1;
				lastToken = tkPlus;
				break;
			case 'x':
				if(inNumber){
					if(addNumber(tokenList, buffer, bufferPos)) return i+1;
					inNumber = 0;
				}
				if(addSimpleToken(tokenList, tkMultiply)) return i+1;
				lastToken = tkPlus;
				break;
			case '/':
				if(inNumber){
					if(addNumber(tokenList, buffer, bufferPos)) return i+1;
					inNumber = 0;
				}
				if(addSimpleToken(tokenList, tkDivide)) return i+1;
				lastToken = tkPlus;
				break;
			case '[':
				if(inNumber){
					if(addNumber(tokenList, buffer, bufferPos)) return i+1;
					inNumber = 0;
				}
				if(addSimpleToken(tokenList, tkOpenBracket)) return i+1;
				lastToken = tkPlus;
				break;
			case ']':
				if(inNumber){
					if(addNumber(tokenList, buffer, bufferPos)) return i+1;
					inNumber = 0;
				}
				if(addSimpleToken(tokenList, tkCloseBracket)) return i+1;
				lastToken = tkPlus;
				break;
		}
	}
	if(inNumber){
		if(addNumber(tokenList, buffer, bufferPos)) return i+1;
		inNumber = 0;
	}

	return 0;
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
	tokenItem *thisItem, *nextItem;
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
	rc = tokenise(&tokenList, line);
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

		while(thisItem){
			errorPos += thisItem->length;
			thisItem = thisItem->next;
		}

		errors[errorPos] = '^';
	}

	if(hasError){
		printf("Error in input:\n%s\n%s\n", line, errors);
	}
	free(line);
	if(tokenList.next){
		thisItem = tokenList.next;

		while(thisItem){
			nextItem = thisItem->next;
			free(thisItem);
			thisItem = nextItem;
		}
	}


	return 0;
}

